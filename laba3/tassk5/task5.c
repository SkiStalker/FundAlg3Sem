#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Определение структуры Student
typedef struct {
    unsigned int id;
    char name[50];
    char surname[50];
    char group[20];
    unsigned char *grades; // Динамический массив из 5 оценок
} Student;

// Прототипы функций
int write_trace_to_file(const char *filename, const char *message);
double calculate_average(unsigned char *grades, int count);
void print_student_simple(const Student *student);
void print_student_with_average(const Student *student);
int compare_by_ID(const void *a, const void *b);
int compare_by_surname(const void *a, const void *b);
int compare_by_name(const void *a, const void *b);
int compare_by_group(const void *a, const void *b);
Student* loadStudents(const char *filename, int *count);
int search_student_by_ID(Student *students, int count, unsigned int id, const char *traceFile);
void search_students_by_surname(Student *students, int count, const char *surname, const char *traceFile);
void search_students_by_name(Student *students, int count, const char *name, const char *traceFile);
void search_students_by_group(Student *students, int count, const char *group, const char *traceFile);
void find_students_above_average(Student *students, int count);
void freeStudents(Student *students, int count);
int is_valid_string(const char *str, int is_name);
void display_menu();

// Функция для записи сообщений в трассировочный файл
int write_trace_to_file(const char *filename, const char *message) {
    FILE *file = fopen(filename, "a");
    if (file) {
        fprintf(file, "%s\n", message);
        fclose(file);
    } else {
        printf("Не удалось открыть файл для записи трассировки.\n");
        return -1;
    }
    return 1;
}

// Функция для расчета средней оценки
double calculate_average(unsigned char *grades, int count) {
    if (count == 0) return 0.0;
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += grades[i];
    }
    return (double)sum / count;
}

// Функция для вывода информации о студенте
void print_student_simple(const Student *student) {
    printf("ID: %u, Имя: %s %s, Группа: %s", 
        student->id, student->name, student->surname, student->group);
    printf(", Оценки: ");
    for (int i = 0; i < 5; i++) {
        printf("%hhu ", student->grades[i]);
    }
    printf("\n");
}

// Функция для вывода информации о студенте с средним баллом
void print_student_with_average(const Student *student) {
    printf("ID: %u, Имя: %s %s, Группа: %s, Средняя оценка: %.2f\n", 
        student->id, student->name, student->surname, student->group,
        calculate_average(student->grades, 5));
}

// Функция-компаратор для сортировки по ID
int compare_by_ID(const void *a, const void *b) {
    unsigned int idA = ((Student *)a)->id;
    unsigned int idB = ((Student *)b)->id;
    if (idA < idB) return -1;
    if (idA > idB) return 1;
    return 0;
}

// Функция-компаратор для сортировки по фамилии
int compare_by_surname(const void *a, const void *b) {
    const char *surnameA = ((Student *)a)->surname;
    const char *surnameB = ((Student *)b)->surname;
    return strcmp(surnameA, surnameB);
}

// Функция-компаратор для сортировки по имени
int compare_by_name(const void *a, const void *b) {
    const char *nameA = ((Student *)a)->name;
    const char *nameB = ((Student *)b)->name;
    return strcmp(nameA, nameB);
}

// Функция-компаратор для сортировки по группе
int compare_by_group(const void *a, const void *b) {
    const char *groupA = ((Student *)a)->group;
    const char *groupB = ((Student *)b)->group;
    return strcmp(groupA, groupB);
}

// Функция для проверки допустимости строки
int is_valid_string(const char *str, int is_name) {
    if (str == NULL || strlen(str) == 0) {
        return 0;
    }
    if (is_name) {
        for (int i = 0; str[i]; i++) {
            if (!isalpha(str[i])) {
                return 0;
            }
        }
    }
    return 1;
}

// Функция для загрузки студентов из файла
Student* loadStudents(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Не удалось открыть файл");
        return NULL;
    }

    if (fscanf(file, "%d", count) != 1 || *count <= 0) {
        fprintf(stderr, "Не удалось прочитать количество студентов или неверное значение.\n");
        fclose(file);
        return NULL;
    }

    Student *students = malloc(*count * sizeof(Student));
    if (!students) {
        perror("Не удалось выделить память для студентов");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < *count; i++) {
        fscanf(file, "%u %s %s %s", &students[i].id, students[i].name, students[i].surname, students[i].group);
        students[i].grades = malloc(5 * sizeof(unsigned char));
        for (int j = 0; j < 5; j++) {
            fscanf(file, "%hhu", &students[i].grades[j]);
        }
    }

    fclose(file);
    return students;
}

// Поиск студента по ID и запись в файл
int search_student_by_ID(Student *students, int count, unsigned int id, const char *traceFile) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "ID: %u, Имя: %s %s, Группа: %s, Средняя оценка: %.2f", 
                    students[i].id, students[i].name, students[i].surname, students[i].group,
                    calculate_average(students[i].grades, 5));
            write_trace_to_file(traceFile, buffer);
            return 1;
        }
    }
    printf("Студент с ID %u не найден.\n", id);
    return 0;
}

