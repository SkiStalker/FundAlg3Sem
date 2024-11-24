#define _CRT_SECURE_NO_DEPRECATE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define CRT_SECURE_NO_WARNINGS

enum {
    MALLOC_FAILED,
    REALLOC_FAILED,
    FILE_IS_INVALID,
    NUMS_IN_FILE_IS_INVALID,
    NOT_A_NUMS_FILE,
    WRONG_INPUT_IN_FUNCTION,
    MASSIVE_CANNOT_BE_EMPTY,
    SIZE_OF_MASSIVE_LESS_THAN_IND,
    SUCCESS
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case MALLOC_FAILED: printf("malloc failed\n"); break;
        case REALLOC_FAILED: printf("realloc failed\n"); break;
        case FILE_IS_INVALID: printf("FILE IS INVALID\n"); break;
        case NUMS_IN_FILE_IS_INVALID: printf("NUMS IN FILE IS INVALID\n"); break;
        case NOT_A_NUMS_FILE: printf("NOT A NUMS IN FILE\n"); break;
        case WRONG_INPUT_IN_FUNCTION: printf("WRONG INPUT IN FUNCTION\n"); break;
        case MASSIVE_CANNOT_BE_EMPTY: printf("MASSIVE CAN NOT BE EMPTY\n"); break;
        case SUCCESS: break;
        default: printf("Unknown error code\n"); break;
    }
}

typedef struct massive {
    char name;
    int* mass;
    int size;
}massive;

typedef int(*callback)(char*);

int isnotsymb(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == EOF;
}

int load_a(massive* a, FILE *fi){
    char ch = (char)fgetc(fi);
    a->size = 0;
    int size_of_a = BUFSIZ, tek_el = 0;
    while (ch != EOF) {
        while(isnotsymb(ch)) {
            ch = (char)fgetc(fi);
        }
        int tek_el_mass[10], ind_tek_el_mass = 0;
        while(!isnotsymb(ch)) {
            if (isdigit(ch)) {
                if (ind_tek_el_mass > 9) {
                    free(a->mass);
                    a->mass = NULL;
                    a->size = 0;
                    return NUMS_IN_FILE_IS_INVALID;
                }
            tek_el_mass[ind_tek_el_mass++] = ch - '0';
            }
            else {
                free(a->mass);
                a->mass = NULL;
                a->size = 0;
                return NOT_A_NUMS_FILE;
            }
            ch = (char)fgetc(fi);
        }
        int tek_el_razr = 1;
        while(ind_tek_el_mass != 0) {
            tek_el += tek_el_mass[--ind_tek_el_mass] * tek_el_razr;
            tek_el_razr *= 10;
        }
        a->mass[a->size++] = tek_el;
        if (a->size == size_of_a) {
            size_of_a *= 2;
            int *new_a = realloc(a->mass, size_of_a * sizeof(int));
            if (new_a == NULL) {
                free(a->mass);
                a->mass = NULL;
                a->size = 0;
                return REALLOC_FAILED;
            }
            a->mass = new_a;
        }
        tek_el = 0;
    }
    return SUCCESS;
}



int save_a(const massive* a, FILE *fiout){
    if (a->mass == NULL) {
        return WRONG_INPUT_IN_FUNCTION;
    }
    char str[12];
    sprintf(str, "Massive %c: ", a->name);
    fputs(str, fiout);
    for(int i = 0; i < a->size; i++) {
        char buf[10];
        snprintf(buf, sizeof(buf), "%d", a->mass[i]);
        fputs(buf, fiout);
        if (i != a->size - 1){
            fputc(' ', fiout);
        }
    }
    fputc('\n', fiout);
    return SUCCESS;
}

int rand_a(massive* a, const int count, const int lb, const int lr){
    if (a->mass != NULL) {
        free(a->mass);
    }
    a->size = count;
    a->mass = malloc(sizeof(int) * a->size);
    srand(time(NULL));
    for(int i = 0; i < count; i++) {
        a->mass[i] = rand() % (lr - lb) + lb;
    }
    return SUCCESS;
}

int concat_a_b(massive* a, const massive* b){
    if (a->mass == NULL || b->mass == NULL) {
        return MASSIVE_CANNOT_BE_EMPTY;
    }
    for(int i = a->size; i < a->size + b->size; i++) {
        a->mass[i] = b->mass[i - a->size];
    }
    a->size += b->size;
    return SUCCESS;
}

