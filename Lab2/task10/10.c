#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int calculateFactorial(int n) {
    return (n <= 1) ? 1 : n * calculateFactorial(n - 1);
}

void reexpandPolynomial(double epsilon, double a, double **resultCoeffs, int degree, double *fCoeffs) {
    *resultCoeffs = (double *)malloc((degree + 1) * sizeof(double));
    if (*resultCoeffs == NULL) {
        printf("Ошибка выделения памяти.\n");
        return;
    }
    for (int k = 0; k <= degree; k++) {
        (*resultCoeffs)[k] = 0.0;
        for (int i = k; i <= degree; i++) {
            double term = fCoeffs[i] * pow(a, i - k) / calculateFactorial(i - k);
            (*resultCoeffs)[k] += term;
            if (fabs((*resultCoeffs)[k]) < epsilon) {
                (*resultCoeffs)[k] = 0.0;
            }
        }
    }
}

int validateDouble(const char *arg, double *value, double maxValue, int allowZero) {
    char *endptr;
    *value = strtod(arg, &endptr);
    if (*endptr != '\0' || isnan(*value) || *value > maxValue || (!allowZero && *value <= 0)) {
        return 0;
    }
    return 1;
}

int validateInteger(const char *arg, int *value) {
    char *endptr;
    *value = strtol(arg, &endptr, 10);
    if (*endptr != '\0' || *value <= 0) {
        return 0;
    }
    return 1;
}

void displayCoefficients(double *coeffs, int degree) {
    printf("Новые коэффициенты многочлена в степенях (x - a): ");
    for (int i = 0; i <= degree; i++) {
        if (fabs(coeffs[i]) < 1e-10) {
            printf("g%d = 0 ", i);
        } else {
            printf("g%d = %.6g ", i, coeffs[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Использование: ./a.out <epsilon> <a> <degree> <coeff_0> <coeff_1> ... <coeff_degree>\n");
        return 1;
    }

    double epsilon, a;
    int degree;

    if (!validateDouble(argv[1], &epsilon, 1.0, 0)) {
        printf("Ошибка: epsilon должно быть числом > 0 и <= 1.\n");
        return 1;
    }
    if (!validateDouble(argv[2], &a, 1e10, 1)) {
        printf("Ошибка: a должно быть числом <= 1e10.\n");
        return 1;
    }
    if (!validateInteger(argv[3], &degree) || degree + 4 > argc) {
        printf("Ошибка: степень должна быть положительным целым числом, и должно быть достаточно коэффициентов.\n");
        return 1;
    }

    double *coeffs = (double *)malloc((degree + 1) * sizeof(double));
    if (coeffs == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }

    for (int i = 0; i <= degree; i++) {
        if (!validateDouble(argv[4 + i], &coeffs[i], 1e10, 1)) {
            printf("Ошибка: коэффициент при x должен быть числом <= 1e10.\n");
            free(coeffs);
            return 1;
        }
    }

    double *newCoeffs;
    reexpandPolynomial(epsilon, a, &newCoeffs, degree, coeffs);

    displayCoefficients(newCoeffs, degree);

    free(newCoeffs);
    free(coeffs);

    return 0;
}
