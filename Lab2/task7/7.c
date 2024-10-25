#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double bisection(double (*func)(double), double a, double b, double epsilon) {
    double mid;
    
    if (func(a) * func(b) >= 0) {
        printf("Функция не меняет знак на концах интервала [%lf, %lf]. ", a, b);
        return NAN;
    }

    while ((b - a) / 2.0 > epsilon) {
        mid = (a + b) / 2.0;
        if (func(mid) == 0.0) {
            return mid;
        } else if (func(a) * func(mid) < 0) {
            b = mid;
        } else {
            a = mid;
        }
    }

    return (a + b) / 2.0;
}

double func1(double x) {
    return pow(x, 2) - 2;
}

double func2(double x) {
    return sin(x);
}

double func3(double x) {
    return pow(x, 3) - 5 * x + 3;
}

double func4(double x) {
    return exp(x) - 3;
}

double func5(double x) {
    return cos(x) - x;
}

int validateDouble(const char *str, double *value) {
    char *endptr;
    *value = strtod(str, &endptr);
    if (*endptr != '\0') {
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Использование: ./a.out <номер функции> <нижняя граница a> <верхняя граница b> <точность epsilon>\n");
        return 1;
    }

    int choice;
    double a, b, epsilon;
    double (*selected_func)(double); 

    if (sscanf(argv[1], "%d", &choice) != 1 || choice < 1 || choice > 5) {
        printf("Неверный номер функции. Введите число от 1 до 5.\n");
        return 1;
    }

    if (!validateDouble(argv[2], &a)) {
        printf("Неверное значение для нижней границы a.\n");
        return 1;
    }

    if (!validateDouble(argv[3], &b) || a >= b) {
        printf("Неверное значение для верхней границы b. Убедитесь, что a < b.\n");
        return 1;
    }

    if (!validateDouble(argv[4], &epsilon) || epsilon <= 0 || epsilon > 1) {
        printf("Неверное значение для точности. Точность должна быть числом от 0 до 1.\n");
        return 1;
    }

    switch (choice) {
        case 1:
            selected_func = func1;
            break;
        case 2:
            selected_func = func2;
            break;
        case 3:
            selected_func = func3;
            break;
        case 4:
            selected_func = func4;
            break;
        case 5:
            selected_func = func5;
            break;
        default:
            printf("Неверный выбор!\n");
            return 1;
    }

    double root = bisection(selected_func, a, b, epsilon);

    if (!isnan(root)) {
        printf("Корень уравнения на интервале [%.6f, %.6f] с точностью %.6f: %.6f\n", a, b, epsilon, root);
    } else {
        printf("Не удалось найти корень на указанном интервале.\n");
        return 1;
    }

    return 0;
}
