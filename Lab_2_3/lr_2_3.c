#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


int strStr(char *str, char *substr) {
    int pos = 1;
    while (*str) {
        char *s = str;
        char *p = substr;
        while (*p && (*s == *p)) {
            s++;
            p++;
        }
        if (!*p) {
            return pos;
        }
        str++;
        pos++;
    }
    return -1;
}

void solve(char *substr, int cnt, ...) {
    va_list args;
    va_start(args, cnt);

    for (int i = 0; i < cnt; i++) {
        char *str = va_arg(args, char *);

        FILE* file = fopen(str, "r");
        if (file == NULL) {
            printf("File opening error <%s>.\n", str);
            return;
        }
        printf("From file <%s>:\n", str);
        char cur = fgetc(file);
        int line_cnt = 0;
        while (cur != EOF) {
            int size = 2, line_ind = 0, rewrite_cnt = 0;
            char* line = (char*)malloc((size + 1)*sizeof(char));
            if (line == NULL) {
                printf("Memory allocating error.\n");
                return;
            }
            if (cur == '\n') {
                cur = fgetc(file);
            }
            while (cur != '\n' && cur != EOF) {
                if (rewrite_cnt == (size + 1)) {
                    size *= 2;
                    char* ptr = (char*)realloc(line, (size + 1)*sizeof(char));
                    if (ptr == NULL) {
                        printf("Error reallocating memory.\n");
                        free(line);
                        return;
                    }
                    line = ptr;
                }
                line[line_ind] = cur;
                cur = fgetc(file);
                line_ind++;
                rewrite_cnt++;
            }
            line_cnt++;
            line[line_ind] = '\0';
            char* ptr = line;
            int substr_cnt, addition = 0;
            while (strStr(ptr, substr) != -1) {
                substr_cnt = strStr(ptr, substr);
                if (substr_cnt != -1) {
                    printf("Line %d from %d symbol.\n", line_cnt, substr_cnt + addition);
                }
                ptr += substr_cnt;
                addition += substr_cnt;
            }
            free(line);
        }
        fclose(file);
    }
    va_end(args);
}


int main() {
    printf("Test1:\n");
    solve("abaaba", 2, "./test1.txt", "./test2.txt");
    printf("Test2:\n");
    solve("abaaba", 3, "./test3.txt", "./test2.txt", "./test1.txt");
    printf("Test3:\n");
    solve("fg", 3, "./test3.txt", "./test2.txt", "./test1.txt");
}