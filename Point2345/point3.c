#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int locateSubstring(const char *text, const char *substring, int **indices) {
    int text_length = strlen(text);
    int sub_length = strlen(substring);
    int match_count = 0;
    int initial_capacity = 10;

    *indices = (int*)malloc(initial_capacity * sizeof(int));
    if (*indices == NULL) {
        printf("Error: Unable to allocate memory\n");
        return 0;
    }

    for (int i = 0; i <= text_length - sub_length; i++) {
        int j = 0;
        while (j < sub_length && text[i + j] == substring[j]) {
            j++;
        }
        if (j == sub_length) {
            if (match_count >= initial_capacity) {
                initial_capacity *= 2;
                *indices = (int*)realloc(*indices, initial_capacity * sizeof(int));
                
                if (*indices == NULL) {
                    printf("Error: Memory reallocation failed\n");
                    return match_count;
                }
            }
            (*indices)[match_count++] = i;
        }
    }

    return match_count;
}

void determineLineAndColumn(const char *text, int pos, int *line_num, int *col_num) {
    int current_line_num = 1;
    int current_col_num = 1;

    for (int i = 0; i < pos; i++) {
        if (text[i] == '\n') {
            current_line_num++;
            current_col_num = 0;
        }
        current_col_num++;
    }

    *line_num = current_line_num;
    *col_num = current_col_num;
}

char* loadFileContent(const char *file_path) {
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Error: Unable to open file %s\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(file_size + 1);

    if (buffer == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    return buffer;
}

void searchFilesForSubstring(const char *substring, int file_count, ...) {
    va_list args;
    va_start(args, file_count);

    for (int i = 0; i < file_count; i++) {
        const char *file_path = va_arg(args, const char *);
        char* content = loadFileContent(file_path);
        
        if (content == NULL) {
            continue;
        }

        int *positions;
        int occurrences = locateSubstring(content, substring, &positions);

        printf("File: %s - Search Results:\n", file_path);

        if (occurrences == 0) {
            printf("No matches found for the substring.\n");
        } else {
            for (int j = 0; j < occurrences; j++) {
                int line, column;
                determineLineAndColumn(content, positions[j], &line, &column);
                printf("Match found at Line %d, Column %d\n", line, column);
            }
        }

        free(positions);
        free(content);
    }

    va_end(args);
}

int main() {
    const char *substring = "BoBa\toBa\nTop";
    searchFilesForSubstring(substring, 2, "input_1.txt", "input_2.txt");
    return 0;
}
