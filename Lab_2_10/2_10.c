#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

enum ret_type_t{
    SUCCESS,
    ERROR_NAN,
    ERROR_FULL,
    ERROR_ARGS,
    ERROR_MALLOC,
    ERROR_OPEN_FILE,
    ERROR_ITERATIONS_LIMIT,
    ERROR_NO_SOLVE
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
            printf("Wrong arguments\n");
            break;

        case ERROR_MALLOC:
            printf("Failed to malloc\n");
            break;

        case ERROR_OPEN_FILE:
            printf("Failed to open file\n");
            break;

        case ERROR_ITERATIONS_LIMIT:
            printf("Too many iterations. Time to shut up\n");
            break;

        case ERROR_NO_SOLVE:
            printf("There is no solvement in your interval\n");
            break;

        default:
            printf("UNKNOWN ERROR CODE\n");
            break;
    }
}

// возвращает значение i-й производной в точке(а), n - порядок многочлена
double derivative(int i, double a, int n, const double *coeffs) {
    double result = 0.0;

    for (int j = i; j <= n; j++) {
        double term = coeffs[j];
        for (int k = 0; k < i; k++) {
            term *= (j - k);
        }
        result += term * pow(a, j - i);
    }
    return result;
}

int decomposePolynomial(double eps, double a, double *result, int n, ...) {
    if ((eps < 0) || (result == NULL)){
        return ERROR_ARGS;
    }

    va_list args;
    va_start(args, n);

    double *coeffs = (double*)malloc((n + 1) * sizeof(double));
    if (coeffs == NULL) {
        return ERROR_MALLOC;
    }

    for (int i = 0; i <= n; i++) {
        coeffs[i] = va_arg(args, double);
    }
    va_end(args);

    unsigned long long int fact = 1;
    for (int i = 0; i <= n; i++) {
        if (i != 0) {
            fact *= i;
        }
        result[i] = derivative(i, a, n, coeffs) / (double)fact;

        if (fabs(result[i]) < eps) {
            result[i] = 0.0;
        }
    }

    free(coeffs);
    return SUCCESS;
}

int main() {
    int n = 2;
    double epsilon = 0.00001;
    double a = 1.0;
    double* result = (double*)malloc(sizeof(double) * (n + 1));
    if (result == NULL){
        logErrors(ERROR_MALLOC);
        return ERROR_MALLOC;
    }

    int code = decomposePolynomial(epsilon, a, result, n, 2.0, 3.0, 4.0);
    if (code != SUCCESS) {
        logErrors(code);
        free(result);
        return code;
    }

    for (int i = 0; i <= n; i++) {
        printf("g%d = %f\n", i, result[i]);
    }

    free(result);

    double* result1 = (double*)malloc(sizeof(double) * (n + 1));
    if (result1 == NULL){
        logErrors(ERROR_MALLOC);
        return ERROR_MALLOC;
    }

    int code1 = decomposePolynomial(epsilon, a, result1, n, 5.0, 2.0, 4.0);
    if (code1 != SUCCESS) {
        logErrors(code);
        free(result1);
        return code1;
    }
    printf("\n");
    for (int i = 0; i <= n; i++) {
        printf("g%d = %f\n", i, result1[i]);
    }

    free(result1);
    return 0;
}