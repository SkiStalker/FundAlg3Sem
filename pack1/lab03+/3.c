#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SUCCESS 0
#define ERROR_INVALID_ARGS 1
#define ERROR_MEMORY_ALLOCATION 2
#define ERROR_DIVISION_BY_ZERO 3
#define ERROR_FILE_OPEN 4
#define ERROR_INVALID_INPUT 5

typedef struct {
    int status;
    double result;
} CalculationResult;

int float_equal(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

int solve_quadratic(double a, double b, double c, double epsilon, FILE *output) {
    if (output == NULL) {
        return ERROR_FILE_OPEN;
    }

    double discriminant = b * b - 4 * a * c;

    if (float_equal(discriminant, 0, epsilon)) {
        double root = -b / (2 * a);
        fprintf(output, "Одно решение: %.10f\n", root);
    } else if (discriminant > 0) {
        double root1 = (-b + sqrt(discriminant)) / (2 * a);
        double root2 = (-b - sqrt(discriminant)) / (2 * a);
        fprintf(output, "Два решения: %.10f и %.10f\n", root1, root2);
    } else {
        fprintf(output, "Нет вещественных решений\n");
    }

    return SUCCESS;
}

int check_multiplicity(int a, int b, FILE *output) {
    if (b == 0) {
        return ERROR_DIVISION_BY_ZERO;
    }

    if (a % b == 0) {
        fprintf(output, "%d кратно %d\n", a, b);
    } else {
        fprintf(output, "%d не кратно %d\n", a, b);
    }

    return SUCCESS;
}

int check_triangle(double a, double b, double c, double epsilon, FILE *output) {
    if (output == NULL) {
        return ERROR_FILE_OPEN;
    }

    if (float_equal(a * a, b * b + c * c, epsilon) ||
        float_equal(b * b, a * a + c * c, epsilon) ||
        float_equal(c * c, a * a + b * b, epsilon)) {
        fprintf(output, "Числа могут быть длинами сторон прямоугольного треугольника\n");
    } else {
        fprintf(output, "Числа не могут быть длинами сторон прямоугольного треугольника\n");
    }

    return SUCCESS;
}

int validate_args(int argc, const char *flag) {
    if (strcmp(flag, "-q") == 0 && argc != 6) {
        return ERROR_INVALID_ARGS;
    } else if (strcmp(flag, "-m") == 0 && argc != 4) {
        return ERROR_INVALID_ARGS;
    } else if (strcmp(flag, "-t") == 0 && argc != 6) {
        return ERROR_INVALID_ARGS;
    }
    return SUCCESS;
}

int validate_double(const char *str, double *value) {
    char *endptr;
    *value = strtod(str, &endptr);
    return (*endptr == '\0') ? SUCCESS : ERROR_INVALID_INPUT;
}

int validate_int(const char *str, int  *value) {
    char *endptr;
    *value = strtol(str, &endptr, 10);
    return (*endptr == '\0') ? SUCCESS : ERROR_INVALID_INPUT;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Ошибка: недостаточно аргументов.\n");
        return ERROR_INVALID_ARGS;
    }

    double epsilon;
    int status;
    FILE *output = stdout;

    if (strcmp(argv[1], "-q") == 0) {
        status = validate_args(argc, "-q");
        if (status != SUCCESS) {
            fprintf(stderr, "Ошибка: неверное количество аргументов для флага -q.\n");
            return status;
        }

        if ((status = validate_double(argv[2], &epsilon)) != SUCCESS) {
            fprintf(stderr, "Некорректный ввод аргумента командной строки.\n");
            return ERROR_INVALID_INPUT;
        }

        double a, b, c;
        if ((status = validate_double(argv[3], &a)) != SUCCESS ||
            (status = validate_double(argv[4], &b)) != SUCCESS ||
            (status = validate_double(argv[5], &c)) != SUCCESS) {
            fprintf(stderr, "Некорректный ввод аргумента командной строки.\n");
            return ERROR_INVALID_INPUT;
        }

        status = solve_quadratic(a, b, c, epsilon, output);
        if (status != SUCCESS) return status;

    } else if (strcmp(argv[1], "-m") == 0) {
        status = validate_args(argc, "-m");
        if (status != SUCCESS) {
            fprintf(stderr, "Ошибка: неверное количество аргументов для флага -m.\n");
            return status;
        }

        int a, b;
        if ((status = validate_int(argv[2], &a)) != SUCCESS ||
            (status = validate_int(argv[3], &b)) != SUCCESS) {
            fprintf(stderr, "Некорректный ввод аргумента командной строки.\n");
            return ERROR_INVALID_INPUT;
        }

        status = check_multiplicity(a, b, output);
        if (status == ERROR_DIVISION_BY_ZERO) {
            fprintf(stderr, "Ошибка: делитель не может быть равен 0.\n");
            return status;
        }

    } else if (strcmp(argv[1], "-t") == 0) {
        status = validate_args(argc, "-t");
        if (status != SUCCESS) {
            fprintf(stderr, "Ошибка: неверное количество аргументов для флага -t.\n");
            return status;
        }

        if ((status = validate_double(argv[2], &epsilon)) != SUCCESS) {
            fprintf(stderr, "Некорректный ввод аргумента командной строки.\n");
            return ERROR_INVALID_INPUT;
        }

        double a, b, c;
        if ((status = validate_double(argv[3], &a)) != SUCCESS ||
            (status = validate_double(argv[4], &b)) != SUCCESS ||
            (status = validate_double(argv[5], &c)) != SUCCESS) {
            fprintf(stderr, "Некорректный ввод аргумента командной строки.\n");
            return ERROR_INVALID_INPUT;
        }

        status = check_triangle(a, b, c, epsilon, output);
        if (status != SUCCESS) return status;

    } else {
        fprintf(stderr, "Ошибка: неизвестный флаг '%s'.\n", argv[1]);
        return ERROR_INVALID_ARGS;
    }

    return SUCCESS;
}




