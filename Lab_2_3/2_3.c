#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum ret_type_t{
    SUCCESS,
    ERROR_NAN,
    ERROR_FULL,
    ERROR_ARGS,
    ERROR_MALLOC,
    ERROR_OPEN_FILE
};

void logErrors(int code) {
    switch (code) {
        case ERROR_NAN:
            printf("Found not a number\n");
            break;

        case ERROR_FULL:
            printf("Overflow detected\n");
            break;

        case ERROR_ARGS:
            printf("Wrong number of arguments\n");
            break;

        case ERROR_MALLOC:
            printf("Failed to malloc\n");
            break;

        case ERROR_OPEN_FILE:
            printf("Failed to open file\n");
            break;

        default:
            printf("UNKNOWN ERROR CODE\n");
            break;
    }
}

typedef struct {
    int len;
    int capacity;
    int* buffer;
} Vector;

Vector* CreateVector(int initialCapacity, int* code) {
    *code = SUCCESS;

    Vector* v = (Vector*)malloc(sizeof(Vector*));
    if (v == NULL) {
        *code = ERROR_MALLOC;
        return NULL;
    }

    v->buffer = (int*)malloc(sizeof(int*) * initialCapacity);
    if (v->buffer == NULL) {
        *code = ERROR_MALLOC;
        free(v);
        return NULL;
    }

    v->capacity = initialCapacity;
    v->len = 0;
    return v;
}

void DestroyVector(Vector* v) {
    free(v->buffer);
    free(v);
}

int VectorPush(Vector* v, int value) {
    if (v->len + 1 >= v->capacity) {
        int* newBuffer = (int*)realloc(v->buffer, v->capacity * sizeof(int) * 2);
        if (newBuffer == NULL) {
            return ERROR_MALLOC;
        }

        v->buffer = newBuffer;
        v->capacity *= 2;
    }

    v->buffer[v->len++] = value;
    return SUCCESS;
}

int findEnterancesInFile(char* substring, char* path, Vector* lines, Vector* chars, Vector* fileIds, int fileId) {
    FILE* fd = fopen(path, "r");
    if (!fd) {
        return ERROR_OPEN_FILE;
    }
    int code;
    int currentLine = 1;
    int currentChar = 1;
    int currentLen = 0;
    int start = 0;
    char currentFile = fgetc(fd);
    char* currentSubstr = substring;

    while (currentFile != EOF) {
        if (currentFile != *currentSubstr) {
            if (*currentSubstr == '\0') {
                code = VectorPush(lines, currentLine);
                if (code != SUCCESS) {
                    fclose(fd);
                    return code;
                }

                code = VectorPush(chars, currentChar);
                if (code != SUCCESS) {
                    fclose(fd);
                    return code;
                }

                code = VectorPush(fileIds, fileId);
                if (code != SUCCESS) {
                    fclose(fd);
                    return code;
                }
            }

            currentSubstr = substring;
            start++;
            fseek(fd, start, SEEK_SET);
            currentFile = fgetc(fd);
            currentChar++;
            if (currentFile == '\n') {
                currentLine++;
                currentChar = 0;
            }

            currentLen = 0;
            continue;
        }

        currentFile = fgetc(fd);
        currentSubstr++;
        currentLen++;
    }

    if (*currentSubstr == '\0') {
        code = VectorPush(lines, currentLine);
        if (code != SUCCESS) {
            fclose(fd);
            return code;
        }
        code = VectorPush(chars, currentChar);
        if (code != SUCCESS) {
            fclose(fd);
            return code;
        }

        code = VectorPush(fileIds, fileId);
        if (code != SUCCESS) {
            fclose(fd);
            return code;
        }
    }

    fclose(fd);
    return SUCCESS;
}

int findEnterancesInFiles(Vector* lines, Vector* chars, Vector* fileIds, char* substring, int count, ...) {
    int code;
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        char* path = va_arg(args, char*);
        code = findEnterancesInFile(substring, path, lines, chars, fileIds, i);
        if (code != SUCCESS) {
            va_end(args);
            return code;
        }
    }

    va_end(args);
    return code;
}

int main() {

    int code;
    Vector* lines = CreateVector(2, &code);
    if (code != SUCCESS) {
        logErrors(code);
        return code;
    }

    Vector* chars = CreateVector(2, &code);
    if (code != SUCCESS) {
        DestroyVector(lines);
        logErrors(code);
        return code;
    }

    Vector* fileIds = CreateVector(2, &code);
    if (code != SUCCESS) {
        DestroyVector(chars);
        DestroyVector(lines);
        logErrors(code);
        return code;
    }

    code = findEnterancesInFiles(lines, chars, fileIds, "n\nn", 1, "input_1.txt");

    if (code != SUCCESS) {
        logErrors(code);
        DestroyVector(lines);
        DestroyVector(chars);
        DestroyVector(fileIds);
        return code;
    }

    for (int i = 0; i < lines->len; i++) {
        printf("Found substring enterance in file with id: %d, on position: line: %d, char: %d\n",
               fileIds->buffer[i], lines->buffer[i] + 1, chars->buffer[i]);
    }

    DestroyVector(lines);
    DestroyVector(chars);
    DestroyVector(fileIds);

    return SUCCESS;
}