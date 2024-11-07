#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *content;
    int size;
} CustomString;

CustomString initializeString(const char *text) {
    CustomString result;
    result.size = strlen(text);
    result.content = (char *)malloc((result.size + 1) * sizeof(char));
    if (result.content) {
        memcpy(result.content, text, result.size + 1);
    }
    printf("[Инициализация строки] \"%s\" (длина: %d)\n", result.content, result.size);
    return result;
}

void releaseString(CustomString *string) {
    if (string->content) {
        free(string->content);
        string->content = NULL;
    }
    string->size = 0;
}

void compareStrings(const CustomString *first, const CustomString *second) {
    if (first->size != second->size) {
        printf("[Сравнение строк] \"%s\" %s \"%s\"\n", first->content, 
               (first->size < second->size) ? "меньше чем" : "больше чем", 
               second->content);
    } else {
        int result = strcmp(first->content, second->content);
        printf("[Сравнение строк] \"%s\" %s \"%s\"\n", first->content, 
               (result < 0) ? "меньше чем" : (result > 0) ? "больше чем" : "равна", 
               second->content);
    }
}

void checkEquality(const CustomString *first, const CustomString *second) {
    printf("[Проверка равенства] \"%s\" и \"%s\": %s\n", 
           first->content, second->content, 
           (strcmp(first->content, second->content) == 0) ? "Да" : "Нет");
}

void duplicateString(CustomString *destination, const CustomString *source) {
    if (destination->content) {
        free(destination->content);
    }
    destination->size = source->size;
    destination->content = (char *)malloc((destination->size + 1) * sizeof(char));
    if (destination->content) {
        memcpy(destination->content, source->content, destination->size + 1);
    }
    printf("[Дублирование строки] Исходная строка: \"%s\" => Дубликат: \"%s\"\n", 
           source->content, destination->content);
}

CustomString duplicateToNew(const CustomString *source) {
    CustomString result;
    result.size = source->size;
    result.content = (char *)malloc((result.size + 1) * sizeof(char));
    if (result.content) {
        memcpy(result.content, source->content, result.size + 1);
    }
    printf("[Создание дубликата] Новая строка: \"%s\" (длина: %d)\n", result.content, result.size);
    return result;
}

void appendStrings(CustomString *destination, const CustomString *source) {
    destination->content = (char *)realloc(destination->content, (destination->size + source->size + 1) * sizeof(char));
    if (destination->content) {
        memcpy(destination->content + destination->size, source->content, source->size + 1);
        destination->size += source->size;
    }
    printf("[Конкатенация строк] Результат: \"%s\" (новая длина: %d)\n", destination->content, destination->size);
}

int main() {
    CustomString string1 = initializeString("Привет");
    CustomString string2 = initializeString("Мир");

    printf("[Инициализация завершена] Строка1: \"%s\", Строка2: \"%s\"\n", string1.content, string2.content);

    compareStrings(&string1, &string2);
    checkEquality(&string1, &string2);

    CustomString string3 = initializeString("");
    printf("[Инициализация завершена] Строка3: \"%s\"\n", string3.content);
    duplicateString(&string3, &string1);

    appendStrings(&string1, &string2);

    printf("\n");

    CustomString string4 = initializeString("Пример");
    printf("[Инициализация завершена] Строка4: \"%s\"\n", string4.content);
    CustomString duplicatedString = duplicateToNew(&string4);
    compareStrings(&string4, &duplicatedString);

    appendStrings(&duplicatedString, &string2);
    printf("[Дополнительная конкатенация] Строка4 + Строка2: \"%s\"\n", duplicatedString.content);

    releaseString(&string1);
    releaseString(&string2);
    releaseString(&string3);
    releaseString(&string4);
    releaseString(&duplicatedString);

    return 0;
}
