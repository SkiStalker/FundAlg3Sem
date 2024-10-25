#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum {
    VALID,
    NOT_A_NUM,
    EMPTY_LINE
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case EMPTY_LINE: printf("empty line\n"); break;
        case NOT_A_NUM: printf("not a number\n"); break;
        default: printf("Unknown error code\n"); break;
    }
}

int ValidateNum(const char* argv) {
    int cnt = 0;
    while (*argv != '\0') {
        if (!isdigit(*argv)) {
            return NOT_A_NUM;
        }
        argv++;
        cnt++;
    }
    if (cnt == 0) {
        return EMPTY_LINE;
    }
    return VALID;
}

int my_strcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return 1;
}

int find_flag(const char* arg, const char** flags) {
    for (int i = 0; i < 5; ++i) {
        if (my_strcmp(arg, flags[i])) {
            return i;
        }
    }
    return -1;
}

int strLength(const char *str) {
    int len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

char *reverse(const char *str) {
    int len = strLength(str);
    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        result[i] = str[len - i - 1];
    }
    result[len] = '\0';
    return result;
}

char *StrToUpper(const char *str) {
    int len = strLength(str);
    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (i % 2 == 1) {
            if (c >= 'a' && c <= 'z') {
                result[i] =  c - 'a' + 'A';
            } else {
                result[i] = c;
            }
        }
        else {
            result[i] = c;
        }
    }
    result[len] = '\0';
    return result;
}

char *newOrder(const char *str) {
    int len = strLength(str);
    char digits[len], alpha[len], othersymbols[len];
    int a = 0, b = 0, c = 0;
    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    for (int i = 0; i < len; ++i) {
        if (isdigit(str[i])) {
            digits[a] = str[i];
            a++;
        } else if (isalpha(str[i])) {
            alpha[b] = str[i];
            b++;
        } else {
            othersymbols[c] = str[i];
            c++;
        }
    }
    int i = 0;
    while (i < len) {
        for (int j = 0; j < a; ++j) {
            result[i] = digits[j];
            i++;
        }
        for (int j = 0; j < b; ++j) {
            result[i] = alpha[j];
            i++;
        }
        for (int j = 0; j < c; ++j) {
            result[i] = othersymbols[j];
            i++;
        }
    }
    result[len] = '\0';
    return result;
}

unsigned int toInt(char* argv) {
    unsigned int res = 0;
    int i = 0;
    while (argv[i] != '\0') {
        res = res * 10 + argv[i] - '0';
        i++;
    }
    return res;
}

char *concat(int argc, char *argv[]) {
    unsigned int seed = toInt(argv[3]);
    int len = strLength(argv[2]);
    for (int i = 4; i < argc; ++i) {
        len += strLength(argv[i]);
    }
    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    srand(seed);
    int cnt = 0, id = 0, ind = 0;
    int markers[argc];
    for (int i = 0; i < argc; ++i) {
        markers[i] = 0;
    }
    while (cnt < argc - 3) {
        do {
            ind = rand() % argc;
        } while (markers[ind] == 1 || ind == 0 || ind == 1 || ind == 3);
        markers[ind] = 1;
        int curLen = strLength(argv[ind]);
        for (int k = 0; k < curLen; ++k) {
            result[id] = argv[ind][k];
            id++;
        }
        cnt++;
    }
    result[len] = '\0';
    return result;
}

int main(int argc, char *argv[]) {
    char* flags[] = { "-l", "-r", "-u", "-n", "-c" };

    if (argc <= 2) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    const int ret = find_flag(argv[1], flags);
    if (ret == -1) {
        printf("Flag %s is not supported.\n", argv[1]);
        return -1;
    }
    switch (ret) {
        case 0: {
            int len = strLength(argv[2]);
            printf("Length is %d.\n", len);
            break;
        }
        case 1: {
            char *rev = reverse(argv[2]);
            printf("Reverse is %s\n", rev);
            free(rev);
            break;
        }
        case 2: {
            char *res = StrToUpper(argv[2]);
            printf("EveryOddToUpped is %s\n", res);
            free(res);
            break;
        }
        case 3: {
            char *res = newOrder(argv[2]);
            printf("NewOrder is %s\n", res);
            free(res);
            break;
        }
        case 4: {
            if (argc < 5) {
                printf("Wrong number of arguments.\n");
                return -1;
            }
            int ret_code = ValidateNum(argv[3]);
            if (ret_code != VALID) {
                ValidateCode(ret_code);
                return -1;
            }
            char* res = concat(argc, argv);
            printf("New string is %s\n", res);
            free(res);
            break;
        }
        default: {
            printf("Invalid function type\n");
            break;
        }
    }


    return 0;
}
