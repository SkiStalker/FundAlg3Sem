#include "functions.h"
#define MAX_LENGTH_OF_STRING_INCLUDES_MARKS 10
#define MAX_NAME_LENGTH 255

return_code is_correct_input(Student student) {
    if (strlen(student.name) == 0 || strlen(student.last_name) == 0
     || strlen(student.group) == 0) {
        return INVALID_INPUT;
    }
    for (size_t i = 0; i < strlen(student.name); ++i) {
        if (student.name[i] >= 'a' || student.name[i] <= 'z' ||
        student.name[i] >= 'A' || student.name[i] <= 'Z') {
            return INVALID_INPUT;
        }
    }
    for (size_t i = 0; i < strlen(student.last_name); ++i) {

        if (student.last_name[i] >= 'a' || student.last_name[i] <= 'z' ||
        student.last_name[i]<= 'A' || student.last_name[i] <= 'Z') {
            return INVALID_INPUT;
        }
    }
    return OK;
}

return_code upload_info(Student** output_info, int* size, FILE* input_file) {
    int cap = 2;
    Student* students = NULL;
    Student* tmp = NULL;
    int reading_cnt;
    *size = 0;
    students = (Student*)malloc(cap * sizeof(Student));

    students[*size].marks = (unsigned char*)malloc(MAX_LENGTH_OF_STRING_INCLUDES_MARKS);

    if (!students[*size].marks) {
        return MEMORY_ALLOCATION_ERROR;
    }

    while ((reading_cnt = fscanf(input_file, "%u%s%s%s%s\n",
            &students[*size].id, students[*size].name, students[*size].last_name, students[*size].group,students[*size].marks)) == 5) {

        if (!is_correct_input(students[*size])) {
            for (int i = 0; i <= *size; ++i) {
                free(students[i].marks);
            }
            free(students);
            return INVALID_INPUT;
        }
        // printf("%s\n", students[0].marks);
        ++(*size);
        students[*size].marks = (unsigned char*)malloc(MAX_LENGTH_OF_STRING_INCLUDES_MARKS);

        if (*size == cap) {
            cap *= 2;
            tmp = (Student *)realloc(students, cap * sizeof(Student));
            if (!tmp) {
                for (int j = 0; j < *size; ++j) {
                    free(students[j].marks);
                }
                free(students);
                return MEMORY_ALLOCATION_ERROR;
            }
            students = tmp;
        }

        students[*size].marks = (unsigned char*)malloc(MAX_LENGTH_OF_STRING_INCLUDES_MARKS);
        if (!students[*size].marks) {
            for (int j = 0; j < *size; ++j) {
                free(students[j].marks);
            }
            free(students);
            return MEMORY_ALLOCATION_ERROR;
        }

    }

    if (reading_cnt > 5 && reading_cnt < 1) {
        return INVALID_INPUT;
    }

    if (!output_info) {
        for (int j = 0; j < *size; ++j) {
            free(students[j].marks);
        }
        free(students);
        return DEREFERENCING_NULL_POINTER_ATTEMPT;
    }

    *output_info = students;
    return OK;
}

return_code get_student_info_by_parameter(Student* result, char* parameter, char* search_str, Student* students, int size) {
    if (!result) {
        return MEMORY_ALLOCATION_ERROR;
    }
    switch(parameter[0]) {
        case 'i':
            // id
            for (int i = 0; i <= size; ++i) {
                if (students[i].id == (unsigned int)atoi(search_str)) {
                    *result = students[i];
                    return OK;
                    break;
                }
            }
            return INVALID_INPUT;
        case 'n':
            // name
            for (int i = 0; i <= size; ++i) {
                if (strcmp(students[i].name, search_str) == 0) {
                    *result = students[i];
                    return OK;
                    break;
                }
            }
            return INVALID_INPUT;
        case 'l':
            //last name
            for (int i = 0; i <= size; ++i) {
                if (strcmp(students[i].last_name, search_str) == 0) {
                    *result = students[i];
                    return OK;
                    break;
                }
            }
            return INVALID_INPUT;
        case 'g':
            // group
            for (int i = 0; i <= size; ++i) {
                if (strcmp(students[i].group, search_str) == 0) {
                    *result = students[i];
                    return OK;
                    break;
                }
            }
            return INVALID_INPUT;
        default:
            return INVALID_INPUT;
    }
    return OK;
}

int comp_by_id(const void *a, const void *b) {
    const Student* st_a = (const Student*)a;
    const Student* st_b = (const Student*)b;

    return st_a->id - st_b->id;

}

int comp_by_name(const void *a, const void *b) {
    const Student* st_a = (const Student*)a;
    const Student* st_b = (const Student*)b;

    return strcmp(st_a->name, st_b->name);

}
int comp_by_lname(const void *a, const void *b) {
    const Student* st_a = (const Student*)a;
    const Student* st_b = (const Student*)b;

    return strcmp(st_a->last_name, st_b->last_name);

}
int comp_by_group(const void *a, const void *b) {
    const Student* st_a = (const Student*)a;
    const Student* st_b = (const Student*)b;

    return strcmp(st_a->group, st_b->group);
}

