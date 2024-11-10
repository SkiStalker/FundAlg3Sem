#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum return_code {
    OK,
    INVALID_INPUT,
    MEMORY_ALLOCATION_ERROR,
    OVERFLOW_ERROR,
    DEREFERENCING_NULL_POINTER_ATTEMPT,
    FILE_PATH_LENGTH_ERROR,
    FILE_OPENNING_ERROR
} return_code;

typedef struct {
    int size;
    char *arr;
} String;

String string_create(const char *str);
void string_clear(String *str);
int string_compare(const String *str_1, const String *str_2);
int string_equals(const String *str_1, const String *str_2);
return_code string_copy(String *dest, const String *src);
String *string_duplicate(const String *src);
return_code string_concat(String *str_1, const String *str_2);
void string_destroy(String *str);

#endif // MY_STRING_H
