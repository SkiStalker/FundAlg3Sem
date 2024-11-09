#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID 0
#define NAME 1
#define SURNAME 2
#define GROUP 3
#define ESTIMATES 4

enum {
    MALLOC_FAILED,
    TO_MANY_SYMBOLS_IN_ID,
    TO_MANY_SYMBOLS_IN_ESTIMATES,
    TO_MANY_SYMBOLS_IN_NAME,
    TO_MANY_SYMBOLS_IN_SURNAME,
    TO_MANY_SYMBOLS_IN_GROUP,
    ID_INVALID,
    NAME_INVALID,
    Wrong_data_of_students,
    SUCCESS,
    transformation_student_to_str,
    writing_to_a_file,
    ERROR_NOT_PATH
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case MALLOC_FAILED: printf("error: malloc failed\n"); break;
        case TO_MANY_SYMBOLS_IN_ID: printf("error: TO MANY SYMBOLS IN ID\n"); break;
        case TO_MANY_SYMBOLS_IN_ESTIMATES: printf("error: TO MANY SYMBOLS IN ESTIMATES\n"); break;
        case TO_MANY_SYMBOLS_IN_NAME: printf("error: TO MANY SYMBOLS IN NAME\n"); break;
        case TO_MANY_SYMBOLS_IN_SURNAME: printf("error: TO MANY SYMBOLS IN SURNAME\n"); break;
        case TO_MANY_SYMBOLS_IN_GROUP: printf("error: TO MANY SYMBOLS IN GROUP\n"); break;
        case ID_INVALID: printf("error: ID or estimates is INVALID\n"); break;
        case NAME_INVALID: printf("error: NAME OR SURNAME IS INVALID\n"); break;
        case Wrong_data_of_students: printf("error: Wrong number of arguments\n"); break;
        case SUCCESS: break;
        case transformation_student_to_str: printf("error: transformation student to str\n"); break;
        case writing_to_a_file: printf("error: writing to a file\n"); break;
        case ERROR_NOT_PATH: printf("error: NOT PATH\n"); break;
        default: printf("error: Unknown error code\n"); break;
    }
}

struct Student {
    unsigned int id;
    char group[100], surname[100], name[100];
    unsigned char* estimates;
};

struct founded_students_and_size {
    struct Student* founded_students;
    int size;
};

void Clear(struct Student* Students, int ind_student, FILE* fi) {
    for (int i = 0; i <= ind_student; i++) {
        free(Students[i].estimates);
    }
    free(Students);
    fclose(fi);
}

int ValidateID(char ch) {
    if (!isdigit(ch)) {
        return ID_INVALID;
    }
    return SUCCESS;
}

int ValidateName(char ch) {
    if (!isalpha(ch)) {
        return NAME_INVALID;
    }
    return SUCCESS;
}

int Validatelen(int len, int tek_fl) {
    if  (len > 9 && tek_fl == 0) {
        return TO_MANY_SYMBOLS_IN_ID;
    }
    if (len > 99 && tek_fl == 1) {
        return TO_MANY_SYMBOLS_IN_NAME;
    }
    if (len > 99 && tek_fl == 2) {
        return TO_MANY_SYMBOLS_IN_SURNAME;
    }
    if (len > 99 && tek_fl == 3) {
        return TO_MANY_SYMBOLS_IN_GROUP;
    }
    if (len > 1 && tek_fl > 3) {
        return TO_MANY_SYMBOLS_IN_ESTIMATES;
    }
    return SUCCESS;
}

int Add_el_to_student(int tek_fl, struct Student* Students, int ind_student, char tek_el[100]) {
    if (tek_fl == ID) {
        int tek_id = atoi(tek_el);
        Students[ind_student].id = tek_id;
    }
    if (tek_fl == NAME) {
        for (int i = 0 ; i < 100; i++) {
            Students[ind_student].name[i] = tek_el[i];
        }
    }
    if (tek_fl == SURNAME) {
        for (int i = 0 ; i < 100; i++) {
            Students[ind_student].surname[i] = tek_el[i];
        }
    }
    if (tek_fl == GROUP) {
        for (int i = 0 ; i < 100; i++) {
            Students[ind_student].group[i] = tek_el[i];
        }
    }
    if (tek_fl >= ESTIMATES && tek_fl < ESTIMATES + 5) {
        Students[ind_student].estimates[tek_fl - ESTIMATES] = tek_el[0];
    }
    return SUCCESS;
}

