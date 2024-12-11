#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum {
    SUCCESS,
    ERROR_WRONG_NUMB_OF_ARGS,
    NOT_A_STR,
    NULL_NUM,
    NOT_A_NUM,
    NUM_TOO_LONG,
    NEGATIVE_NUM,
    NOT_PATH,
    FILE_OPENING_ERROR,
    ERROR_SAME_IN_AND_OUT_FILES,
    MEMORY_ALLOCATION_ERROR,
    FILE_ENDED
} CODES;

void ValidateCode(int code) {
    switch (code) {
        case NEGATIVE_NUM: printf("error: Negative value was entered\n"); break;
        case NULL_NUM: printf("error: Not number was entered\n"); break;
        case NOT_A_NUM: printf("error: Value is not a number\n"); break;
        case NUM_TOO_LONG: printf("error: Too long string was entered\n"); break;
        case FILE_OPENING_ERROR: printf("error: File opening error\n"); break;
        case NOT_PATH: printf("error: Value is not a path\n"); break;
        case ERROR_SAME_IN_AND_OUT_FILES: printf("error: Input and output files can not be equal\n"); break;
        case MEMORY_ALLOCATION_ERROR: printf("error: Memory allocation error\n"); break;
        case NOT_A_STR: printf("error: String can not be name or surname\n"); break;
        default: printf("Unknown error\n"); break;
    }
}

typedef int(*callback)(char*);

struct Employee {
    unsigned int id;
    char* name;
    char* surname;
    float salary;
    char* str_salary;
};

int is_number(const char* s) {
    if (*s == '\0') return NULL_NUM;
    while (*s == ' ') s++;
    if (*s == '-') {
        return NEGATIVE_NUM;
    }
    int lenght = 0;
    while (isdigit(*s)) {
        s++;
        if (lenght++ > 18) return NUM_TOO_LONG;
    }
    if (*s == '\0') return SUCCESS;
    return NOT_A_NUM;
}

int is_float(const char* s) {
    if (*s == '\0') return NULL_NUM;

    while (*s == ' ') s++;
    if (*s == '-') {
        return NEGATIVE_NUM;
    }
    int lenght = 0;
    int point_was = 0;
    while (isdigit(*s) || (*s == '.' && !point_was)) {
        if (*s == '.')
            point_was += 1;
        s++;
        if (lenght++ > 20)
            return NUM_TOO_LONG;
    }

    if (*s == '\0') return SUCCESS;
    return NOT_A_NUM;
}

int is_str(const char* s) {
    while (*s) {
        if (!isalpha(*s)) {
            return NOT_A_STR;
        }
        s++;
    }
    return SUCCESS;
}

unsigned int StrToUnsignedint(const char* s) {
    while (*s == ' ') s++;
    unsigned int res = 0;
    while (*s != '\0') {
        res = (res * 10) + (*s - '0');
        s++;
    }
    return res;
}

int newEmployee(FILE *infile, struct Employee* one_Employee) {
    char tekSym = '\0';
    char *data = malloc(sizeof(char));
    int data_index = 0;
    int data_type = 0;
    while (tekSym != EOF && tekSym != '\n') {
        tekSym = (char)fgetc(infile);
        if (tekSym == EOF && data_type != 3) {
            return FILE_ENDED;
        }
        if (tekSym != '\n' && tekSym != ' ' && tekSym != EOF) {
            data[data_index] = tekSym;
            data_index++;
            if (data_index >= (int)strlen(data)) {
                char *ptr = realloc(data, 2 * (int)sizeof(data) * sizeof(char));
                if (ptr == NULL) {
                    free(data);
                    return MEMORY_ALLOCATION_ERROR;
                }
                data = ptr;
            }
        } else {
            if (!data_index) {
                continue;
            }
            data[data_index] = '\0';
            int code;
            switch (data_type) {
                case 0:
                    if ((code = is_number(data))) {
                        free(data);
                        return code;
                    }
                    one_Employee->id = StrToUnsignedint(data);
                    break;
                case 1:
                    if ((code = is_str(data)) != SUCCESS) {
                        free(data);
                        return code;
                    }
                    one_Employee->name = malloc(sizeof(char) * strlen(data));
                    strcpy(one_Employee->name, data);
                    break;
                case 2:
                    if ((code = is_str(data)) != SUCCESS) {
                        free(data);
                        return code;
                    }
                    one_Employee->surname = malloc(sizeof(char) * strlen(data));
                    strcpy(one_Employee->surname, data);
                    break;
                case 3:
                    if ((code = is_float(data))) {
                        free(data);
                        return code;
                    }
                    one_Employee->salary = atof(data);
                    one_Employee->str_salary = malloc(sizeof(char) * strlen(data));
                    strcpy(one_Employee->str_salary, data);
                    break;
                default:
                    printf("Something went wrong!\n");
                    break;
            }
            data_type++;
            data_index = 0;
        }
    }
    free(data);
    if (data_type != 4) {
        printf("Wrong number of arguments.");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }
    return SUCCESS;
}

