#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

int calculateLength(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* reverseString(const char *str) {
    int len = calculateLength(str);
    char *reversedStr = (char*)malloc(len + 1);
    if (reversedStr == NULL) {
        printf("Unable to allocate memory\n");
        return NULL;
    }
    
    for (int i = 0; i < len; i++) {
        reversedStr[i] = str[len - i - 1];
    }
    
    reversedStr[len] = '\0';
    return reversedStr;
}

char* capitalizeOddIndices(const char *str) {
    int len = calculateLength(str);
    char *newStr = (char*)malloc(len + 1);
    if (newStr == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        newStr[i] = (i % 2 != 0) ? toupper(str[i]) : str[i];
    }

    newStr[len] = '\0';
    return newStr;
}

char* reorderCharacters(const char *str) {
    int len = calculateLength(str);
    char *orderStr = (char*)malloc(len + 1);
    if (orderStr == NULL) {
        printf("Memory allocation failed\n");
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

char* generateConcatenatedString(const unsigned int seed, int count, char **strings) {
    srand(seed);

    int full_len = 0;
    for (int i = 0; i < count; i++) {
        full_len += calculateLength(strings[i]);
    }

    char *result = (char*)malloc(full_len + 1);
    if (result == NULL) {
        printf("Insufficient memory\n");
        return NULL;
    }

    result[0] = '\0';

    for (int k = 0; k < count; k++) {
        int index = rand() % count;
        strcat(result, strings[index]);
    }

    return result;
}

int convertStringToUnsignedInt(const char *str, unsigned int *result) {
    char *endptr;
    unsigned long value = strtoul(str, &endptr, 10);

    if (*endptr != '\0') {
        printf("Invalid input: not a valid number\n");
        return 0;
    }

    if (value > UINT_MAX) {
        printf("Error: Number too large for unsigned int\n");
        return 0;
    }

    *result = (unsigned int)value;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Incorrect usage. Please provide <flag> <string>\n");
        return 1;
    }

    const char *flag = argv[1];
    const char *str = argv[2];

    if (!strcmp(flag, "-c")) {
        if (argc < 4) {
            printf("Error: Insufficient arguments for the -c flag\n");
            return 1;
        }

        unsigned int seed;
        if (!convertStringToUnsignedInt(argv[2], &seed)) {
            return 1;
        }

        char *result = generateConcatenatedString(seed, argc - 3, &argv[3]);
        if (result != NULL) {
            printf("Concatenated result: %s\n", result);
            free(result);
        }
        return 0;
    }

    if (!strcmp(flag, "-l")) {
        printf("Length of the string = %d\n", calculateLength(str));
    } else if (!strcmp(flag, "-r")) {
        char *reversed = reverseString(str);
        if (reversed != NULL) {
            printf("Reversed version: %s\n", reversed);
            free(reversed);
        }
    } else if (!strcmp(flag, "-u")) {
        char *newStr = capitalizeOddIndices(str);
        if (newStr != NULL) {
            printf("Modified string: %s\n", newStr);
            free(newStr);
        }
    } else if (!strcmp(flag, "-n")) {
        char *orderedStr = reorderCharacters(str);
        if (orderedStr != NULL) {
            printf("Reordered string: %s\n", orderedStr);
            free(orderedStr);
        }
    } else {
        printf("Unrecognized flag: %s\n", flag);
        return 1;
    }

    return 0;
}