double get_average_of_marks(Student student) {
    unsigned char* marks = student.marks;
    int i = 0;
    double sum = 0;

    while(marks[i] != '\0') {
        sum += marks[i] - '0';
        ++i;
    }

    return sum / i;
}

double calculate_student_average(Student* students, int size) {
    double total_sum = 0;
    int total_marks_count = 0;

    for (int i = 0; i < size; i++) {
        unsigned char* marks = students[i].marks;
        int j = 0;
        while (marks[j] != '\0') {
            total_sum += marks[j] - '0';
            ++j;
        }
        total_marks_count += j;
    }

    return (total_marks_count == 0) ? 0 : total_sum / total_marks_count;
}

void write_best_students_in_the_file(Student* students, int size, FILE* output_file) {
    double overall_average = calculate_student_average(students, size);
    fprintf(output_file, "best students:\n");

    for (int i = 0; i < size; i++) {
        double student_average = get_average_of_marks(students[i]);
        if (student_average > overall_average) {
            fprintf(output_file, "name: %s %s, group: %s, average grade: %.2f\n",
                    students[i].name, students[i].last_name, students[i].group, student_average);
        }
    }
}

void interactive_dialog(Student* students, int size, FILE* output_file) {
    int choice;
    printf("Choose:\n");
    printf("1. search by ID\n");
    printf("2. search by last name\n");
    printf("3. search by name\n");
    printf("4. search by number of group\n");
    printf("5. sort by ID\n");
    printf("6. sort by last name\n");
    printf("7. sort by name\n");
    printf("8. sort by group\n");
    printf("9. get the best student\n");
    printf("10. exit\n");
    char id[MAX_NAME_LENGTH];
    char lname[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    do {
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter ID: \n");
                scanf("%s", id);
                Student found_1;
                return_code rc_1 = get_student_info_by_parameter(&found_1, "id", id, students, size);
                if (rc_1 == OK) {
                    printf("\n");
                    print_student_info(&found_1);

                    fprintf(output_file, "ID: %u\n", found_1.id);
                    fprintf(output_file, "ФИО: %s %s %s\n", found_1.last_name, found_1.name, found_1.group);
                    double avg_mark = get_average_of_marks(found_1);
                    fprintf(output_file, "average: %lf\n", avg_mark);


                } else {
                    printf("Doesn't exist.\n");
                }
                break;

            case 2:
                printf("Enter last name: \n");
                scanf("%s", lname);
                Student found_2;
                return_code rc_2 = get_student_info_by_parameter(&found_2, "last_name", lname, students, size);
                if (rc_2 == OK) {
                    print_student_info(&found_2);
                } else {
                    printf("Doesn't exist.\n");
                }
                break;

            case 3:
                printf("Enter name: \n");
                scanf("%s", name);
                Student found_3;
                return_code rc_3 = get_student_info_by_parameter(&found_3, "name", name, students, size);
                if (rc_3 == OK) {
                    print_student_info(&found_3);
                } else {
                    printf("Doesn't exist.\n");
                }
                break;

            case 4:
                printf("Enter group number: \n");
                scanf("%s", group);
                Student found_4;
                return_code rc_4 = get_student_info_by_parameter(&found_4, "group", group, students, size);
                if (rc_4 == OK) {
                    print_student_info(&found_4);
                } else {
                    printf("Doesn't exist.\n");
                }
                break;
            case 5:

                qsort(students, size, sizeof(Student), comp_by_id);
                printf("Sorted by ID:\n");
                for (int i = 0; i < size; i++) {
                    print_student_info(&students[i]);
                }
                break;

            case 6:
                qsort(students, size, sizeof(Student), comp_by_lname);
                printf("Sorted by last name:\n");
                for (int i = 0; i < size; i++) {
                    print_student_info(&students[i]);
                }
                break;

            case 7:
                qsort(students, size, sizeof(Student), comp_by_name);
                printf("Sorted by name:\n");
                for (int i = 0; i < size; i++) {
                    print_student_info(&students[i]);
                }
                break;

            case 8:
                qsort(students, size, sizeof(Student), comp_by_group);
                printf("Sorted by group:\n");
                for (int i = 0; i < size; i++) {
                    print_student_info(&students[i]);
                }
                break;
            case 9:
                write_best_students_in_the_file(students, size, output_file);
            default:
                break;
        }
    } while (choice != 10);

    for (int i = 0; i < size; ++i) {
        free(students[i].marks);
    }
    free(students);
}

void print_student_info(Student* student) {
    printf("ID: %u\n", student->id);
    printf("name: %s\n", student->name);
    printf("last name: %s\n", student->last_name);
    printf("group: %s\n", student->group);
    printf("marks: %s\n", student->marks);
    printf("\n");
}