int free_a(massive* a){
    if (a->mass == NULL) {
        return MASSIVE_CANNOT_BE_EMPTY;
    }
    free(a->mass);
    a->mass = malloc(sizeof(int) * 0);
    a->size = 0;
    return SUCCESS;
}

int remove_a(massive* a, const int ind_start, const int count){
    if (a == NULL) {
        return WRONG_INPUT_IN_FUNCTION;
    }
    massive RES, *res = &RES;
    res->name = a->name;
    res->size = a->size - count;
    res->mass = malloc(sizeof(int) * res->size);
    for(int i = 0; i < res->size; i++) {
        if (i >= ind_start) {
            res->mass[i] = a->mass[i + count];
        }
        else {
            res->mass[i] = a->mass[i];
        }
    }
    *a = *res;
    return SUCCESS;
}

int copy_a_b(const massive* a, massive* b, const int start, const int end){
    if (b->mass != NULL) {
        free(b->mass);
    }
    if (a->mass == NULL || start > end || end >= a->size || start < 0) {
        return WRONG_INPUT_IN_FUNCTION;
    }
    b->size = end - start + 1;
    b->mass = malloc(sizeof(int) * b->size);
    for (int i = start; i <= end; i++) {
        b->mass[i - start] = a->mass[i];
    }
    return SUCCESS;
}