int comporatorD(const void* y1, const void* y2) {
    struct Employee* x1 = (struct Employee*)y1;
    struct Employee* x2 = (struct Employee*)y2;
    int res;
    if (strcmp(x1->str_salary, x2->str_salary) < 0) {
        return 1;
    }
    if (strcmp(x1->str_salary, x2->str_salary) > 0) {
        return -1;
    }
    if ((res = strcmp(x1->surname, x2->surname)) != 0) {
        return res;
    }
    if ((res = strcmp(x1->name, x2->name)) != 0) {
        return res;
    }
    if (x1->id < x2->id) {
        return 1;
    }
    if (x1->id > x2->id) {
        return -1;
    }
    return 0;
}

int comporatorA(const void* y1, const void* y2) {
    struct Employee* x1 = (struct Employee*)y1;
    struct Employee* x2 = (struct Employee*)y2;
    int res;
    if (strcmp(x1->str_salary, x2->str_salary) < 0) {
        return -1;
    }
    if (strcmp(x1->str_salary, x2->str_salary) > 0) {
        return 1;
    }
    if ((res = strcmp(x1->surname, x2->surname)) != 0) {
        return res;
    }
    if ((res = strcmp(x1->name, x2->name)) != 0) {
        return res;
    }
    if (x1->id < x2->id) {
        return -1;
    }
    if (x1->id > x2->id) {
        return 1;
    }
    return 0;
}

int findexFlag(const char* fl, const char** flags, const int size) {
    for (int i = 0; i < size; ++i) {
        if (!strcmp(fl, flags[i])) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        return -1;
    }
    if (!strcmp(argv[1], argv[3])) {
        ValidateCode(ERROR_SAME_IN_AND_OUT_FILES);
        return -1;
    }
    FILE *infile = fopen(argv[1], "r");
    FILE *fiout = fopen(argv[3], "w");
    if (infile == NULL || fiout == NULL) {
        ValidateCode(FILE_OPENING_ERROR);
        return -1;
    }
    struct Employee *one_Employee = malloc(sizeof(struct Employee));
    int data_size = 1;
    struct Employee prom_data;
    int index = 0, code;
    while ((code = newEmployee(infile, &prom_data)) != FILE_ENDED) {
        if (code) {
            for (int i = 0; i < index; ++i) {
                free(one_Employee[i].name);
                free(one_Employee[i].surname);
                free(one_Employee[i].str_salary);
            }
            free(one_Employee);
            ValidateCode(code);
            return -1;
        }
        if (index >= data_size) {
            data_size *= 2;
            struct Employee *ptr;
            ptr = (struct Employee *) realloc(one_Employee, data_size * sizeof(struct Employee));
            if (ptr == NULL) {
                for (int i = 0; i < index; ++i) {
                    free(one_Employee[i].name);
                    free(one_Employee[i].surname);
                    free(one_Employee[i].str_salary);
                }
                free(one_Employee);
                ValidateCode(MEMORY_ALLOCATION_ERROR);
                return -1;
            }
            one_Employee = ptr;
        }
        one_Employee[index] = prom_data;
        ++index;
    }
    const char *flags[] = { "-a", "/a", "-d", "/d"};
    int ret = findexFlag(argv[2], flags, sizeof(flags) / sizeof(char*));
    if (ret == -1) {
        printf("Flag <%s> is not supported\n", argv[2]);
        return -1;
    }
    if (ret < 2) {
        qsort(one_Employee, index, sizeof(struct Employee), comporatorA);
    } else {
        qsort(one_Employee, index, sizeof(struct Employee), comporatorD);
    }
    for (int i = 0; i < index; ++i) {
        fprintf(fiout, "| Id: %-3u| Name: %-10s| Surname: %-10s| Salary: %-7.3f|\n", one_Employee[i].id, one_Employee[i].name, one_Employee[i].surname, one_Employee[i].salary);
    }
    //printf("Answer in file: %s\n", argv[3]);
    fclose(infile);
    fclose(fiout);
    for (int i = 0; i < index; ++i) {
        free(one_Employee[i].name);
        free(one_Employee[i].surname);
        free(one_Employee[i].str_salary);
    }
    free(one_Employee);
    return 0;
}