#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Student {
    unsigned int id;
    char name[BUFSIZ];
    char surname[BUFSIZ];
    char group[BUFSIZ];
    unsigned char* marks;
};

double cur_avg(struct Student s) {
    int sum = 0.;
    for (int i = 0; i < 5; i++)
        sum += s.marks[i];
    return sum / 5.;
}

enum ret_type_t {
    SUCCESS,
    FILE_OPENING_ERROR,
    MEM_ALLOC_ERROR,
    INCORRECT_ARGS,
    INCORRECT_NAME,
    INCORRECT_MARK,
    INCORRECT_ID,
    EMPTY_FILE
};

int is_alletter(char* s) {
    while (*s) {
        if (!isalpha(*s))
            return 0;
        s++;
    }
    return 1;
}

int compare_string(const char* str1, const char* str2) {

    while (*str1 && *str2) {
        if (*str1 > *str2)
            return 1;
        if (*str1 < *str2)
            return -1;
        str1++;
        str2++;
    }

    if (*str1 && !(*str2))
        return 1;
    if (!(*str1) && *str2)
        return -1;
    return 0;
}

int compare_surname(const void* v1, const void* v2) {
    struct Student* s1 = (struct Student*)v1;
    struct Student* s2 = (struct Student*)v2;
    return compare_string(s1->surname, s2->surname);
}

int compare_name(const void* v1, const void* v2) {
    struct Student* s1 = (struct Student*)v1;
    struct Student* s2 = (struct Student*)v2;
    return compare_string(s1->name, s2->name);
}

int compare_group(const void* v1, const void* v2) {
    struct Student* s1 = (struct Student*)v1;
    struct Student* s2 = (struct Student*)v2;
    return compare_string(s1->group, s2->group);
}

int compare_id(const void* v1, const void* v2) {
    struct Student* s1 = (struct Student*)v1;
    struct Student* s2 = (struct Student*)v2;
    return s1->id > s2->id;
}

int is_cor(int mark) {
    return mark > 1 && mark < 6;
}

int read_students(const char* ifilename, struct Student** students, int* count, int* sum_mark) {
    FILE* input_file = fopen(ifilename, "r");
    if (!input_file) {
        return FILE_OPENING_ERROR;
    }

    *sum_mark = 0;
    int id = 1, size = 0, a = 0, b = 0, c = 0, d = 0, e = 0;
    char name[BUFSIZ], surname[BUFSIZ], group[BUFSIZ];

    while (fscanf(input_file, "%d %s %s %s %d %d %d %d %d", &id, name, surname, group, &a, &b, &c, &d, &e) == 9) {
        struct Student* new_students = (struct Student*)realloc(*students, (size + 1) * sizeof(struct Student));
        if (!new_students) {
            free(students);
            return MEM_ALLOC_ERROR;
        }
        if (id < 0) {
            free(students);
            free(new_students);
            return INCORRECT_ID;
        }
        if (!(is_cor(a) && is_cor(b) && is_cor(c) && is_cor(d) && is_cor(e))) {
            free(students);
            free(new_students);
            return INCORRECT_MARK;
        }
        if (!is_alletter(name) || !is_alletter(surname)) {
            free(students);
            free(new_students);
            return INCORRECT_NAME;
        }
        unsigned char* marks = (unsigned char*)malloc(5 * sizeof(unsigned char));
        if (!marks) {
            free(students);
            free(new_students);
            return MEM_ALLOC_ERROR;
        }
        *sum_mark += a + b + c + d + e;
        marks[0] = a, marks[1] = b, marks[2] = c, marks[3] = d, marks[4] = e;
        *students = new_students;
        (*students)[size].id = id;
        strcpy((*students)[size].name, name);
        strcpy((*students)[size].surname, surname);
        strcpy((*students)[size].group, group);
        (*students)[size].marks = marks;
        size++;
    }

    *count = size;
    fclose(input_file);
    return SUCCESS;
}

enum ret_type_t write_students(char* ofile, struct Student* students, int count) {
    FILE* output_file = fopen(ofile, "w");
    if (!output_file)
        return FILE_OPENING_ERROR;