int compare_up(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int sort_a_up(const massive* a){
    if (a->mass == NULL) {
        return MASSIVE_CANNOT_BE_EMPTY;
    }
    qsort(a->mass, a->size, sizeof(int), compare_up);
    return SUCCESS;
}

int compare_down(const void* a, const void* b)
{
    return *(int*)b - *(int*)a;
}

int sort_a_down(const massive* a){
    if (a->mass == NULL) {
        return MASSIVE_CANNOT_BE_EMPTY;
    }
    qsort(a->mass, a->size, sizeof(int), compare_down);
    return SUCCESS;
}

int compare_shuffle() {
    srand(time(NULL));
    switch(rand() % 2) {
        case 1 : return 1;
        case 0 : return -1;
        default: return 0;
    }
}

int shuffle_a(const massive* a){
    if (a->mass == NULL) {
        return MASSIVE_CANNOT_BE_EMPTY;
    }
    qsort(a->mass, a->size, sizeof(int), compare_shuffle);
    return SUCCESS;
}

typedef struct el_mass {
    int value;
    int count;
} el_mass;

int stats_a(const massive* a) {
    if (a->mass == NULL) {
        return MASSIVE_CANNOT_BE_EMPTY;
    }
    el_mass count_els[a->size];
    for (int i = 0; i < a->size; i++) {
        count_els[i].value = 0;
        count_els[i].count = 0;
    }
    int max_a = a->mass[0], min_a = a->mass[0], max_a_ind = 0, min_a_ind = 0, max_often_el = a->mass[0],
    max_count_el = 0, ind_count_els = 0;
    double average_number = 0.0;
    for (int i = 0; i < a->size; i++) {
        int tek_el = a->mass[i];
        if (tek_el > max_a) {
            max_a = tek_el;
            max_a_ind = i;
        }
        if (tek_el < min_a) {
            min_a = tek_el;
            min_a_ind = i;
        }
        average_number += tek_el;
        int fl_in_count_els = 0;
        for(int j = 0; j < i; j++) {
            if (tek_el == count_els[j].value) {
                count_els[j].count++;
                fl_in_count_els = 1;
                break;
            }
        }
        if (fl_in_count_els == 0) {
            count_els[ind_count_els].value = tek_el;
            count_els[ind_count_els++].count++;
        }
    }
    for (int i = 0; i < a->size; i++) {
        if (count_els[i].count == 0) {
            break;
        }
        int tek_el = count_els[i].value, tek_count = count_els[i].count;
        if (tek_count > max_count_el) {
            max_count_el = tek_count;
            max_often_el = tek_el;
        }
        else if (tek_count == max_count_el) {
            if (tek_el > max_often_el) {
                max_often_el = tek_el;
            }
        }
    }
    average_number /= a->size;
    double max_diff = fmax(fabs(max_a - average_number), fabs(min_a - average_number));
    printf("stats of massive %c:\n    size: %d\n    max of mass: %d, ind: %d\n    min of mass: %d, ind: %d\n"
           "    the most common element of mass: %d\n    average number of mass: %f\n    max diff: %f\n", a->name, a->size, max_a,
           max_a_ind, min_a, min_a_ind, max_often_el, average_number, max_diff);
    return SUCCESS;
}

int print_a_num(const massive* a, int ind) {
    if (a->mass != NULL) {
        if (ind < a->size && ind >= 0) {
            printf("Element of massive %c with index %d: %d\n", a->name, ind, a->mass[ind]);
            return SUCCESS;
        }
        return WRONG_INPUT_IN_FUNCTION;
    }
    return MASSIVE_CANNOT_BE_EMPTY;
}

int print_a_num_num(const massive* a, int ind_st, int ind_end) {
    if (a->mass != NULL) {
        if (ind_st > ind_end || ind_end > a->size - 1 || ind_st < 0) {
            return WRONG_INPUT_IN_FUNCTION;
        }
        printf("Elements of massive %c [%d : %d]: ", a->name, ind_st, ind_end);
        for (int i = ind_st; i < ind_end; i++) {
            printf("%d ", a->mass[i]);
        }
        if (ind_st != ind_end){
            printf("%d", a->mass[ind_end]);
        }
        printf("\n");
        return SUCCESS;
    }
    return MASSIVE_CANNOT_BE_EMPTY;
}

int print_a_all(const massive* a) {
    if (a->mass != NULL) {
        printf("Elements of massive %c:", a->name);
        if (a->size == 0){
            printf("\n");
            return SUCCESS;
        }
        for (int i = 0; i < a->size - 1; i++) {
            printf("%d ", a->mass[i]);
        }
        printf("%d", a->mass[a->size - 1]);
        printf("\n");
        return SUCCESS;
    }
    return MASSIVE_CANNOT_BE_EMPTY;
}

char* Tolower(char* str) {
    for(int i = 0; str[i]; i++){
        str[i] = (char)tolower(str[i]);
    }
    return str;
}

void CLEAR(massive *mass_mass) {
    if (mass_mass != NULL) {
        for(int i = 0; i < 26; i++) {
            if (mass_mass[i].mass != NULL) {
                free(mass_mass[i].mass);
            }
        }
        free(mass_mass);
    }
}

int isINT(char* str) {
    char* a = str;
    while(*str != '\0') {
        if (!isdigit(*str)) {
            return -1;
        }
        str++;
    }
    return atoi(a);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Wrong number of arguments\n");
        return -1;
    }
    massive *mass_mass = malloc(sizeof(massive) * 26);
    for (int i = 0; i < 26; i++) {
        mass_mass[i].mass = NULL;
        mass_mass[i].size = 0;
    }
    int i = 1;
    while (i < argc){
        if(!strcmp(Tolower(argv[i]), "load")){
            if (argc < i + 3) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            massive A, *a = &A;
            a->size = BUFSIZ;
            a->mass = malloc(sizeof(int) * a->size);
            a->name = (char)toupper(name);
            char* path = argv[++i];
            FILE *fi = fopen(path, "r");
            if (fi == NULL){
                printf("No such file\n");
                free(mass_mass);
                return -1;
            }
            int res = load_a(a, fi);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            fclose(fi);
            mass_mass[name - 'A'] = *a;
            printf("loaded successfully\n");
            print_a_all(a);
            printf("\n");
        }
        else if (!strcmp(Tolower(argv[i]), "save")){
            if (argc < i + 3) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            if (mass_mass[name - 'A'].mass == NULL) {
                printf("There is no such massive\n");
                free(mass_mass);
                return -1;
            }
            char* path = argv[++i];
            FILE *fiout = fopen(path, "w");
            if (fiout == NULL){
                printf("No such file\n");
                free(mass_mass);
                return -1;
            }
            int res = save_a(&mass_mass[name - 'A'], fiout);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            fclose(fiout);
            printf("saved successfully\n");
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "rand")){
            if (argc < i + 5) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            mass_mass[name - 'A'].name = name;
            int count = isINT(argv[++i]), lb = isINT(argv[++i]), lr = isINT(argv[++i]);
            if (count == -1 || lb == -1 || lr == -1) {
                printf("Сount or segment of numbers are not a integer numbers\n");
                free(mass_mass);
                return -1;
            }
            rand_a(&mass_mass[name - 'A'], count, lb, lr);
            printf("randed successfully\n");
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "concat")){
            if (argc < i + 3) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name1 = (char)toupper(argv[i][0]);
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name2 = (char)toupper(argv[i][0]);
            if (name1 == name2) {
                printf("Names of massives can not be the same\n");
                free(mass_mass);
                return -1;
            }
            massive *a = &mass_mass[name1 - 'A'], *b = &mass_mass[name2 - 'A'];
            print_a_all(a);
            print_a_all(b);
            int res = concat_a_b(a, b);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            printf("concated successfully\n");
            print_a_all(a);
            printf("\n");
        }
        else if (!strcmp(Tolower(argv[i]), "free")){
            if (argc < i + 2) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            print_a_all(&mass_mass[name - 'A']);
            int res = free_a(&mass_mass[name - 'A']);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            printf("freed successfully\n");
            print_a_all(&mass_mass[name - 'A']);
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "remove")){
            if (argc < i + 4) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            int start = isINT(argv[++i]), count = isINT(argv[++i]);
            if (start == -1 || count == -1) {
                printf("start or count are not a integer numbers\n");
                free(mass_mass);
                return -1;
            }
            print_a_all(&mass_mass[name - 'A']);
            int res = remove_a(&mass_mass[name - 'A'], start, count);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            printf("removed successfully\n");
            print_a_all(&mass_mass[name - 'A']);
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "copy")){
            if (argc < i + 3) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name1 = (char)toupper(argv[i][0]);
            int start = isINT(argv[++i]), end = isINT(argv[++i]);
            if (start == -1 || end == -1) {
                printf("start or end are not a integer numbers\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name2 = (char)toupper(argv[i][0]);
            if (name1 == name2) {
                printf("Names of massives can not be the same\n");
                free(mass_mass);
                return -1;
            }
            massive *a = &mass_mass[name1 - 'A'], *b = &mass_mass[name2 - 'A'];
            print_a_all(a);
            print_a_all(b);
            int res = copy_a_b(a, b, start, end);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            printf("copied successfully\n");
            print_a_all(a);
            print_a_all(b);
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "sort")){
            if (argc < i + 2) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 2 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            mass_mass[name - 'A'].name = name;
            print_a_all(&mass_mass[name - 'A']);
            int res;
            if (argv[i][1] == '-') {
                res = sort_a_down(&mass_mass[name - 'A']);
            }
            else if(argv[i][1] == '+') {
                res = sort_a_up(&mass_mass[name - 'A']);
            }
            else {
                printf("No such command\n");
                free(mass_mass);
                return -1;
            }
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            printf("sorted successfully\n");
            print_a_all(&mass_mass[name - 'A']);
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "shuffle")){
            if (argc < i + 2) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            mass_mass[name - 'A'].name = name;
            int res = shuffle_a(&mass_mass[name - 'A']);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            printf("shuffled successfully\n");
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "stats")){
            if (argc < i + 2) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            mass_mass[name - 'A'].name = name;
            int res = stats_a(&mass_mass[name - 'A']);
            if (res != SUCCESS) {
                ValidateCode(res);
                free(mass_mass);
                return -1;
            }
            printf("stats are displayed successfully\n");
            printf("\n");
        }
        else if(!strcmp(Tolower(argv[i]), "print")){
            if (argc < i + 3) {
                printf("Wrong number of arguments\n");
                free(mass_mass);
                return -1;
            }
            if (strlen(argv[++i]) != 1 || !isalpha(argv[i][0])) {
                printf("Wrong name of massive\n");
                free(mass_mass);
                return -1;
            }
            char name = (char)toupper(argv[i][0]);
            mass_mass[name - 'A'].name = name;
            if (isINT(argv[++i]) == -1) {
                if (!strcmp(argv[i], "all")) {
                    int res = print_a_all(&mass_mass[name - 'A']);
                    if (res != SUCCESS) {
                        ValidateCode(res);
                        free(mass_mass);
                        return -1;
                    }
                }
                else {
                    printf("No such command\n");
                    free(mass_mass);
                    return -1;
                }
            }
            else if (argc > i + 3) {
                int ind = isINT(argv[i]), end = isINT(argv[i + 1]);
                if (end != -1) {
                    i++;
                    int res = print_a_num_num(&mass_mass[name - 'A'], ind, end);
                    if (res != SUCCESS) {
                        ValidateCode(res);
                        free(mass_mass);
                        return -1;
                    }
                }
                else {
                    int res = print_a_num(&mass_mass[name - 'A'], ind);
                    if (res != SUCCESS) {
                        ValidateCode(res);
                        free(mass_mass);
                        return -1;
                    }
                }
            }
            else {
                int ind = isINT(argv[++i]);
                int res = print_a_num(&mass_mass[name - 'A'], ind);
                if (res != SUCCESS) {
                    ValidateCode(res);
                    free(mass_mass);
                    return -1;
                }
            }
            printf("printed successfully\n");
            printf("\n");
        }
        else {
            printf("No such command\n");
            break;
        }
        i++;
    }
    free(mass_mass);
    return 0;
}