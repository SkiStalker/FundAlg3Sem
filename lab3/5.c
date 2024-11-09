#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    unsigned int id;
    char name[50];
    char surname[50];
    char group[20];
    unsigned char *grades;
} Student;

int loadStudents(const char *filename, Student **students, unsigned int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    *count = 0;
    while (!feof(file)) {
        if (fgetc(file) == '\n') {
            (*count)++;
        }
    }
    rewind(file);

    *students = (Student *)malloc(*count * sizeof(Student));
    if (!*students) {
        perror("Memory allocation error");
        fclose(file);
        return 0;
    }

    for (unsigned int i = 0; i < *count; i++) {
        fscanf(file, "%u %s %s %s", &(*students)[i].id, (*students)[i].name, 
               (*students)[i].surname, (*students)[i].group);
        (*students)[i].grades = (unsigned char *)malloc(5 * sizeof(unsigned char));
        for (unsigned int j = 0; j < 5; j++) {
            fscanf(file, "%hhu", &(*students)[i].grades[j]);
        }
    }
    fclose(file);
    return 1;
}

Student *findById(Student *students, unsigned int count, unsigned int id) {
    for (unsigned int i = 0; i < count; i++) {
        if (students[i].id == id) {
            return &students[i];
        }
    }
    return NULL;
}

int findBySurname(Student *students, unsigned int count, const char *surname) {
    int found = 0;
    for (unsigned int i = 0; i < count; i++) {
        if (strcmp(students[i].surname, surname) == 0) {
            printf("ID: %u, Name: %s, Surname: %s, Group: %s\n", 
                   students[i].id, students[i].name, students[i].surname, students[i].group);
            found = 1;
        }
    }
    if (!found) {
        printf("Студенты с фамилией %s не найдены.\n", surname);
    }
    return found;
}

int findByName(Student *students, unsigned int count, const char *name) {
    int found = 0;
    for (unsigned int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name) == 0) {
            printf("ID: %u, Name: %s, Surname: %s, Group: %s\n", 
                   students[i].id, students[i].name, students[i].surname, students[i].group);
            found = 1;
        }
    }
    if (!found) {
        printf("Студенты с именем %s не найдены.\n", name);
    }
    return found;
}

int findByGroup(Student *students, unsigned int count, const char *group) {
    int found = 0;
    for (unsigned int i = 0; i < count; i++) {
        if (strcmp(students[i].group, group) == 0) {
            printf("ID: %u, Name: %s, Surname: %s, Group: %s\n", 
                   students[i].id, students[i].name, students[i].surname, students[i].group);
            found = 1;
        }
    }
    if (!found) {
        printf("Студенты в группе %s не найдены.\n", group);
    }
    return found;
}

int compareById(const void *a, const void *b) {
    return ((Student *)a)->id - ((Student *)b)->id;
}

int compareByName(const void *a, const void *b) {
    return strcmp(((Student *)a)->name, ((Student *)b)->name);
}

int compareBySurname(const void *a, const void *b) {
    return strcmp(((Student *)a)->surname, ((Student *)b)->surname);
}

int compareByGroup(const void *a, const void *b) {
    return strcmp(((Student *)a)->group, ((Student *)b)->group);
}

int logStudentById(const char *logFilename, Student *student) {
    FILE *file = fopen(logFilename, "a");
    if (!file) {
        perror("Error opening log file");
        return 0;
    }

    float averageGrade = 0;
    for (unsigned int i = 0; i < 5; i++) {
        averageGrade += student->grades[i];
    }
    averageGrade /= 5;

    fprintf(file, "ID: %u, Name: %s, Surname: %s, Group: %s, Average Grade: %.2f\n",
            student->id, student->name, student->surname, student->group, averageGrade);

    fclose(file);
    return 1;
}