int ValidateTekStr(const char tek_str[], struct Student* Students, int ind_student) {
    int ind = 0, tek_fl = 0;
    char ch = tek_str[ind];
    Students[ind_student].estimates = (unsigned char*)malloc(sizeof(unsigned char) * 5);
    if (Students[ind_student].estimates == NULL) {
        return MALLOC_FAILED;
    }
    while(ch != '\n' && ch != '\0' && ch != EOF) {
        if (tek_fl > 8) {
            return Wrong_data_of_students;
        }
        while (ch == ' ' || ch == '\t') {
            ch = tek_str[++ind];
        }
        char tek_el[100];
        int len = 0;
        while(ch != ' ' && ch != '\t' && ch != '\n' && ch != EOF && ch != '\0') {
            len++;
            int res_validate = Validatelen(len, tek_fl);
            if (res_validate != SUCCESS) {
                return res_validate;
            }
            if(tek_fl == ID || tek_fl >= ESTIMATES) {
                res_validate = ValidateID(ch);
                if (res_validate != SUCCESS) {
                    return res_validate;
                }
            }
            if(tek_fl == NAME || tek_fl == SURNAME) {
                res_validate = ValidateName(ch);
                if (res_validate != SUCCESS) {
                    return res_validate;
                }
            }
            tek_el[len - 1] = ch;
            ch = tek_str[++ind];
        }
        tek_el[len] = '\0';
        Add_el_to_student(tek_fl, Students, ind_student, tek_el);
        tek_fl++;
        while (ch == ' ' || ch == '\t') {
            ch = tek_str[++ind];
        }
    }
    return SUCCESS;
}

int StudentInFile(struct Student* founded_students, int size, FILE *fiout, char Find_by[8]) {
    for (int i = 0; i < size; i++) {
        char tek_student[500];
        if (strcmp(Find_by, "id") != 0) {
            if (snprintf(tek_student, 500, "%d %s %s %s %c %c %c %c %c\n", founded_students[i].id, founded_students[i].name, founded_students[i].surname, founded_students[i].group, founded_students[i].estimates[0],
                    founded_students[i].estimates[1], founded_students[i].estimates[2], founded_students[i].estimates[3], founded_students[i].estimates[4]) < 0) {
                return transformation_student_to_str;
            }
        }
        else {
            if (snprintf(tek_student, 500, "%d %s %s %s %f\n", founded_students[i].id, founded_students[i].name, founded_students[i].surname, founded_students[i].group,
                (founded_students[i].estimates[0] - '0' + founded_students[i].estimates[1] - '0' + founded_students[i].estimates[2] - '0' + founded_students[i].estimates[3] - '0' + founded_students[i].estimates[4] - '0') / 5.0) < 0) {
                return transformation_student_to_str;
            }
        }
        if (fputs(tek_student, fiout) < 0) {
            return writing_to_a_file;
        }
    }
    return SUCCESS;
}

int find_id(struct Student* Student, char id[100]) {
    if (Student->id == (unsigned int)atoi(id)) {
        return 1;
    }
    return 0;
}

int find_name(struct Student* Student, char name[100]) {
    if (strcmp(Student->name, name) == 0) {
        return 1;
    }
    return 0;
}

int find_surname(struct Student* Student, char surname[100]) {
    if (strcmp(Student->surname, surname) == 0) {
        return 1;
    }
    return 0;
}

int find_group(struct Student* Student, char group[100]) {
    if (strcmp(Student->group, group) == 0) {
        return 1;
    }
    return 0;
}

