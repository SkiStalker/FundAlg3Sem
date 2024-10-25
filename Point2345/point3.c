#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int findSubstring(const char *text, const char *substring, int **positions) {
    int text_len = strlen(text);
    int sub_len = strlen(substring);
    int count = 0;
    int capacity = 10;

    *positions = (int*)malloc(capacity * sizeof(int));
    if (*positions == NULL) {
        printf("Memory allocation error\n");
        return 0;
    }

    for (int i = 0; i <= text_len - sub_len; i++) {
        int j = 0;
        while (j < sub_len && text[i + j] == substring[j]) {
            j++;
        }
        if (j == sub_len) {
            if (count >= capacity) {
                capacity *= 2;
                *positions = (int*)realloc(*positions, capacity * sizeof(int));
                
                if (*positions == NULL) {
                    printf("Memory allocation error\n");
                    return count;
                }
            }
            (*positions)[count++] = i;
        }
    }

    return count;
}

void findLineAndPosition(const char *text, int pos, int *line, int *pos_in_line) {
    int current_line = 1;
    int current_pos_in_line = 1;

    for (int i = 0; i < pos; i++) {
        if (text[i] == '\n') {
            current_line++;
            current_pos_in_line = 0;
        }
        current_pos_in_line++;
    }

    *line = current_line;
    *pos_in_line = current_pos_in_line;
}

char* readFile(const char *file_path) {
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Failed to open file: %s\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(size + 1);

    if (buffer == NULL) {
        printf("Memory allocation error\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    return buffer;
}

void searchInFiles(const char *substring, int n, ...) {
    va_list args;
    va_start(args, n);

    for (int k = 0; k < n; k++) {
        const char *file_path = va_arg(args, const char *);
        char* file_content = readFile(file_path);
        
        if (file_content == NULL) {
            continue;
        }

        int *positions;
        int found_count = findSubstring(file_content, substring, &positions);

        printf("Results for file: %s\n", file_path);

        if (found_count == 0) {
            printf("Substring not found\n");
        } else {
            for (int j = 0; j < found_count; j++) {
                int line;
                int pos_in_line;
                findLineAndPosition(file_content, positions[j], &line, &pos_in_line);
                printf("Line: %d, Position: %d\n", line, pos_in_line);
            }
        }

        free(positions);
        free(file_content);
    }

    va_end(args);
}

int main() {
    const char *substring = "boba\toba\nloh";
    searchInFiles(substring, 2, "file1.txt", "file2.txt");
    return 0;
}
