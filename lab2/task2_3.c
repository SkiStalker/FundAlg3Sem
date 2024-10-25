#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int prefix(const char* str, int** pi) {
    int len = strlen(str);
    *pi = (int*)malloc(len * sizeof(int));
    if (!(*pi)) {
        printf("Memory allocation error");
        free(*pi);
        return 1;
    }
    (*pi)[0] = 0;
    int j = 0, i = 1;
    while (i < len) {
        if (str[i] == str[j]) {
            (*pi)[i] = j + 1;
            i++;
            j++;
        }
        else {
            if (!j) {
                (*pi)[i] = 0;
                i++;
            }
            else
                j = (*pi)[j - 1];
        }
    }
    return 0;
}

int find_substr(const char* file_name, const char* substr) {
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        printf("File opening error");
        return 1;
    }
    int is_substr = 0, string_num = 1, char_num = -1;
    char cur = fgetc(fp);
    if (strlen(substr) == 1) {
        while (cur != EOF) {
            if (cur == '\n')
                string_num++;
            char_num++;
            if (cur == substr[0]) {
                is_substr = 1;
                printf("Substring in %d string(%d symbol) ", string_num, char_num);
            }
            cur = fgetc(fp);
        }
    }
    else {
        int* pi = NULL;
        if (prefix(substr, &pi))
            return 1;
        printf("%s:   ", file_name);
        int l = 0, k = 0, len = strlen(substr);
        while (cur != EOF) {
            if (cur == '\n')
                string_num++;
            char_num++;
            if (cur == substr[l]) {
                cur = fgetc(fp);
                k++;
                l++;
                if (l == len) {
                    is_substr = 1;
                    printf("Substring in %d string(%d symbol)  ", string_num, k - l - 1);
                }
            }
            else {
                if (l == 0) {
                    k++;
                    cur = fgetc(fp);
                }
                else
                    l = pi[l - 1];
            }
        }
        free(pi);
    }
    if (!is_substr)
        printf("No such substring ");
    fclose(fp);
    return 0;
}

int process_files(int cnt, const char* substr, ...) {
    if (!strcmp(substr, "")) {
        printf("Empty substring error");
        return 1;
    }
    va_list factor;
    va_start(factor, substr);
    char* file_name = NULL;
    for (int i = 0; i < cnt; i++) {
        file_name = va_arg(factor, char*);
        if (find_substr(file_name, substr))
            return 1;
    }
    va_end(factor);
    return 0;
}

/*int kmp(const char* text, const char* substr) {
    int* pi = NULL;
    int n = strlen(substr), m = strlen(text);
    if(prefix(substr, &pi))
        return 1;
    int k = 0, l = 0;
    while(k < m){
        if(text[k] == substr[l]){
            l++;
            k++;
            if(l == n)
                printf("In %d symbol\n", k - l);
        }
        else{
            if(l == 0){
                k++;
                if (k == m){
                    printf("No such substr");
                    break;
                }
            }
            else
                l = pi[l - 1];
        }
    }
    free(pi);
    return 0;
}*/

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Incorrect number of arguments");
        return 1;
    }
    if (!strcmp(argv[1], "1"))
        if (process_files(2, "ab", "1.txt", "2.txt"))
            return 1;
    if (!strcmp(argv[1], "2"))
        if (process_files(2, "ab", "1.txt", "3.txt"))
            return 1;

    return 0;
}