    for (int i = 0; i < count; i++) {
        fprintf(output_file, "%u %s %s %s ",
            students[i].id,
            students[i].name,
            students[i].surname,
            students[i].group);
        for (int j = 0; j < 5; j++)
            fprintf(output_file, "%u ", students[i].marks[j]);
        fprintf(output_file, "\n");
    }
    fclose(output_file);
    return SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments");
        return INCORRECT_ARGS;
    }

    struct Student* students = NULL;
    int count = 0, sum_mark = 0;
    int res = read_students(argv[1], &students, &count, &sum_mark);

    if (!students) {
        printf("File is empty");
        return EMPTY_FILE;
    }

    if (res == 1) {
        printf("File %s opening error", argv[1]);
        return FILE_OPENING_ERROR;
    }

    if (res == 2) {
        printf("Memory allocation error");
        return MEM_ALLOC_ERROR;
    }

    if (res == 4) {
        printf("Name and surname must conatain only letters");
        return INCORRECT_NAME;
    }

    if (res == 5) {
        printf("Marks must be from 2 to 5");
        return INCORRECT_MARK;
    }

    if (res == 6) {
        printf("Id must be biggger 0");
        return INCORRECT_ID;
    }
    double avg = (double)sum_mark / count / 5;
    int com;
    char str[BUFSIZ];
    printf("Data was successfully read. To sort input 1, to find - 2, to write students average mark with upper-middle average mark - 3:\n");
    scanf("%d", &com);

    if (com < 1 || com  > 3) {
        printf("Incorrect command");
        free(students);
        return INCORRECT_ARGS;
    }

    if (com == 1) {

        printf("Input parameter\n");
        scanf("%s", str);
        if (!strcmp(str, "id"))
            qsort(students, count, sizeof(struct Student), compare_id);
        else if (!strcmp(str, "name"))
            qsort(students, count, sizeof(struct Student), compare_surname);
        else if (!strcmp(str, "surname"))
            qsort(students, count, sizeof(struct Student), compare_name);
        else if (!strcmp(str, "group"))
            qsort(students, count, sizeof(struct Student), compare_group);
        else{
            printf("Incorrect parameter");
            return INCORRECT_ARGS;
        }

        res = write_students(argv[2], students, count);
        free(students);

        if (!res){
            printf("Program finished successfully, result was written in %s", argv[2]);
            return SUCCESS;
        }
        else{
            printf("File %s opening error", argv[2]);
            return FILE_OPENING_ERROR;
        }

    }

    if (com == 2) {
        
        int param, flag = 0;
        printf("Input parameter\n");
        scanf("%s", str);

        if (!strcmp(str, "id")) {
            FILE* ofile = fopen(argv[2], "w");
            if (!ofile) {
                printf("File opening error");
                return FILE_OPENING_ERROR;
            }
            int val;
            printf("Input value\n");
            scanf("%d", &val);
            for (int i = 0; i < count; i++) {
                struct Student cur = students[i];
                if (val == (int)cur.id) {
                    fprintf(ofile, "%s %s %s %lf", cur.name, cur.surname, cur.group, cur_avg(cur));
                    fclose(ofile);
                    free(students);
                    printf("Program finished successfully, result was written in %s", argv[2]);
                    return SUCCESS;
                }
            }
            fprintf(ofile, "No such student");
            fclose(ofile);
            free(students);
            printf("Program finished successfully, result was written in %s", argv[2]);
            return SUCCESS;
        }
        else if (!strcmp(str, "name"))
            param = 1;
        else if (!strcmp(str, "surname"))
            param = 2;
        else if (!strcmp(str, "group"))
            param = 3;
        else {
            printf("Incorrect parameter");
            return INCORRECT_ARGS;
        }
        flag = 0;
        printf("Input value\n");
        scanf("%s", str);
        for (int i = 0; i < count; i++) {
            struct Student cur = students[i];
            if ((param == 1 && !strcmp(str, cur.name)) || (param == 2 && !strcmp(str, cur.surname)) || (param == 3 && !strcmp(str, cur.group))) {
                printf("%d %s %s %s %lf", cur.id, cur.name, cur.surname, cur.group, cur_avg(cur));
                flag = 1;
            }
        }
        if (!flag)
            printf("No such student");
        free(students);
        return SUCCESS;
    }

    if (com == 3) {
        FILE* output_file = fopen(argv[2], "w");
        if (!output_file)
            return FILE_OPENING_ERROR;
        for (int i = 0; i < count; i++) {
            if (cur_avg(students[i]) > avg) {
                fprintf(output_file, "%s %s\n", students[i].name, students[i].surname);
            }
        }
        free(students);
        fclose(output_file);
        printf("Program finished successfully, result was written in %s", argv[2]);
        return SUCCESS;
    }

}