#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    int length;
} String;

int createString(String *str, const char *input) {
    str->length = strlen(input);
    str->data = (char *)malloc((str->length + 1) * sizeof(char));
    if (str->data == NULL) {
        return 1;
    }
    strcpy(str->data, input);
    return 0;
}

int deleteString(String *str) {
    if (str->data == NULL) {
        return 1;
    }
    free(str->data);
    str->data = NULL;
    str->length = 0;
    return 0;
}

int compareStrings(const String *str1, const String *str2) {
    if (str1->length != str2->length) {
        return ((str1->length > str2->length) - (str1->length < str2->length));
    }
    return strcmp(str1->data, str2->data);
}

int equalsStrings(const String *str1, const String *str2) {
    return strcmp(str1->data, str2->data) == 0;
}

int copyString(String *destination, const String *source) {
    if (source->data == NULL) {
        return 1;
    }
    free(destination->data);
    destination->length = source->length;
    destination->data = (char *)malloc((source->length + 1) * sizeof(char));
    if (destination->data == NULL) {
        return 1;
    }
    strcpy(destination->data, source->data);
    return 0;
}

int cloneString(String *clone, const String *source) {
    return createString(clone, source->data);
}

int concatenateStrings(String *str1, const String *str2) {
    if (str1->data == NULL || str2->data == NULL) {
        return 1;
    }
    str1->data = (char *)realloc(str1->data, (str1->length + str2->length + 1) * sizeof(char));
    if (str1->data == NULL) {
        return 1;
    }
    strcat(str1->data, str2->data);
    str1->length += str2->length;
    return 0;
}

int main() {
    String str1, str2, str3;
    
    if (createString(&str1, "Hello") != 0 || createString(&str2, "World") != 0) {
        printf("Error creating strings\n");
        return 1;
    }
    
    printf("String 1: %s, Length: %d\n", str1.data, str1.length);
    printf("String 2: %s, Length: %d\n", str2.data, str2.length);

    int comparison = compareStrings(&str1, &str2);
    printf("Comparison result: %d\n", comparison);

    int equality = equalsStrings(&str1, &str2);
    printf("Equality result: %d\n", equality);

    if (copyString(&str3, &str1) != 0) {
        printf("Error copying string\n");
        return 1;
    }
    printf("Copied String 3: %s, Length: %d\n", str3.data, str3.length);

    if (concatenateStrings(&str1, &str2) != 0) {
        printf("Error concatenating strings\n");
        return 1;
    }
    printf("Concatenated String 1: %s, Length: %d\n", str1.data, str1.length);

    if (deleteString(&str1) != 0 || deleteString(&str2) != 0 || deleteString(&str3) != 0) {
        printf("Error deleting strings\n");
        return 1;
    }

    return 0;
}