struct founded_students_and_size find(int number_of_students, struct Student* Students, char el[100], char fl[8]) {
    int cnt_founded_students = 1;
    struct founded_students_and_size res;
    struct Student* founded_students = malloc(sizeof(struct Student) * cnt_founded_students);
    int founded_students_ind = 0;
    if (founded_students == NULL) {
        ValidateCode(MALLOC_FAILED);
        res.founded_students = NULL;
        res.size = 0;
        return res;
    }
    for (int i = 0; i < number_of_students; i++) {
        if (founded_students_ind == cnt_founded_students) {
            cnt_founded_students *= 2;
            struct Student* new_founded_students = realloc(founded_students, sizeof(struct Student) * cnt_founded_students);
            if (new_founded_students == NULL) {
                ValidateCode(MALLOC_FAILED);
                free(founded_students);
                res.founded_students = NULL;
                res.size = 0;
                return res;
            }
            founded_students = new_founded_students;
        }
        if (strcmp(fl, "id") == 0) {
            if(find_id(&Students[i], el) == 1) {
                founded_students[founded_students_ind++] = Students[i];
            }
        }
        else if(strcmp(fl, "name") == 0) {
            if(find_name(&Students[i], el) == 1) {
                founded_students[founded_students_ind++] = Students[i];
            }
        }
        else if(strcmp(fl, "surname") == 0) {
            if(find_surname(&Students[i], el) == 1) {
                founded_students[founded_students_ind++] = Students[i];
            }
        }
        else if(strcmp(fl, "group") == 0) {
            if(find_group(&Students[i], el) == 1) {
                founded_students[founded_students_ind++] = Students[i];
            }
        }
    }
    if (founded_students_ind == 0) {
        free(founded_students);
        printf("No one has been found\n");
        res.founded_students = NULL;
        res.size = -1;
        return res;
    }
    res.founded_students = founded_students;
    res.size = founded_students_ind;
    return res;
}

int cmpForId(const void* x1, const void* x2) {
    unsigned int id_1 = ((struct Student*)x1)->id;
    unsigned int id_2 = ((struct Student*)x2)->id;
    if (id_1 < id_2) {
        return -1;
    }
    if (id_1 > id_2) {
        return 1;
    }
    return 0;
}

int cmpForName(const void* y1, const void* y2) {
    struct Student* x1 = (struct Student*)y1;
    struct Student* x2 = (struct Student*)y2;
    int res = strcmp(x1->name, x2->name);
    return res;
}

int cmpForSurname(const void* y1, const void* y2) {
    struct Student* x1 = (struct Student*)y1;
    struct Student* x2 = (struct Student*)y2;
    int res = strcmp(x1->surname, x2->surname);
    return res;
}

