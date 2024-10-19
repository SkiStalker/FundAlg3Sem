// lb1task3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

typedef enum {
    SUCCESS,
    RAVNO_0,
    RAVNO_1,
    MENHE_0,
    OUTOF,
    ERROR
} ret_type_t;

typedef struct {
    double x1;
    double x2;
    double A;
    double B;
    double C;
    int hasSolutions;
} QuadraticResult;

ret_type_t quadratic_equation(double epsilon, double a, double b, double c, int* count, QuadraticResult* results) {
    *count = 0;
    if (a < -DBL_MAX || a > DBL_MAX || b < -DBL_MAX || b > DBL_MAX || c < -DBL_MAX || c > DBL_MAX) {
        return OUTOF;
    }

    double coeff[3] = { a, b, c };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i != j) {
                for (int k = 0; k < 3; k++) {
                    if (k != i && k != j) {
                        double A = coeff[i];
                        double B = coeff[j];
                        double C = coeff[k];

                        double discrim = B * B - 4 * A * C;
                        QuadraticResult result;
                        result.A = A;
                        result.B = B;
                        result.C = C;

                        if (fabs(discrim) < epsilon) {
                            result.x1 = -B / (2 * A);
                            result.x2 = result.x1;
                            result.hasSolutions = 1;
                        }
                        else if (discrim > 0) {
                            result.x1 = (-B + sqrt(discrim)) / (2 * A);
                            result.x2 = (-B - sqrt(discrim)) / (2 * A);
                            result.hasSolutions = 2;
                        }
                        else {
                            result.hasSolutions = 0;
                        }

                        results[*count] = result;
                        (*count)++;
                    }
                }
            }
        }
    }

    return SUCCESS;
}


ret_type_t check_divisibility(int num1, int num2, const char** message) {
    if (num1 < INT_MIN || num1 > INT_MAX || num2 < INT_MIN || num2 > INT_MAX) {
        return OUTOF;
    }
    else if (num2 == 0) {
        *message = "Error: division by zero";
        return RAVNO_0;
    }
    else if (num1 % num2 == 0) {
        *message = "multiple";
        return SUCCESS;
    }
    else {
        *message = "not a multiple";
        return ERROR;
    }
}

ret_type_t check_triangle(double a, double b, double c, double epsilon, const char** message) {
    if (a < -DBL_MAX || a > DBL_MAX || b < -DBL_MAX || b > DBL_MAX || c < -DBL_MAX || c > DBL_MAX) {
        return OUTOF;
    }
    else if (a <= 0 || b <= 0 || c <= 0) {
        *message = "Sides must be greater than zero";
        return MENHE_0;
    }
    else if (fabs(a + b - c) < epsilon || fabs(a + c - b) < epsilon || fabs(b + c - a) < epsilon) {
        *message = "Can't form a triangle";
        return ERROR;
    }
    *message = "Can form a triangle";
    return SUCCESS;
}


int is_valid_number(const char* str) {
    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough arguments\n");
        return ERROR;
    }

    char flag = argv[1][0];
    if (flag != '-' && flag != '/') {
        printf("Invalid flag\n");
        return ERROR;
    }

    const char* message = NULL;

    switch (argv[1][1]) {
    case 'q': {
        if (argc != 6) {
            printf("Invalid number of parameters for flag -q\n");
            return ERROR;
        }

        if (!is_valid_number(argv[2]) || !is_valid_number(argv[3]) ||
            !is_valid_number(argv[4]) || !is_valid_number(argv[5])) {
            printf("All parameters must be valid numbers\n");
            return ERROR;
        }

        double epsilon = atof(argv[2]);
        double a = atof(argv[3]);
        double b = atof(argv[4]);
        double c = atof(argv[5]);

        QuadraticResult results[10];
        int count;
        ret_type_t result = quadratic_equation(epsilon, a, b, c, &count, results);
        if (result == OUTOF) {
            printf("Values are out of range -q\n");
        }
        else {
            printf("Results for the quadratic equation:\n");
            for (int i = 0; i < count; i++) {
                if (results[i].hasSolutions == 1) {
                    printf("- For A=%.2lf, B=%.2lf, C=%.2lf: x1 = x2 = %.2lf\n", results[i].A, results[i].B, results[i].C, results[i].x1);
                }
                else if (results[i].hasSolutions == 2) {
                    printf("- For A=%.2lf, B=%.2lf, C=%.2lf: x1 = %.2lf, x2 = %.2lf\n", results[i].A, results[i].B, results[i].C, results[i].x1, results[i].x2);
                }
                else {
                    printf("- For A=%.2lf, B=%.2lf, C=%.2lf: No valid solutions\n", results[i].A, results[i].B, results[i].C);
                }
            }
        }
        break;
    }
    case 'm': {
        if (argc != 4) {
            printf("Invalid number of parameters for flag -m\n");
            return ERROR;
        }

        if (!is_valid_number(argv[2]) || !is_valid_number(argv[3])) {
            printf("Both parameters must be valid numbers\n");
            return ERROR;
        }

        int num1 = atoi(argv[2]);
        int num2 = atoi(argv[3]);
        ret_type_t result = check_divisibility(num1, num2, &message);
        if (result == OUTOF) {
            printf("Values are out of range -m\n");
        }
        else {
            printf("Checking divisibility: ");
            printf("%d is %s %d\n", num1, message, num2);
        }
        break;
    }
    case 't': {
        if (argc != 6) {
            printf("Invalid number of parameters for flag -t\n");
            return ERROR;
        }

        if (!is_valid_number(argv[2]) || !is_valid_number(argv[3]) ||
            !is_valid_number(argv[4]) || !is_valid_number(argv[5])) {
            printf("All parameters must be valid numbers\n");
            return ERROR;
        }

        double epsilon = atof(argv[2]);
        double a = atof(argv[3]);
        double b = atof(argv[4]);
        double c = atof(argv[5]);

        ret_type_t result = check_triangle(a, b, c, epsilon, &message);
        if (result == OUTOF) {
            printf("Values are out of range -t\n");
        }
        else {
            printf("Triangle check: ");
            printf("%s\n", message);
        }
        break;
    }
    default:
        printf("Unknown flag\n");
        return -1;
    }

    return SUCCESS;
}
