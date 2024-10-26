#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum {
    MALLOC_FAILED,
    OK
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case MALLOC_FAILED: printf("malloc failed\n"); break;
        case OK: break;
        default: printf("Unknown error code\n"); break;
    }
}

int** PythagorTriangle(int num) {
    int** coefs_str_num = malloc((num + 1) * sizeof(int*));
    if (coefs_str_num == NULL) {
        return NULL;
    }
    for (int i = 0; i <= num; i++) {
        coefs_str_num[i] = malloc((i + 1) * sizeof(int));
        int* tek_str = coefs_str_num[i];
        if (tek_str == NULL) {
            free(coefs_str_num);
            return NULL;
        }
        tek_str[0] = 1;
        for(int j = 1; j < i; j++) {
            tek_str[j] = coefs_str_num[i - 1][j - 1] + coefs_str_num[i - 1][j];
        }
        tek_str[i] = 1;
    }
    return coefs_str_num;
}

int COEFS(double a, double* res_coefs, int n, ...) {
    va_list args;
    va_start (args, n);
    double* tek_coefs = (double*)malloc(sizeof(double) * (n + 1));
    if (tek_coefs == NULL) {
        return MALLOC_FAILED;
    }
    for(int i = 0; i <= n; i++) {
        tek_coefs[i] = va_arg(args, double);
        res_coefs[i] = tek_coefs[i];
    }
    int** PythagorTriangle_tek = PythagorTriangle(n);
    if (PythagorTriangle_tek == NULL) {
        free(tek_coefs);
        return MALLOC_FAILED;
    }
    for (int i = n; i > 0; i--) {
        int* tek_triangle = PythagorTriangle_tek[i];
        double tek_a = a;
        for (int j = i - 1; j >= 0; j--) {
            res_coefs[j] += tek_triangle[j] * tek_a * tek_coefs[i];
            tek_a *= a;
        }
    }
    for (int i = 0; i <= n; i++) {
        printf("%lf ", res_coefs[i]);
    }
    printf("\n");
    va_end(args);
    free(PythagorTriangle_tek);
    free(tek_coefs);
    return OK;
}

int main() {
    int n = 3;
    double* res_coefs = (double*)malloc((n + 1) * sizeof(double));
    if (res_coefs == NULL) {
        return MALLOC_FAILED;
    }
    ValidateCode(COEFS(2.0, res_coefs, n, 4.0, 3.0, 2.0, 1.0));
    ValidateCode(COEFS(3.0, res_coefs, n, 7.0, 3.5, 2.7, 1.4));
    ValidateCode(COEFS(5.0, res_coefs, n, 17.1, 2.1, 1.5, 6.1));
    free(res_coefs);
    return 0;
}