int logAboveAverageStudents(const char *logFilename, Student *students, unsigned int count) {
    FILE *file = fopen(logFilename, "a");
    if (!file) {
        perror("Error opening log file");
        return 0;
    }

    float totalAverage = 0;
    for (unsigned int i = 0; i < count; i++) {
        for (unsigned int j = 0; j < 5; j++) {
            totalAverage += students[i].grades[j];
        }
    }
    totalAverage /= (count * 5);

    fprintf(file, "Students with above average grades:\n");
    for (unsigned int i = 0; i < count; i++) {
        float studentAverage = 0;
        for (unsigned int j = 0; j < 5; j++) {
            studentAverage += students[i].grades[j];
        }
        studentAverage /= 5;

        if (studentAverage > totalAverage) {
            fprintf(file, "%s %s\n", students[i].surname, students[i].name);
        }
    }

    fclose(file);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file> <log_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Student *students;
    unsigned int count;
    if (!loadStudents(argv[1], &students, &count)) {
        return EXIT_FAILURE;
    }

    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Найти студента по ID\n");
        printf("2. Найти студента по фамилии\n");
        printf("3. Найти студента по имени\n");
        printf("4. Найти студента по группе\n");
        printf("5. Сортировать студентов по ID\n");
        printf("6. Сортировать студентов по имени\n");
        printf("7. Сортировать студентов по фамилии\n");
        printf("8. Сортировать студентов по группе\n");
        printf("9. Записать данные студента в лог-файл по ID\n");
        printf("10. Записать студентов с оценками выше среднего в лог-файл\n");
        printf("11. Выход\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);

        if (choice == 11) {
            break;
        }

        switch (choice) {
            case 1: {
                unsigned int id;
                printf("Введите ID студента: ");
                if (scanf("%u", &id) != 1) {
                    printf("Ошибка ввода ID!\n");
                    break;
                }

                if (id == UINT_MAX) {
                    printf("ID переполнен! Введите корректное значение.\n");
                    break;
                }

                Student *student = findById(students, count, id);
                if (student) {
                    printf("Студент найден: %s %s, Группа: %s\n", student->name, student->surname, student->group);
                } else {
                    printf("Студент с ID %u не найден.\n", id);
                }
                break;
            }
            case 2: {
                char surname[50];
                printf("Введите фамилию: ");
                scanf("%s", surname);
                findBySurname(students, count, surname);
                break;
            }
            case 3: {
                char name[50];
                printf("Введите имя: ");
                scanf("%s", name);
                findByName(students, count, name);
                break;
            }
            case 4: {
                char group[20];
                printf("Введите группу: ");
                scanf("%s", group);
                findByGroup(students, count, group);
                break;
            }
            case 5:
                qsort(students, count, sizeof(Student), compareById);
                printf("Студенты отсортированы по ID.\n");
                break;
            case 6:
                qsort(students, count, sizeof(Student), compareByName);
                printf("Студенты отсортированы по имени.\n");
                break;
            case 7:
                qsort(students, count, sizeof(Student), compareBySurname);
                printf("Студенты отсортированы по фамилии.\n");
                break;
            case 8:
                qsort(students, count, sizeof(Student), compareByGroup);
                printf("Студенты отсортированы по группе.\n");
                break;
            case 9: {
                unsigned int id;
                printf("Введите ID студента: ");
                if (scanf("%u", &id) != 1) {
                    printf("Ошибка ввода ID!\n");
                    break;
                }

                if (id == UINT_MAX) {
                    printf("ID переполнен! Введите корректное значение.\n");
                    break;
                }

                Student *student = findById(students, count, id);
                if (student) {
                    if (logStudentById(argv[2], student)) {
                        printf("Данные студента успешно записаны в лог-файл.\n");
                    } else {
                        printf("Ошибка записи в лог-файл.\n");
                    }
                } else {
                    printf("Студент с ID %u не найден.\n", id);
                }
                break;
            }
            case 10:
                if (logAboveAverageStudents(argv[2], students, count)) {
                    printf("Студенты с оценками выше среднего записаны в лог-файл.\n");
                } else {
                    printf("Ошибка записи в лог-файл.\n");
                }
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
                break;
        }
    }

    for (unsigned int i = 0; i < count; i++) {
        free(students[i].grades);
    }
    free(students);

    return 0;
}
