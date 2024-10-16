#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

double f1(double x) {
    return log(1.0 + x) / x;
}

double f2(double x) {
    return exp(-x * x / 2.0);
}

double f3(double x) {
    double tmp = 1.0 / (1.0 - x);
    return log(tmp);
}

double f4(double x) {
    return pow(x, x);
}

double trapezoidal(double (*func)(double), double epsilon) {

    double res = 0.0;
    double start = epsilon / 100000;
    do{
        res += ((func(start) + func(start + epsilon)) / 2.0) * epsilon;
        start += epsilon;
    }while(1.0 - start >= epsilon);
    return res;
}

bool is_number(const char *str) {
    if (*str == '-') {
        str++;
    }

    bool hasDot = false;
    if (*str == '\0') return false;

    while (*str) {
        if (*str == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <epsilon>\n", argv[0]);
        return 1;
    }
    double epsilon;
    if (is_number(argv[1])){
        epsilon = atof(argv[1]);
    }
    else{
        printf("'%s' - не является числом\n", argv[1]);
        return 0;
    }

    if (epsilon <= 0) {
        printf("Ошибка: epsilon должно быть положительным числом.\n");
        return 1;
    }


    printf("Integral 1: %lf\n", trapezoidal(f1, epsilon));
    printf("Integral 2: %lf\n", trapezoidal(f2,  epsilon));
    printf("Integral 3: %lf\n", trapezoidal(f3,  epsilon));
    printf("Integral 4: %lf\n", trapezoidal(f4,  epsilon));

    return 0;
}
