#include <stdio.h>
#include <stdlib.h>

enum ret_type_t {
    SUCCESS,
    ERROR
};

int length(const char* str) {
    int res = 0;
    while (*str) {
        res++;
        str++;
    }
    return res;
}

enum ret_type_t reverse(const char* str, char** new_str) {
    int len = length(str);
    *new_str = (char*)malloc(len+1);
    if (!(*new_str)) {
        printf("Memory allocation error");
        return ERROR;
    }
    for (int i = 0; i < len; i++)
        (*new_str)[i] = str[len - 1 - i];
    (*new_str)[len] = '\0';
    return SUCCESS;

}

enum ret_type_t odd_up(const char* str, char** new_str) {
    int len = length(str);
    *new_str = (char*)malloc(len+1);
    if (!(*new_str)) {
        printf("Memory allocation error");
        return ERROR;
    }
    for (int i = 0; i < len; i++) {
        if (i % 2 && 'a' <= str[i] && str[i] <= 'z')
            (*new_str)[i] = str[i] - 32;
        else
            (*new_str)[i] = str[i];
    }
    (*new_str)[len] = '\0';
    return SUCCESS;
}

void append(char* str, int* i, char c) {
    str[*i] = c;
    (*i)++;
}

enum ret_type_t group_symbols(const char* str, char** new_str) {

    int len = length(str), i_num = 0, i_let = 0, i_oth = 0;
    char* nums = (char*)malloc(len);
    char* letters = (char*)malloc(len);
    char* other = (char*)malloc(len);
    *new_str = (char*)malloc(len);

    if (!(nums && letters && other && *new_str)) {
        printf("Memory allocation error");
        free(nums);
        free(letters);
        free(other);
        free(*new_str);
        *new_str = NULL;
        return ERROR;
    }

    for (int i = 0; i < len; i++) {
        if ('0' <= str[i] && str[i] <= '9')
            append(nums, &i_num, str[i]);
        else if (('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z'))
            append(letters, &i_let, str[i]);
        else
            append(other, &i_oth, str[i]);
    }

    int ind = 0;
    for (int i = 0; i < i_num; i++)
        append(*new_str, &ind, nums[i]);
    for (int i = 0; i < i_let; i++)
        append(*new_str, &ind, letters[i]);
    for (int i = 0; i < i_oth; i++)
        append(*new_str, &ind, other[i]);

    free(nums);
    free(letters);
    free(other);
    return SUCCESS;
}

enum ret_type_t concatenate(char* str, char** new_str) {
    int l1 = length(str), l2 = 0, i;
    if (!(*new_str))
        *new_str = (char*)malloc(l1 + 1);
    else {
        l2 = length(*new_str);
        *new_str = (char*)realloc(*new_str, l1 + l2 + 1);
    }
    if (!(*new_str)) {
        printf("Memory allocation error");
        free(*new_str);
        return ERROR;
    }
    for (i = l2; i < l1 + l2; i++)
        (*new_str)[i] = str[i - l2];
    append(*new_str, &i, '\0');
    return SUCCESS;
}

int compare(char* str1, const char* str2) {
    char* c1 = str1;
    const char* c2 = str2;
    while (*c1 && *c2) {
        if (*c1 != *c2)
            return 0;
        c1++;
        c2++;
    }
    if (*c1 != *c2)
        return 0;
    return 1;
}

int is_num(char* str) {
    char* s = str;
    while (*s) {
        if (*s < '0' || *s > '9')
            return 0;
        s++;
    }
    return 1;
}

int main(int argc, char* argv[])
{

    if (argc < 3) {
        printf("To little parameters, must be bigger 2");
        return 1;
    }

    if (compare(argv[1], "-l")) {
        int len = length(argv[2]);
        printf("%d", len);
    }
    else if (compare(argv[1], "-r")) {
        char* new_str = NULL;
        if (reverse(argv[2], &new_str)) {
            free(new_str);
            return 1;
        }
        printf("%s", new_str);
        free(new_str);
    }
    else if (compare(argv[1], "-u")) {
        char* new_str = NULL;
        if (odd_up(argv[2], &new_str)) {
            free(new_str);
            return 1;
        }
        printf("%s", new_str);
        free(new_str);
    }
    else if (compare(argv[1], "-n")) {
        char* new_str = NULL;
        if (group_symbols(argv[2], &new_str)) {
            free(new_str);
            return 1;
        }
        printf("%s", new_str);
        free(new_str);
    }

    else if (compare(argv[1], "-c")) {
        if (argc < 5) {
            printf("To little parameters for this flag, must be bigger 4");
            return 1;
        }
        if (!is_num(argv[3])) {
            printf("You must give unsigned number by third argument");
            return 1;
        }
        srand(strtoul(argv[3], NULL, 10));
        char* new_str = NULL;
        int size = argc - 3, ind;
        int* inds = (int*)malloc(sizeof(int) * size);
        if (!inds) {
            printf("Memory allocation error");
            free(inds);
            return 1;
        }
        inds[0] = 2;
        for (int i = 1; i < size; i++)
            inds[i] = 3 + i;
        for (int i = 0; i < size; i++) {
            ind = rand() % size;
            while (inds[ind] == -1) {
                ind++;
                ind %= size;
            }
            if (concatenate(argv[inds[ind]], &new_str)) {
                free(new_str);
                return 1;
            }
            inds[ind] = -1;
        }
        printf("%s", new_str);
        free(inds);
        free(new_str);
    }
    else {
        printf("Flag %s does not exist", argv[1]);
        return 1;
    }

    return 0;
}