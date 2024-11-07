#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define EPS 0.00000001

enum Erros {
    ERROR = -1,
    SUCCESS,
};

typedef struct Student {
    unsigned int id;
    char* name;
    char* surname;
    char* group;
    unsigned char* marks;
} Student;

typedef struct Students {
    Student* students;
    int capacity;
    int size;
} Students;


int char_to_int(char ch) {
    if (isdigit(ch)) {
        return (ch - '0');
    } else {
        return ERROR;
    }
    
}

int ValidateString(char* str) {
    if (strlen(str) == 0) {
        return 0;
    } else {
        while (*str != '\0') {
            if (!isalpha(*str++)) {
                return 0;
            }
        }
    }
    return 1;
}

int ValidateInput(Student* s) {
    if (ValidateString(s->surname) && ValidateString(s->name) && (strlen(s->group) > 0) && (strlen((char*)s->marks) == 5)) {
        return 1;
    } else return 0;
}

int CompareId(const void* a, const void *b) {
    Student* s1 = (Student*)a;
    Student* s2 = (Student*)b;;
    return s1->id - s2->id;
}

int CompareName(const void* a, const void *b) {
    Student* s1 = (Student*)a;
    Student* s2 = (Student*)b;;
    return strcmp(s1->name, s2->name);
}

int CompareSurname(const void* a, const void *b) {
    Student* s1 = (Student*)a;
    Student* s2 = (Student*)b;;
    return strcmp(s1->surname, s2->surname);
}

int CompareGroup(const void* a, const void *b) {
    Student* s1 = (Student*)a;
    Student* s2 = (Student*)b;;
    return strcmp(s1->group, s2->group);
}

int GetSumMarks(unsigned char* marks) {
    return marks[0] + marks[1] + marks[2] + marks[3] + marks[4];
}

Students* CreateStudents() {
    Students* students = (Students*)malloc(sizeof(Students));
    if (students == NULL) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    Student* student = (Student*)malloc(10 * sizeof(Student));
    if (students == NULL) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    students->students = student;
    students->capacity = 10;
    students->size = 0;
    return students;
}

int ProccessFile(const char* f, Students* s) {
    FILE* file = fopen(f, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        free(s);
        return ERROR;
    }

    while (!feof(file)) {
        if (s->size >= s->capacity) {
            s->capacity *= 2;
            s->students = (Student*)realloc(s->students, s->capacity * sizeof(Student));
            if (s == NULL) {
                printf("Ошибка перераспределения памяти\n");
                fclose(file);
                return ERROR;
            }
        }

        s->students[s->size].id = s->size + 1;
        s->students[s->size].name = (char*)malloc(50 * sizeof(char));
        s->students[s->size].surname = (char*)malloc(50 * sizeof(char));
        s->students[s->size].group = (char*)malloc(50 * sizeof(char));
        s->students[s->size].marks = (unsigned char*)malloc(5 * sizeof(unsigned char));

        if (fscanf(file, "%49s %49s %49s", s->students[s->size].name, s->students[s->size].surname, s->students[s->size].group) != 3) {
            printf("Ошибка чтения данных студента\n");
            return ERROR;
        }



        for (int i = 0; i < 5; i++) {
            if (fscanf(file, "%hhu", &s->students[s->size].marks[i]) != 1) { 
                printf("Ошибка чтения оценок студента\n");
                return ERROR;
            }
        }
        if (!ValidateInput(&s->students[s->size])) {
            printf("Ошибка чтения данных студента %ls\n", &s->students[s->size].id);
            free(s->students[s->size].name);
            free(s->students[s->size].surname);
            free(s->students[s->size].group);
            free(s->students[s->size].marks);
            return ERROR;
        }

        s->size++;
    }
    
    fclose(file);
    return SUCCESS;
}

int PrintStudent(Student* s) {
    int id = s->id;
    char* name = s->name;
    char* surname = s->surname;
    char* group = s->group;
    unsigned char mark1 = s->marks[0];
    unsigned char mark2 = s->marks[1];
    unsigned char mark3 = s->marks[2];
    unsigned char mark4 = s->marks[3];
    unsigned char mark5 = s->marks[4];
    printf("Студент %d: %s %s, Группа: %s, Оценки: %d %d %d %d %d\n", id, name, surname, group, mark1, mark2, mark3, mark4, mark5);
    return SUCCESS;
}

int FilePrintStudent(Student* s, FILE* f) {
    char* name = s->name;
    char* surname = s->surname;
    char* group = s->group;
    float mark = GetSumMarks(s->marks) / 5.0;
    fprintf(f, "%s %s, Группа: %s, Средняя оценка: %.2f\n", name, surname, group, mark);
    return SUCCESS;
}

int SearchStudent(Students* s, char* criteria, char* value) {
    Student* m = (Student*)malloc(s->size * sizeof(Student));
    int n = 0;
    for (int i = 0; i < s->size; i++) {
        Student student = s->students[i];
        if (!strcmp(criteria, "id")) {
            if((int)student.id == char_to_int(value[0])) {
                m[n++] = student;
            }
        } else if (!strcmp(criteria, "name")) {
            if(!strcmp(student.name, value)) {
                m[n++] = student;
            }
        } else if (!strcmp(criteria, "surname")) {
            if(!strcmp(student.surname, value)) {
                m[n++] = student;
            }
        } else if (!strcmp(criteria, "group")) {
            if(!strcmp(student.group, value)) {
                printf("%s\n", value);
                m[n++] = student;
            }
        } else {
            return ERROR;
        }
    }
    if (n == 0) {
        printf("Студент с таким %s не найден\n", value);
    }
    for (int i = 0; i < n; i++) {
        PrintStudent(&m[i]);
    }
    free(m);
    return SUCCESS;
}