int cmpForGroup(const void* y1, const void* y2) {
    struct Student* x1 = (struct Student*)y1;
    struct Student* x2 = (struct Student*)y2;
    int res = strcmp(x1->group, x2->group);
    return res;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments");
        return -1;
    }
    int number_of_students = 5;
    FILE* fi = fopen(argv[1], "r");
    if (fi == NULL) {
        fclose(fi);
        printf("No such file");
        return -1;
    }
    struct Student* Students = malloc(sizeof(struct Student) * number_of_students);
    if (Students == NULL) {
        ValidateCode(MALLOC_FAILED);
        fclose(fi);
        return -1;
    }
    double AVERAGE_ESTIMATE = 0.0;
    int ind_student = 0;
    char tek_str[500];
    while(fgets(tek_str, 500, fi) != NULL) {
        int res_validate = ValidateTekStr(tek_str, Students, ind_student);
        if (res_validate != SUCCESS) {
            ValidateCode(res_validate);
            Clear(Students, ind_student, fi);
            return -1;
        }
        tek_str[499] = '\0';
        if (ind_student + 1 == number_of_students) {
            number_of_students *= 2;
            struct Student* new_students = realloc(Students, sizeof(struct Student) * number_of_students);
            if (new_students == NULL) {
                ValidateCode(MALLOC_FAILED);
                Clear(Students, ind_student, fi);
                return -1;
            }
            Students = new_students;
        }

        AVERAGE_ESTIMATE += (Students[ind_student].estimates[0] - '0' + Students[ind_student].estimates[1] - '0' + Students[ind_student].estimates[2] - '0' + Students[ind_student].estimates[3] - '0' + Students[ind_student].estimates[4] - '0') / 5.0;
        ind_student++;
    }
    number_of_students = ind_student;
    ind_student--;
    AVERAGE_ESTIMATE /= number_of_students;
    struct founded_students_and_size find_res;
    struct Student* res_students = Students;
    int size_of_founded_students = number_of_students;
    char Find_by[8];
    printf("Find by\nYour answer (id \\ name \\ surname \\ group \\ no): ");
    scanf("%s", Find_by);
    if (strcmp(Find_by, "id") == 0) {
        char id[100];
        char str[8] = "id";
        printf("print id: ");
        scanf("%9s", id);
        find_res = find(number_of_students, Students, id, str);
    }
    else if(strcmp(Find_by, "name") == 0) {
        char name[100];
        char str[8] = "name";
        printf("print name: ");
        scanf("%99s", name);
        find_res = find(number_of_students, Students, name, str);
    }
    else if(strcmp(Find_by, "surname") == 0) {
        char surname[100];
        char str[8] = "surname";
        printf("print surname: ");
        scanf("%99s", surname);
        find_res = find(number_of_students, Students, surname, str);
    }
    else if(strcmp(Find_by, "group") == 0) {
        char group[100];
        char str[8] = "group";
        printf("print group: ");
        scanf("%99s", group);
        find_res = find(number_of_students, Students, group, str);
    }
    else if(strcmp(Find_by, "no") == 0) {}
    else {
        printf("error: unknown command");
        Clear(Students, ind_student, fi);
        return -1;
    }
    if (strcmp(Find_by, "no") != 0) {
        res_students = find_res.founded_students;
        size_of_founded_students = find_res.size;
        if (res_students == NULL && size_of_founded_students == 0) {
            Clear(Students, ind_student, fi);
            return -1;
        }
    }
    char Sort_by[8];
    printf("Sort by\nYour answer (id \\ name \\ surname \\ group \\ no): ");
    scanf("%s", Sort_by);
    if (strcmp(Sort_by, "id") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), cmpForId);
    }
    else if(strcmp(Sort_by, "name") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), cmpForName);
    }
    else if(strcmp(Sort_by, "surname") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), cmpForSurname);
    }
    else if(strcmp(Sort_by, "group") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), cmpForGroup);
    }
    else if(strcmp(Sort_by, "no") == 0) {}
    else {
        printf("error: unknown command");
        Clear(Students, ind_student, fi);
        return -1;
    }
    FILE *fiout = fopen(argv[2], "w");
    if (fiout == NULL) {
        Clear(Students, ind_student, fi);
        printf("No such file");
        return -1;
    }
    int res_writing_to_a_file = StudentInFile(res_students, size_of_founded_students, fiout, Find_by);
    if (res_writing_to_a_file != SUCCESS) {
        ValidateCode(res_writing_to_a_file);
        Clear(Students, ind_student, fi);
        fclose(fiout);
        return -1;
    }
    char answer[2];
    printf("Do you need students that have average estimate above average estimate of all students\nYour answer (y \\ n): ");
    scanf("%s", answer);
    if (strcmp(answer, "y") == 0) {
        fputs("Students that have average estimate above average estimate of all students:\n", fiout);
        for(int i = 0; i < number_of_students; i++) {
            double tek_avg_estimate = (Students[i].estimates[0] - '0' + Students[i].estimates[1] - '0' + Students[i].estimates[2] - '0' + Students[i].estimates[3] - '0' + Students[i].estimates[4] - '0') / 5.0;
            if (tek_avg_estimate > AVERAGE_ESTIMATE) {
                char tek_student[202];
                snprintf(tek_student, 201, "%s %s\n", Students[i].name, Students[i].surname);
                fputs(tek_student, fiout);
            }
        }
        printf("Done");
        Clear(Students, ind_student, fi);
        fclose(fiout);
        return 0;
    }
    if (!strcmp(answer, "n")) {
        printf("Done");
        Clear(Students, ind_student, fi);
        fclose(fiout);
        return 0;
    }
    printf("error: unknown command");
    Clear(Students, ind_student, fi);
    fclose(fiout);
    return -1;

}