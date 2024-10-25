#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    SUCCESS,
    ERROR_NOT_A_NUMBER,
    ERROR_EMPTY_SRT
} ret_type_t;

unsigned int AtoUi(const char* argv) {
    unsigned int res = 0;
    int i = 0;
    while (argv[i] != '\0') {
        res = res * 10 + argv[i] - '0';
        i++;
    }
    return res;
}

void HandlingError(const int code) {
    switch (code) {
        case ERROR_EMPTY_SRT:
            printf("Nothing was entered.\n");
            break;
        case ERROR_NOT_A_NUMBER:
            printf("Not a number\n");
            break;
        default:
            printf("ABORTED!!!\n");
            break;
    }
}

int is_int(const char* argv) {
    int cnt = 0;
    while (*argv != '\0') {
        if (!isdigit(*argv)) {
            return ERROR_NOT_A_NUMBER;
        } else {
            argv++;
            cnt++;
        }
    }
    if (cnt == 0) {
        return ERROR_EMPTY_SRT;
    }
    return SUCCESS;
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

int strLen(const char *str) {
    int res = 0;
    while (*str != '\0') {
        res++;
        str++;
    }
    return res;
}

char *funcForR(const char *str) {
    int len = strLen(str);
    char *reversed = (char*)malloc(len + 1);
    if (reversed == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';
    return reversed;
}

char *funcForU(const char *str) {
    int len = strLen(str);
    char *res = (char*)malloc(len + 1);
    if (res == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        if (i % 2 == 1) {
            res[i] = toupper(str[i]);
        } else {
            res[i] = str[i];
        }
    }
    res[len] = '\0';
    return res;
}

char *funcForN(const char *str) {
    int len = strLen(str);
    char digits[len], alpha[len], others[len];
    int len_dig = 0, len_al = 0, len_ot = 0;

    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    for (int i = 0; i < len; ++i) {
        if (isdigit(str[i])) {
            digits[len_dig] = str[i];
            len_dig++;
        } else if (isalpha(str[i])) {
            alpha[len_al] = str[i];
            len_al++;
        } else {
            others[len_ot] = str[i];
            len_ot++;
        }
    }
    int i = 0;
    while (i < len) {
        for (int j = 0; j < len_dig; ++j) {
            result[i] = digits[j];
            i++;
        }
        for (int j = 0; j < len_al; ++j) {
            result[i] = alpha[j];
            i++;
        }
        for (int j = 0; j < len_ot; ++j) {
            result[i] = others[j];
            i++;
        }
    }
    result[len] = '\0';
    return result;
}

char *funcForC(int argc, char *argv[]) {
    unsigned int seed = AtoUi(argv[3]);
    int len = strLen(argv[2]);

    for (int i = 4; i < argc; ++i) {
        len += strLen(argv[i]);
    }
    char *res = (char*)malloc(len + 1);
    if (res == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }
    srand(seed);
    int cnt = 0, res_ind = 0, str_num = 0;
    int conced[argc];

    for (int i = 0; i < argc; ++i) {
        conced[i] = 0;
    }

    while (cnt < (argc - 3)) {
        do {
            str_num = rand() % argc;
        } while (conced[str_num] == 1 || str_num == 0 || str_num == 1 || str_num == 3);
        conced[str_num] = 1;

        int curLen = strLen(argv[str_num]);
        for (int k = 0; k < curLen; ++k) {
            res[res_ind] = argv[str_num][k];
            res_ind++;
        }
        cnt++;
    }
    res[len] = '\0';
    return res;
}

int main(int argc, char *argv[]) {
    const char* flags[] = { "-l", "-r", "-u", "-n", "-c" };

    if (argc <= 2) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    const int ret = find_flag(argv[1], flags);
    if (ret == -1) {
        printf("Flag <%s> is not supported.\n", argv[1]);
        return -1;
    }
    switch (ret) {
        case 0: {
            int len = strLen(argv[2]);
            printf("Length of string: %d.\n", len);
            break;
        }
        case 1: {
            char *rev = funcForR(argv[2]);
            printf("Reverse string: %s\n", rev);
            free(rev);
            break;
        }
        case 2: {
            char *res = funcForU(argv[2]);
            printf("New half-upper string: %s\n", res);
            free(res);
            break;
        }
        case 3: {
            char *res = funcForN(argv[2]);
            printf("Right ordered string: %s\n", res);
            free(res);
            break;
        }
        case 4: {
            if (argc < 5) {
                printf("Wrong number of arguments.\n");
                return -1;
            }
            int ret_code = is_int(argv[3]);
            if (ret_code) {
                HandlingError(ret_code);
                return ret_code;
            }
            char* res = funcForC(argc, argv);
            printf("Concatenated string: <%s>\n", res);
            free(res);
            break;
        }
        default: {
            printf("Smth went wrong.\n");
            break;
        }
    }
    return 0;
}