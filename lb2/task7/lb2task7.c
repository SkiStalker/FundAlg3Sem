#include <stdio.h>
#include <math.h>

typedef enum {
    NO_ERROR,
    ERROR_INVALID_INTERVAL,
    ERROR_FUNCTION_VALUE_SAME_SIGN
} ErrorCode;

typedef double (*Function)(double x);

ErrorCode bisection(Function func, double a, double b, double epsilon, double* root) {
    if (a >= b) {
        return ERROR_INVALID_INTERVAL;
    }

    if (func(a) * func(b) > 0) {
        return ERROR_FUNCTION_VALUE_SAME_SIGN;
    }

    while ((b - a) / 2 > epsilon) {
        double c = (a + b) / 2;
        if (func(c) == 0.0) {
            *root = c;
            return NO_ERROR;
        }
        else if (func(a) * func(c) < 0) {
            b = c;
        }
        else {
            a = c;
        }
    }

    *root = (a + b) / 2;
    return NO_ERROR;
}

// Пример функции: f(x) = x^2 - 2
double func1(double x) {
    return x * x - 2;
}

// Пример функции: f(x) = x - 1
double func2(double x) {
    return x - 1;
}

// Пример функции: f(x) = cos(x) - x
double func3(double x) {
    return cos(x) - x;
}

void print_error(ErrorCode error) {
    switch (error) {
    case NO_ERROR:
        printf("No error.\n");
        break;
    case ERROR_INVALID_INTERVAL:
        printf("Error: Invalid interval.\n");
        break;
    case ERROR_FUNCTION_VALUE_SAME_SIGN:
        printf("Error: Function values at the interval endpoints have the same sign.\n");
        break;
    }
}

int main() {
    double root;
    ErrorCode error;

    // Тестирование первой функции
    error = bisection(func1, 0, 2, 0.001, &root);
    print_error(error);
    if (error == NO_ERROR) {
        printf("Root of x^2 - 2: %.6f\n", root);
    }

    // Тестирование второй функции
    error = bisection(func2, 0, 2, 0.001, &root);
    print_error(error);
    if (error == NO_ERROR) {
        printf("Root of x - 1: %.6f\n", root);
    }

    // Тестирование третьей функции
    error = bisection(func3, 0, 1, 0.001, &root);
    print_error(error);
    if (error == NO_ERROR) {
        printf("Root of cos(x) - x: %.6f\n", root);
    }

    // Тестирование с некорректным интервалом
    error = bisection(func1, 2, 0, 0.001, &root);
    print_error(error);

    // Тестирование функции с одинаковыми знаками
    error = bisection(func1, -1, 1, 0.001, &root);
    print_error(error);

    return 0;
}
