#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_GRADES 5

typedef enum {
    SUCCESS,
    FILE_ERROR,
    MEMORY_ERROR,
    INPUT_ERROR,
    NOT_FOUND,
    UNKNOWN_ERROR
} ErrorCode;


typedef struct {
    int id;
    char* name;
    char* surname;
    char* group;
    unsigned char* grades; 
} Student;


ErrorCode validate_name(const char* str) {
    if (str == NULL || *str == '\0') {
        return INPUT_ERROR;
    }
    for (const char* c = str; *c; ++c) {
        if (!isalpha((unsigned)*c)) {
            return INPUT_ERROR;
        }
    }
    return SUCCESS;
}


ErrorCode init_student(Student* student, int id, const char* name, const char* surname, const char* group, const unsigned char grades[], size_t grades_count) {
    if (id < 0 || validate_name(name) != SUCCESS || validate_name(surname) != SUCCESS || group == NULL || *group == '\0') {
        return INPUT_ERROR; 
    }
    student->id = id;                     
    student->name = strdup(name);         
    student->surname = strdup(surname);   
    student->group = strdup(group);       

    student->grades = malloc(grades_count * sizeof(unsigned char)); 
    if (student->grades == NULL) {
        return MEMORY_ERROR; 
    }
    memcpy(student->grades, grades, grades_count * sizeof(unsigned char));
    return SUCCESS; 
}


ErrorCode read_students(const char* filepath, Student** students, size_t* student_count) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        return FILE_ERROR;
    }
    size_t capacity = 10; 
    *students = malloc(capacity * sizeof(Student)); 
    if (*students == NULL) {
        fclose(file);
        return MEMORY_ERROR; 
    }

    *student_count = 0;
    int id;
    char name[50], surname[50], group[50];
    unsigned char grades[NUM_GRADES];

    while (fscanf(file, "%d %49s %49s %49s %hhu %hhu %hhu %hhu %hhu", &id, name, surname, group,
        &grades[0], &grades[1], &grades[2], &grades[3], &grades[4]) == 9) {
        if (*student_count >= capacity) {
            capacity *= 2; 
            Student* temp = realloc(*students, capacity * sizeof(Student));
            if (temp == NULL) {
                fclose(file);
                return MEMORY_ERROR;
            }
            *students = temp;
        }
        if (init_student(&(*students)[*student_count], id, name, surname, group, grades, NUM_GRADES) == SUCCESS) {
            (*student_count)++;
        }
    }

    fclose(file); 
    return SUCCESS;
}


Student* search_by_id(Student* students, size_t count, int id) {
    for (size_t i = 0; i < count; i++) {
        if (students[i].id == id) {
            return &students[i];
        }
    }
    return NULL;
}


int compare_by_id(const void* a, const void* b) {
    return ((Student*)a)->id - ((Student*)b)->id;
}

int compare_by_surname(const void* a, const void* b) {
    return strcmp(((Student*)a)->surname, ((Student*)b)->surname); 
}

int compare_by_name(const void* a, const void* b) {
    return strcmp(((Student*)a)->name, ((Student*)b)->name); 
}

int compare_by_group(const void* a, const void* b) {
    return strcmp(((Student*)a)->group, ((Student*)b)->group); 
}


double calculate_average(const unsigned char* grades, size_t count) {
    int sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += grades[i]; 
    }
    return (double)sum / count; 
}


double calculate_overall_average(Student* students, size_t student_count, size_t grades_count) {
    double total_sum = 0;
    for (size_t i = 0; i < student_count; i++) {
        total_sum += calculate_average(students[i].grades, grades_count); 
    }
    return total_sum / student_count; 
}


ErrorCode write_student_to_trace(FILE* trace_file, const Student* student, double average) {
    if (!trace_file || !student) return FILE_ERROR;

    fprintf(trace_file, "ID: %d, Name: %s %s, Group: %s, Average Grade: %.2f\n",
        student->id, student->name, student->surname, student->group, average);
    return SUCCESS;
}


ErrorCode write_above_average_students(FILE* trace_file, Student* students, size_t student_count, size_t grades_count) {
    double overall_avg = calculate_overall_average(students, student_count, grades_count);
    for (size_t i = 0; i < student_count; i++) {
        double avg = calculate_average(students[i].grades, grades_count);
        if (avg > overall_avg) {
            fprintf(trace_file, "Name: %s %s\n", students[i].name, students[i].surname);
        }
    }
    return SUCCESS;
}


void print_students(const Student* students, size_t student_count) {
    for (size_t i = 0; i < student_count; i++) {
        printf("ID: %d, Name: %s, Surname: %s, Group: %s\n",
            students[i].id, students[i].name, students[i].surname, students[i].group);
    }
}



void interactive_mode(Student* students, size_t student_count, const char* trace_filepath) {
    int choice, id;
    FILE* trace_file = fopen(trace_filepath, "w");
    if (!trace_file) {
        printf("Error opening trace file.\n");
        return;
    }

    while (1) {
        printf("Menu:\n");
        printf("1. Find a student by ID\n");
        printf("2. Sort students by ID\n");
        printf("3. Bring out students with above-average scores.\n");
        printf("0. Exit\n");
        printf("Select an option:");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter student ID: ");
            scanf("%d", &id);
            Student* found = search_by_id(students, student_count, id);
            if (found) {
                double avg = calculate_average(found->grades, NUM_GRADES);
                write_student_to_trace(trace_file, found, avg);
                printf("The student is found and recorded in the file.\n");
            }
            else {
                printf("Student ID % d was not found.\n", id);
            }
            break;
        case 2:
            printf("Select a field to sort:\n");
            printf("1 - By ID\n");
            printf("2 - By surname\n");
            printf("3 - By name\n");
            printf("4 - By group\n");
            int sort_option;
            scanf("%d", &sort_option); 

            switch (sort_option) {
            case 1:
                qsort(students, student_count, sizeof(Student), compare_by_id);
                printf("Students are sorted by ID.\n");
                break;
            case 2:
                qsort(students, student_count, sizeof(Student), compare_by_surname);
                printf("Students are sorted by surname.\n");
                break;
            case 3:
                qsort(students, student_count, sizeof(Student), compare_by_name);
                printf("Students are sorted by name.\n");
                break;
            case 4:
                qsort(students, student_count, sizeof(Student), compare_by_group);
                printf("Students are sorted by group.\n");
                break;
            default:
                printf("Incorrect choice. Try again.\n");
                break;
            }

            print_students(students, student_count);
            break;
        case 3:
            write_above_average_students(trace_file, students, student_count, NUM_GRADES);
            printf("Students with above average scores are recorded on file.\n");
            break;
        case 0:
            fclose(trace_file);
            return;
        default:
            printf("Invalid input. Try again.\n");
        }
    }
}


void free_students(Student* students, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(students[i].name);
        free(students[i].surname);
        free(students[i].group);
        free(students[i].grades);
    }
    free(students);
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Not enough arguments.\n");
        return INPUT_ERROR;
    }

    const char* students_filepath = argv[1];
    const char* trace_filepath = argv[2];
    Student* students = NULL;
    size_t student_count = 0;

    if (read_students(students_filepath, &students, &student_count) != SUCCESS) {
        printf("Error reading student file.\n");
        return FILE_ERROR;
    }

    interactive_mode(students, student_count, trace_filepath);

    free_students(students, student_count);
    return SUCCESS;
}


