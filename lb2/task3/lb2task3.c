#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef enum {
    SUCCESS,
    FILE_OPEN_ERROR,
    SUBSTRING_NOT_FOUND,
    NO_SUBSTRING_ERROR,
    NO_FILES_ERROR
} ErrorCode;

int searchInFile(const char* substring, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file.\n");
        return FILE_OPEN_ERROR;
    }

    char line[1024];
    int lineNumber = 0;
    int found = 0;
    size_t subLength = strlen(substring);

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        size_t lineLength = strlen(line); 

        for (size_t i = 0; i <= lineLength - subLength; i++) { 
            if (strncmp(&line[i], substring, subLength) == 0) {
                printf("File: %s, Line: %d, Column: %zu\n", filename, lineNumber, i + 1);
                found = 1;
            }
        }
    }

    fclose(file);

    if (!found) {
        printf("Error: Substring not found in file.\n");
        return SUBSTRING_NOT_FOUND;
    }

    return SUCCESS;
}

int searchInFiles(const char* substring, int fileCount, ...) {
    va_list files;
    va_start(files, fileCount);
    int overallError = SUCCESS;

    for (int i = 0; i < fileCount; i++) {
        const char* filename = va_arg(files, const char*);
        int error = searchInFile(substring, filename);
        if (error != SUCCESS) {
            overallError = error;
            break;
        }
    }

    va_end(files);
    return overallError;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        if (argc == 1) {
            printf("Error: No substring and no files provided.\n");
            return SUBSTRING_NOT_FOUND;
        }
        else if (argc == 2) {
            printf("Error: No files provided.\n");
            return NO_FILES_ERROR;
        }
        else {
            printf("Error: No substring provided.\n");
            return SUBSTRING_NOT_FOUND;
        }
    }

    const char* substring = argv[1];
    int fileCount = argc - 2;

    if (fileCount <= 0) {
        printf("Error: No files provided.\n");
        return EXIT_FAILURE;
    }

    int error = searchInFiles(substring, fileCount, argv[2], argv[3], argv[4], argv[5]);

    if (error != SUCCESS) {
        return EXIT_FAILURE;
    }

    return SUCCESS;
}




