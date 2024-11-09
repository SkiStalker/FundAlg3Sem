#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


enum ret_type_t {
    SUCCESS,
    ERROR_WRONG_NUMB_OF_ARGS,
    ERROR_NOT_STR,
    ERROR_NO_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
    ERROR_NEGATIVE_VALUE,
    FILE_OPENING_ERROR,
    ERROR_SAME_IN_AND_OUT_FILES,
    MEMORY_ALLOCATION_ERROR,
    FILE_ENDED
};

void HandlingError(int code) {
    switch (code) {
        case ERROR_NEGATIVE_VALUE:
            printf("Negative value was entered.\n");
            break;
        case ERROR_NO_VALUE:
            printf("Not number was entered.\n");
            break;
        case ERROR_NOT_NUMBER:
            printf("Value is not a number.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case FILE_OPENING_ERROR:
            printf("File opening error.\n");
            break;
        case ERROR_SAME_IN_AND_OUT_FILES:
            printf("Input and output files can not be equal.\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error.\n");
            break;
        case ERROR_NOT_STR:
            printf("String can not be name or surname.\n");
            break;
        default:
            break;
    }
}

typedef int(*callback)(char*);

struct Employee {
    unsigned int id;
    char* name;
    char* surname;
    char* wage;
};

int is_number(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') {
        return ERROR_NEGATIVE_VALUE;
    }

    int len = 0;
    while (isdigit(*s)) {
        s++;
        if (len++ > 18) return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCESS;
    return ERROR_NOT_NUMBER;
}

int is_float(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') {
        return ERROR_NEGATIVE_VALUE;
    }

    int len = 0;
    int point_was = 0;
    while (isdigit(*s) || (*s == '.' && !point_was)) {
        if (*s == '.')
            point_was += 1;
        s++;
        if (len++ > 20)
            return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCESS;
    return ERROR_NOT_NUMBER;
}

int is_str(const char* s) {
    while (*s) {
        if (!isalpha(*s)) {
            return ERROR_NOT_STR;
        }
        s++;
    }
    return SUCCESS;
}

unsigned int Atou(const char* s) {
    while (*s == ' ') s++;
    unsigned int res = 0;
    while (*s != '\0') {
        res = (res * 10) + (*s - '0');
        s++;
    }
    return res;
}

int newEmployee(FILE *input, struct Employee* data) {
    char cur = '\0';
    char *lexeme = (char*) malloc(sizeof(char));
    int ind_in_lexeme = 0;
    int ind_of_data = 0;
    while (cur != EOF && cur != '\n') {
//        printf("HUI\n");
        cur = (char)fgetc(input);
        if (cur == EOF && ind_of_data != 3) {
            return FILE_ENDED;
        }
        if (cur != '\n' && cur != ' ' && cur != EOF) {
//            printf("%c ", cur);
            lexeme[ind_in_lexeme++] = cur;

            if (ind_in_lexeme >= (int)sizeof(lexeme)) {
                char *ptr;
                ptr = (char*) realloc(lexeme, 2 * (sizeof(lexeme) + 1) * sizeof(char));
                if (ptr == NULL) {
                    free(lexeme);
                    return MEMORY_ALLOCATION_ERROR;
                }
                lexeme = ptr;
            }
        } else {
            if (!ind_in_lexeme) {
                continue;
            }
            lexeme[ind_in_lexeme] = '\0';
//            printf("|%s|\n", lexeme);
            int code;
//            printf("%d\n", ind_of_data);
            switch (ind_of_data) {
                case 0:
                    if ((code = is_number(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    data->id = Atou(lexeme);
                    break;
                case 1:
                    if ((code = is_str(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    data->name = (char*) malloc(sizeof(char) * strlen(lexeme));
                    strcpy(data->name, lexeme);
                    break;
                case 2:
                    if ((code = is_str(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    data->surname = (char*) malloc(sizeof(char) * strlen(lexeme));
                    strcpy(data->surname, lexeme);
                    break;
                case 3:
                    if ((code = is_float(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    data->wage = (char*) malloc(sizeof(char) * strlen(lexeme));
                    strcpy(data->wage, lexeme);
                    break;
                default:
                    printf("Something went wrong!\n");
                    break;
            }
            ind_of_data++;
            ind_in_lexeme = 0;
        }
    }
    free(lexeme);
    if (ind_of_data != 4) {
        printf("Wrong number of arguments.");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }
    return SUCCESS;
}

int wageCmp(char *str1, char *str2) {
    char* ptr1 = str1;
    char* ptr2 = str2;
    int len1 = 0, len2 = 0;
    while (*ptr1 != '.') {
        ptr1++;
        len1++;
    }

    while (*ptr2 != '.') {
        ptr2++;
        len2++;
    }
    if (len1 != len2) {
        return (len1 > len2)? 1 : -1;
    }

    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str1 > *str2;
        }
        str1++;
        str2++;
    }
    if (*str1 != '\0') {
        return 1;
    }
    if (*str2 != '\0') {
        return -1;
    }
    return 0;
}

int cmpForD(const void* y1, const void* y2) {
    struct Employee* x1 = (struct Employee*)y1;
    struct Employee* x2 = (struct Employee*)y2;
    int res;
    if (wageCmp(x1->wage, x2->wage) < 0) {
        return 1;
    } else if (wageCmp(x1->wage, x2->wage) > 0) {
        return -1;
    } else {
        if ((res = strcmp(x1->surname, x2->surname)) != 0) {
            return res;
        }

        if ((res = strcmp(x1->name, x2->name)) != 0) {
            return res;
        }
        if (x1->id < x2->id) {
            return 1;
        } else if (x1->id > x2->id) {
            return -1;
        }
        return 0;
    }
}

int cmpForA(const void* y1, const void* y2) {
    struct Employee* x1 = (struct Employee*)y1;
    struct Employee* x2 = (struct Employee*)y2;
    int res;
    if (wageCmp(x1->wage, x2->wage) < 0) {
        return -1;
    } else if (wageCmp(x1->wage, x2->wage) > 0) {
        return 1;
    } else {
        if ((res = strcmp(x1->surname, x2->surname)) != 0) {
            return res;
        }

        if ((res = strcmp(x1->name, x2->name)) != 0) {
            return res;
        }
        if (x1->id < x2->id) {
            return -1;
        } else if (x1->id > x2->id) {
            return 1;
        }
        return 0;
    }
}

int findFlag(char* curarg, const char** flags, int size) {
    for (int i = 0; i < size; ++i)
    {
        if (!(strcmp(curarg, flags[i])))
        {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments.\n");
        return -1;
    }

    if (!strcmp(argv[1], argv[3])) {
        HandlingError(ERROR_SAME_IN_AND_OUT_FILES);
        return -1;
    }

    FILE *input, *output;
    input = fopen(argv[1], "r");
    output = fopen(argv[3], "w");
    if (input == NULL || output == NULL) {
        HandlingError(FILE_OPENING_ERROR);
        return -1;
    }


    struct Employee *data = (struct Employee*) malloc(sizeof(struct Employee));
    int data_size = 1;
    struct Employee prom_data;
    int ind = 0, code;
    while ((code = newEmployee(input, &prom_data)) != FILE_ENDED) {

//        printf("| ID: %u | Name: %s | Surname: %s | Wage: %.10f |\n", prom_data.id, prom_data.name, prom_data.surname, prom_data.wage);
        if (code) {
            for (int i = 0; i < ind; ++i) {
                free(data[i].name);
                free(data[i].surname);
                free(data[i].wage);
            }
            free(data);
            HandlingError(code);
            return -1;
        }

        if (ind >= data_size) {
            data_size *= 2;
            struct Employee *ptr;
            ptr = (struct Employee *) realloc(data, data_size * sizeof(struct Employee));
            if (ptr == NULL) {
                for (int i = 0; i < ind; ++i) {
                    free(data[i].name);
                    free(data[i].surname);
                    free(data[i].wage);
                }
                free(data);
                HandlingError(MEMORY_ALLOCATION_ERROR);
                return -1;
            }
            data = ptr;
        }

        data[ind] = prom_data;
        ++ind;
    }

    const char *flags[] = { "-a", "/a", "-d", "/d"};
    int ret = findFlag(argv[2], flags, sizeof(flags) / sizeof(char*));
    if (ret == -1) {
        printf("THIS FLAG DOES NOT EXIST %s\n", argv[2]);
        return -1;
    }
    if (ret < 2) {
        qsort(data, ind, sizeof(struct Employee), cmpForA);
    } else {
        qsort(data, ind, sizeof(struct Employee), cmpForD);
    }
    for (int i = 0; i < ind; ++i) {
        fprintf(output, "| ID: %-5u | Name: %-15s | Surname: %-15s | Wage: %-20s |\n", data[i].id, data[i].name, data[i].surname, data[i].wage);
    }

    printf("Answer in file: %s\n", argv[3]);

    fclose(input);
    fclose(output);

    for (int i = 0; i < ind; ++i) {
        free(data[i].name);
        free(data[i].surname);
        free(data[i].wage);
    }
    free(data);
}