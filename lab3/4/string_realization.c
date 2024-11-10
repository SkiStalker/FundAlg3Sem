#include "my_string.h"
#include "my_string.h"

String string_create(const char *str) {
    String instance = {0, NULL};
    if (str) {
        instance.size = strlen(str);
        instance.arr = malloc((instance.size + 1) * sizeof(char));
        if (instance.arr) {
            strcpy(instance.arr, str);
        } else {
            instance.size = -1;
        }
    }
    return instance;
}

void string_clear(String *str) {
    if (str && str->arr) {
        free(str->arr);
        str->arr = NULL;
        str->size = 0;
    }
}

int string_compare(const String *str_1, const String *str_2) {
    if (str_1->size != str_2->size) return str_1->size - str_2->size;
    return strcmp(str_1->arr, str_2->arr);
}

int string_equals(const String *str_1, const String *str_2) {
    return string_compare(str_1, str_2) == 0;
}

return_code string_copy(String *dest, const String *src) {
    if (!dest || !src) return MEMORY_ALLOCATION_ERROR;
    dest->size = src->size;
    if (src->arr) {
        dest->arr = malloc((dest->size + 1) * sizeof(char));
        if (!dest->arr) return MEMORY_ALLOCATION_ERROR;
        strcpy(dest->arr, src->arr);
    } else {
        dest->arr = NULL;
    }
    return OK;
}

String *string_duplicate(const String *src) {
    if (!src) return NULL;
    String *new_str = malloc(sizeof(String));
    if (!new_str || string_copy(new_str, src) != OK) {
        free(new_str);
        return NULL;
    }
    return new_str;
}

return_code string_concat(String *str_1, const String *str_2) {
    if (!str_1 || !str_2 || !str_1->arr || !str_2->arr) return MEMORY_ALLOCATION_ERROR;

    int new_size = str_1->size + str_2->size;
    char *new_arr = realloc(str_1->arr, (new_size + 1) * sizeof(char));
    if (!new_arr) return MEMORY_ALLOCATION_ERROR;

    str_1->arr = new_arr;
    strcat(str_1->arr, str_2->arr);
    str_1->size = new_size;
    return OK;
}

void string_destroy(String *str) {
    if (str) {
        string_clear(str);
        free(str);
    }
}