#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>

enum status_codes {
    OK,
    ERR_NOT_A_NUMBER,
    ERR_OVERFLOW,
    ERR_INVALID_ARGS,
    ERR_MEMORY,
    ERR_FILE_OPEN,
    ERR_TOO_MANY_ITERATIONS,
    ERR_NO_SOLUTION
};

double compute_derivative(int order, double point, int degree, const double* coefficients) {
    double result = 0.0;

    for (int i = order; i <= degree; i++) {
        double term = coefficients[i];
        for (int j = 0; j < order; j++) {
            term *= (i - j);
        }
        result += term * pow(point, i - order);
    }
    return result;
}

int decompose_polynomial(double tolerance, double point, double* output, int degree, ...) {
    if (tolerance < 0 || output == NULL) {
        return ERR_INVALID_ARGS;
    }

    va_list args;
    va_start(args, degree);

    double* coefficients = (double*)malloc((degree + 1) * sizeof(double));
    if (coefficients == NULL) {
        return ERR_MEMORY;
    }

    for (int i = 0; i <= degree; i++) {
        coefficients[i] = va_arg(args, double);
    }
    va_end(args);

    unsigned long long factorial = 1;
    for (int i = 0; i <= degree; i++) {
        if (i != 0) {
            factorial *= i;
        }
        output[i] = compute_derivative(i, point, degree, coefficients) / (double)factorial;

        if (fabs(output[i]) < tolerance) {
            output[i] = 0.0;
        }
    }

    free(coefficients);
    return OK;
}

int main() {
    int degree = 2;
    double tolerance = 0.00001;
    double a = 1.0;
    double* output = (double*)malloc(sizeof(double) * (degree + 1));
    if (output == NULL) {
        printf("Memory allocation failed\n");
        return ERR_MEMORY;
    }

    int code = decompose_polynomial(tolerance, a, output, degree, 2.0, 3.0, 4.0);
    switch (code) {
    case ERR_NOT_A_NUMBER:
        printf("Not a number found\n");
        break;

    case ERR_OVERFLOW:
        printf("Overflow detected\n");
        break;

    case ERR_INVALID_ARGS:
        printf("Invalid arguments\n");
        break;

    case ERR_MEMORY:
        printf("Memory allocation failed\n");
        break;

    case ERR_FILE_OPEN:
        printf("Failed to open file\n");
        break;

    case ERR_TOO_MANY_ITERATIONS:
        printf("Too many iterations. Exiting\n");
        break;

    case ERR_NO_SOLUTION:
        printf("No solution in the given interval\n");
        break;

    case OK:
        for (int i = 0; i <= degree; i++) {
            printf("g%d = %f\n", i, output[i]);
        }
        break;

    default:
        printf("UNKNOWN ERROR CODE\n");
        break;
    }

    free(output);
    return code;
}











