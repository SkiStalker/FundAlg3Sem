#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_NAME_LEN 50
#define NUM_EXAMS 5

typedef enum StatusCode {
    SUCCESS = 0,
    ERR_FILE_OPEN,
    ERR_MEMORY_ALLOCATION,
    ERR_INVALID_DATA,
    ERR_NOT_FOUND,
    ERR_UNKNOWN,
    ERR_MEMORY
} StatusCode;

typedef struct Student {
    unsigned int id;
    char name[MAX_NAME_LEN];
    char surname[MAX_NAME_LEN];
    char group[MAX_NAME_LEN];
    unsigned char *grades;
} Student;

typedef struct StudentArray {
    Student *students;
    size_t count;
} StudentArray;


StatusCode free_student_array(StudentArray *array) {
    if (array == NULL) {
        return ERR_MEMORY; 
    }

    if (array->students != NULL) {
        for (size_t i = 0; i < array->count; i++) {
            free(array->students[i].grades);
            array->students[i].grades = NULL; 
        }
        free(array->students); 
        array->students = NULL; 
    }

    return SUCCESS;
}


int is_valid_string(const char *str) {
    while (*str) {
        if (!isalpha(*str)) return 0;
        str++;
    }
    return 1;
}

int is_valid_id(const char *str) {
    char *end;
    unsigned long id = strtoul(str, &end, 10);
    return (*end == '\0' && id <= UINT_MAX);
}

StatusCode load_students(const char *filename, StudentArray *array) {
    FILE *file = fopen(filename, "r");
    if (!file) return ERR_FILE_OPEN;

    size_t capacity = 10;
    array->students = malloc(capacity * sizeof(Student));
    if (!array->students) {
        fclose(file);
        return ERR_MEMORY_ALLOCATION;
    }
    array->count = 0;

    while (1) {
        Student *s = &array->students[array->count];
        s->grades = malloc(NUM_EXAMS * sizeof(unsigned char));
        if (!s->grades) {
            free_student_array(array);
            fclose(file);
            return ERR_MEMORY_ALLOCATION;
        }

        if (fscanf(file, "%u %49s %49s %49s", &s->id, s->name, s->surname, s->group) != 4) {
            free(s->grades);
            break; 
        }
        
        for (int i = 0; i < NUM_EXAMS; ++i) {
            if (fscanf(file, "%hhu", &s->grades[i]) != 1) {
                free(s->grades);
                free_student_array(array);
                fclose(file);
                return ERR_INVALID_DATA; 
            }
        }

        if (!is_valid_string(s->name) || !is_valid_string(s->surname)) {
            free(s->grades);
            free_student_array(array);
            fclose(file);
            return ERR_INVALID_DATA;
        }

        array->count++;
        if (array->count >= capacity) {
            capacity *= 2;
            Student *temp = realloc(array->students, capacity * sizeof(Student));
            if (!temp) {
                free_student_array(array);
                fclose(file);
                return ERR_MEMORY_ALLOCATION;
            }
            array->students = temp;
        }
    }

    fclose(file);
    return SUCCESS;
}

float calculate_student_average(const Student *s) {
    int sum = 0;
    for (int i = 0; i < NUM_EXAMS; i++) {
        sum += s->grades[i];
    }
    return ((float)sum / NUM_EXAMS);
}

StatusCode find_student_by_id(const StudentArray *array, unsigned int id, const Student **result) {
    for (size_t i = 0; i < array->count; i++) {
        if (array->students[i].id == id) {
            *result = &array->students[i];
            return SUCCESS;
        }
    }
    return ERR_NOT_FOUND;
}

float calculate_average_all(const StudentArray *array) {
    float total_sum = 0.0f;
    for (size_t i = 0; i < array->count; i++) {
        total_sum += calculate_student_average(&array->students[i]);
    }
    return total_sum / array->count;
}

StudentArray filter_above_average(const StudentArray *array, float average) {
    StudentArray result = {NULL, 0};
    result.students = malloc(array->count * sizeof(Student));
    if (!result.students) return result;

    for (size_t i = 0; i < array->count; i++) {
        if (calculate_student_average(&array->students[i]) > average) {
            result.students[result.count++] = array->students[i];
        }
    }
    return result;
}

int compare_by_id(const void *a, const void *b) {
    return ((const Student *)a)->id - ((const Student *)b)->id;
}

int compare_by_surname(const void *a, const void *b) {
    return strcmp(((const Student *)a)->surname, ((const Student *)b)->surname);
}

