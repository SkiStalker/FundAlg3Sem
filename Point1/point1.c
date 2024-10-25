#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

int lenCounter(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* reverse(const char *str) {
    int len = lenCounter(str);
    char *reversedStr = (char*)malloc(len + 1);
    if (reversedStr == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }
    
    for (int i = 0; i < len; i++) {
        reversedStr[i] = str[len - i - 1];
    }
    
    reversedStr[len] = '\0';
    return reversedStr;
}

char* upperOdd(const char *str) {
    int len = lenCounter(str);
    char *newStr = (char*)malloc(len + 1);
    if (newStr == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        if (i % 2 != 0) {
            newStr[i] = toupper(str[i]);
        } else {
            newStr[i] = str[i];
        }
    }

    newStr[len] = '\0';
    return newStr;
}

char* origOrder(const char *str) {
    int len = lenCounter(str);
    char *orderStr = (char*)malloc(len + 1);
    if (orderStr == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }

    int index = 0;

    for (int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            orderStr[index++] = str[i];
        }
    }

    for (int i = 0; i < len; i++) {
        if (isalpha(str[i])) {
            orderStr[index++] = str[i];
        }
    }

    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i]) && !isalpha(str[i])) {
            orderStr[index++] = str[i];
        }
    }

    orderStr[index] = '\0';
    return orderStr;
}

char* concatStr(const unsigned int seed, int count, char **strings) {
    srand(seed);

    int full_len = 0;
    for (int i = 0; i < count; i++) {
        full_len += lenCounter(strings[i]);
    }

    char *result = (char*)malloc(full_len + 1);
    if (result == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }

    result[0] = '\0';

    for (int k = 0; k < count; k++) {
        int index = rand() % count;
        strcat(result, strings[index]);
    }

    return result;
}

int str_to_unsigned_int(const char *str, unsigned int *result) {
    char *endptr;
    unsigned long value = strtoul(str, &endptr, 10);

    if (*endptr != '\0') {
        printf("Error: Invalid number input\n");
        return 0;
    }

    if (value > UINT_MAX) {
        printf("Error: Input exceeds the maximum for unsigned int\n");
        return 0;
    }

    *result = (unsigned int)value;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: <flag> <string>\n");
        return 1;
    }

    const char *flag = argv[1];
    const char *str = argv[2];

    if (!strcmp(flag, "-c")) {
        if (argc < 4) {
            printf("Error: Not enough arguments for the -c flag\n");
            return 1;
        }

        unsigned int seed;
        if (!str_to_unsigned_int(argv[2], &seed)) {
            return 1;
        }

        char *result = concatStr(seed, argc - 3, &argv[3]);
        if (result != NULL) {
            printf("Concatenated string: %s\n", result);
            free(result);
        }
        return 0;
    }

    if (!strcmp(flag, "-l")) {
        printf("String length = %d\n", lenCounter(str));
    } else if (!strcmp(flag, "-r")) {
        char *reversed = reverse(str);
        if (reversed != NULL) {
            printf("Reversed string: %s\n", reversed);
            free(reversed);
        }
    } else if (!strcmp(flag, "-u")) {
        char *newStr = upperOdd(str);
        if (newStr != NULL) {
            printf("New string: %s\n", newStr);
            free(newStr);
        }
    } else if (!strcmp(flag, "-n")) {
        char *orderedStr = origOrder(str);
        if (orderedStr != NULL) {
            printf("New string: %s\n", orderedStr);
            free(orderedStr);
        }
    } else {
        printf("Unknown flag: %s\n", flag);
        return 1;
    }

    return 0;
}