// Поиск студентов по фамилии и запись в файл
void search_students_by_surname(Student *students, int count, const char *surname, const char *traceFile) {
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].surname, surname) == 0) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "ID: %u, Имя: %s %s, Группа: %s, Средняя оценка: %.2f", 
                    students[i].id, students[i].name, students[i].surname, students[i].group,
                    calculate_average(students[i].grades, 5));
            write_trace_to_file(traceFile, buffer);
        }
    }
}

// Поиск студентов по имени и запись в файл
void search_students_by_name(Student *students, int count, const char *name, const char *traceFile) {
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name) == 0) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "ID: %u, Имя: %s %s, Группа: %s, Средняя оценка: %.2f", 
                    students[i].id, students[i].name, students[i].surname, students[i].group,
                    calculate_average(students[i].grades, 5));
            write_trace_to_file(traceFile, buffer);
        }
    }
}

// Поиск студентов по группе и запись в файл
void search_students_by_group(Student *students, int count, const char *group, const char *traceFile) {
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].group, group) == 0) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "ID: %u, Имя: %s %s, Группа: %s, Средняя оценка: %.2f", 
                    students[i].id, students[i].name, students[i].surname, students[i].group,
                    calculate_average(students[i].grades, 5));
            write_trace_to_file(traceFile, buffer);
        }
    }
}

// Вывод студентов с оценками выше среднего
void find_students_above_average(Student *students, int count) {
    double totalAverage = 0.0;
    for (int i = 0; i < count; i++) {
        totalAverage += calculate_average(students[i].grades, 5);
    }
    totalAverage /= count;

    printf("Студенты, чей средний балл выше общего среднего (%.2f):\n", totalAverage);
    for (int i = 0; i < count; i++) {
        double studentAverage = calculate_average(students[i].grades, 5);
        if (studentAverage > totalAverage) {
            print_student_with_average(&students[i]);
        }
    }
}

// Освобождение памяти
void freeStudents(Student *students, int count) {
    for (int i = 0; i < count; i++) {
        free(students[i].grades);
    }
    free(students);
}

// Основная функция
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <путь к файлу с данными студентов> <путь к журналу>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int count;
    Student *students = loadStudents(argv[1], &count);
    if (!students) return EXIT_FAILURE;

    int choice;
    unsigned int queryId;
    char buffer[50];

    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Поиск по ID
                printf("Введите ID студента: ");
                scanf("%u", &queryId);
                search_student_by_ID(students, count, queryId, argv[2]);
                break;
            case 2: // Поиск по фамилии
                printf("Введите фамилию студента: ");
                scanf("%s", buffer);
                search_students_by_surname(students, count, buffer, argv[2]);
                break;
            case 3: // Поиск по имени
                printf("Введите имя студента: ");
                scanf("%s", buffer);
                search_students_by_name(students, count, buffer, argv[2]);
                break;
            case 4: // Поиск по группе
                printf("Введите группу студента: ");
                scanf("%s", buffer);
                search_students_by_group(students, count, buffer, argv[2]);
                break;
            case 5: // Сортировка по ID
                qsort(students, count, sizeof(Student), compare_by_ID);
                for (int i = 0; i < count; i++) {
                    print_student_simple(&students[i]);
                }
                break;
            case 6: // Сортировка по фамилии
                qsort(students, count, sizeof(Student), compare_by_surname);
                for (int i = 0; i < count; i++) {
                    print_student_simple(&students[i]);
                }
                break;
            case 7: // Сортировка по имени
                qsort(students, count, sizeof(Student), compare_by_name);
                for (int i = 0; i < count; i++) {
                    print_student_simple(&students[i]);
                }
                break;
            case 8: // Сортировка по группе
                qsort(students, count, sizeof(Student), compare_by_group);
                for (int i = 0; i < count; i++) {
                    print_student_simple(&students[i]);
                }
                break;
            case 9: // Студенты с оценками выше среднего
                find_students_above_average(students, count);
                break;
            case 0: // Выход
                freeStudents(students, count);
                return EXIT_SUCCESS;
            default:
                printf("Неверная опция. Пожалуйста, выберите из меню.\n");
        }
    }

    freeStudents(students, count);
    return EXIT_SUCCESS;
}


// Функция для отображения меню
void display_menu() {
    printf("\nВыберите опцию:\n");
    printf("1. Поиск по ID\n");
    printf("2. Поиск по фамилии\n");
    printf("3. Поиск по имени\n");
    printf("4. Поиск по группе\n");
    printf("5. Сортировка по ID\n");
    printf("6. Сортировка по фамилии\n");
    printf("7. Сортировка по имени\n");
    printf("8. Сортировка по группе\n");
    printf("9. Студенты с оценками выше среднего\n");
    printf("0. Выход\n");
    printf("Ваш выбор: ");
}