int FileSearchStudent(Students* s, char* value) {
    FILE* trace = fopen("student.txt", "w");
    if (trace == NULL) {
        printf("Ошибка открытия файла\n");
        free(s);
        return ERROR;
    }
    Student* m = (Student*)malloc(s->size * sizeof(Student));
    int n = 0;
    for (int i = 0; i < s->size; i++) {
        Student student = s->students[i];
        if((int)student.id == char_to_int(value[0])) {
            m[n++] = student;
        }
    }
    if (n == 0) {
        fprintf(trace, "Студент с таким id не найден\n");
    }
    for (int i = 0; i < n; i++) {
        FilePrintStudent(&m[i], trace);
    }
    free(m);
    fclose(trace);
    return SUCCESS;
}

int SortStudents(Students* s, char* criteria) {
    if (!strcmp(criteria, "id")) {
        qsort(s->students, s->size, sizeof(Student), CompareId);
    } else if (!strcmp(criteria, "name")) {
        qsort(s->students, s->size, sizeof(Student), CompareName);
    } else if (!strcmp(criteria, "surname")) {
        qsort(s->students, s->size, sizeof(Student), CompareSurname);
    } else if (!strcmp(criteria, "group")) {
        qsort(s->students, s->size, sizeof(Student), CompareGroup);
    } else {
        return ERROR;
    }
    return SUCCESS;
    
}

int PrintStudents(Students* s) {
    for (int i = 0; i < s->size; i++) {
        PrintStudent(&s->students[i]);
    }
    return SUCCESS;
}



float GetMidMarkAll(Students* s) {
    int count = s->size * 5;
    int sum = 0;
    for (int i = 0; i < s->size; i++) {
        
        sum += GetSumMarks(s->students[i].marks);
    }

    return (float)sum / count;
}

int PrintBetterStudent(Students* s) {
    float midMark = GetMidMarkAll(s);
    FILE* trace = fopen("better_student.txt", "w");
    if (trace == NULL) {
        printf("Ошибка открытия файла\n");
        free(s);
        return ERROR;
    }
    Student* m = (Student*)malloc(s->size * sizeof(Student));
    int n = 0;
    for (int i = 0; i < s->size; i++) {
        Student student = s->students[i];
        float studentMidMark = GetSumMarks(student.marks) / 5.0;
        if ((studentMidMark - midMark) > EPS) {
            m[n++] = student;
        }
    }

    fprintf(trace, "Средняя оценка по всех студентам: %.2f\n", midMark);
    for (int i = 0; i < n; i++) {
        FilePrintStudent(&m[i], trace);
    }
    free(m);
    fclose(trace);
    return SUCCESS;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Укажите имя файла как аргумент программы\n");
        return ERROR;
    }
    Students* students = CreateStudents();
    int res = ProccessFile(argv[1], students);
    if (res == ERROR) {
        return ERROR;
    }
    


    char search[15] = "search";
    char fsearch[15] = "fsearch";
    char stop[15] = "stop";
    char sort[15] = "sort";
    char print[15] = "print";
    char better[15] = "better";

    printf("----------------------------------\n");
    printf("print - список всех студентов\n");
    printf("search <mode> - поиск студента по критерию\n");
    printf("   |mode = id; name; surname; group\n");
    printf("sort <mode> - сортировка студентов по критерию\n");
    printf("   |mode = id; name; surname; group\n");
    printf("fsearch - поиск студента по id и вывод данных в файл\n");
    printf("better - вывод всех студентов в файл, чей средний балл больше общего\n");
    printf("stop - завершить программу\n");
    printf("----------------------------------\n");

    while (1) {
        
        char command[20];
        scanf("%s", command);

        if (!strcmp(command, search)) {
            char mode[20], value[50];
            scanf("%s %s", mode, value);
            int res = SearchStudent(students, mode, value);
            if (res == ERROR) {
                printf("Неверный критерий поиска\n");
                return ERROR;
            }
            
        } else if (!strcmp(command, fsearch)) {
            char value[30];
            scanf("%s", value);
            int res = FileSearchStudent(students, value);
            if (res == ERROR) {
                printf("Неверный критерий поиска\n");
                return ERROR;
            }
        } else if (!strcmp(command, better)) {
            int res = PrintBetterStudent(students);
            if (res == ERROR) {
                printf("Ошибка в поиске лучших студентов\n");
                return ERROR;
            }
        }else if (!strcmp(command, sort)) {
            char mode[20];
            scanf("%s", mode);
            int res = SortStudents(students, mode);
            if (res == ERROR) {
                printf("Неверный критерий сортировки\n");
                return ERROR;
            }
        } else if (!strcmp(command, print)) {
            PrintStudents(students);
        } else if (!strcmp(command, stop)) {
            for (int i = 0; i < students->size; i++) {
                    free(students->students[i].name);
                    free(students->students[i].surname);
                    free(students->students[i].group);
                }
                free(students);
            break;
        } else {
            printf("Неизвестная команда. Пожалуйста, попробуйте снова.\n");
            return ERROR;
        }
    }


    
    
    return SUCCESS;
}