int compare_by_name(const void *a, const void *b) {
    return strcmp(((const Student *)a)->name, ((const Student *)b)->name);
}

int compare_by_group(const void *a, const void *b) {
    return strcmp(((const Student *)a)->group, ((const Student *)b)->group);
}

StatusCode sort_students(StudentArray *array, int (*comparator)(const void *, const void *)) {
    if (!array || !array->students) return ERR_INVALID_DATA;
    qsort(array->students, array->count, sizeof(Student), comparator);
    return SUCCESS;
}

StatusCode print_student_trace(const Student *student, const char *trace_filename) {
    FILE *trace_file = fopen(trace_filename, "a");
    if (!trace_file) return ERR_FILE_OPEN;

    float average_grade = calculate_student_average(student);
    fprintf(trace_file, "ID: %u, Name: %s %s, Group: %s, Average: %.2f\n",
            student->id, student->name, student->surname, student->group, average_grade);
    fclose(trace_file);
    return SUCCESS;
}

StatusCode print_above_average_trace(const StudentArray *array, const char *trace_filename) {
    FILE *trace_file = fopen(trace_filename, "a");
    if (!trace_file) return ERR_FILE_OPEN;

    float average = calculate_average_all(array);
    fprintf(trace_file, "Students above average (%.2f):\n", average);
    for (size_t i = 0; i < array->count; i++) {
        float student_average = calculate_student_average(&array->students[i]);
        if (student_average > average) {
            fprintf(trace_file, "%s %s\n", array->students[i].surname, array->students[i].name);
        }
    }

    fclose(trace_file);
    return SUCCESS;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <data_filename> <trace_filename>\n", argv[0]);
        return 1;
    }

    StudentArray array = {NULL, 0};
    StatusCode status = load_students(argv[1], &array);
    if (status != SUCCESS) {
        fprintf(stderr, "Error loading students: %d\n", status);
        return status;
    }

    char command[10];
    unsigned int id;

    while (1) {
        printf("\nOptions:\n");
        printf("1 - Find student by ID and log\n");
        printf("2 - Log students above average grade\n");
        printf("3 - Sort by surname and display\n");
        printf("4 - Sort by ID and display\n");
        printf("5 - Sort by name and display\n");
        printf("6 - Sort by group and display\n");
        printf("7 - Exit\n");
        printf("Enter command: ");
        scanf("%9s", command);

        if (strcmp(command, "1") == 0) {
            printf("Enter student ID: ");
            if (scanf("%u", &id) != 1) {
                fprintf(stderr, "Invalid input for ID.\n");
                continue;
            }

            const Student *student;
            if (find_student_by_id(&array, id, &student) == SUCCESS) {
                printf("Found student: %s %s (Group: %s)\n", student->name, student->surname, student->group);
                print_student_trace(student, argv[2]);
            } else {
                fprintf(stderr, "Student with ID %u not found.\n", id);
            }
        } else if (strcmp(command, "2") == 0) {
            float average = calculate_average_all(&array);
            StudentArray above_average = filter_above_average(&array, average);
            print_above_average_trace(&above_average, argv[2]);
            free_student_array(&above_average);
        } else if (strcmp(command, "3") == 0) {
            sort_students(&array, compare_by_surname);
            for (size_t i = 0; i < array.count; i++) {
                printf("%s %s (Group: %s)\n", array.students[i].surname, array.students[i].name, array.students[i].group);
            }
        } else if (strcmp(command, "4") == 0) {
            sort_students(&array, compare_by_id);
            for (size_t i = 0; i < array.count; i++) {
                printf("%u: %s %s (Group: %s)\n", array.students[i].id, array.students[i].name, array.students[i].surname, array.students[i].group);
            }
        } else if (strcmp(command, "5") == 0) {
            sort_students(&array, compare_by_name);
            for (size_t i = 0; i < array.count; i++) {
                printf("%s %s (Group: %s)\n", array.students[i].name, array.students[i].surname, array.students[i].group);
            }
        } else if (strcmp(command, "6") == 0) {
            sort_students(&array, compare_by_group);
            for (size_t i = 0; i < array.count; i++) {
                printf("%s %s (Group: %s)\n", array.students[i].group, array.students[i].surname, array.students[i].name);
            }
        } else if (strcmp(command, "7") == 0) {
            break;
        } else {
            fprintf(stderr, "Invalid command.\n");
        }
    }

    free_student_array(&array);
    return 0;
}
