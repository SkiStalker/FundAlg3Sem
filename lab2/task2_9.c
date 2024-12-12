#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

enum ret_type_t {
    SUCCESS,
    FRACT_VALUE_ERROR,
    NO_ARGUMENTS_ERROR,
    INCORRECT_NUMBER_SYSTEM
};

void reduce(int numerator, int* denominator) {
    int x = numerator, y = *denominator;
    for (int i = 2; i <= numerator; i++) {
        while (x % i == 0 && y % i == 0) {
            x /= i;
            y /= i;
        }
    }
    *denominator = y;
}

int is_endless(int denominator, int base) {
    int x = denominator;
    for (int i = 2; i <= denominator; i++) {
        if (x % i == 0 && base % i > 0)
            return 0;
        while (x % i == 0)
            x /= i;
    }
    return 1;
}

int count_float_dig(double x) {
    int cnt = 0;
    while (fabs(trunc(x) - x) > DBL_MIN) {
        x *= 10;
        cnt++;
    }
    return cnt;
}

enum ret_type_t required_func(int num, int base, ...) {
    if (num < 1)
        return NO_ARGUMENTS_ERROR;
    if (base < 2)
        return INCORRECT_NUMBER_SYSTEM;
    va_list arg;
    double fract;
    printf("base %d:    ", base);
    va_start(arg, base);
    while (num > -1) {
        num--;
        fract = va_arg(arg, double);
        if (fract < 0 || fract > 1)
            return FRACT_VALUE_ERROR;
        int denominator = pow(10, count_float_dig(fract));
        int numerator = (int)(fract * denominator);
        reduce(numerator, &denominator);
        if (is_endless(denominator, base))
            printf("%f:Yes ", fract);
        else
            printf("%f:No ", fract);
    }
    va_end(arg);
    return SUCCESS;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Incorrect number of arguments");
        return 1;
    }
    if (!strcmp(argv[1], "1")) {
        switch ((int)required_func(5, 2, 0.1, 0.0001, 0.5, 0.25, 0.8, 0.13)) {
        case 1:
            printf("Fraction must be from 0 to 1\n");
            return 1;
        case 2:
            printf("No arguments given\n");
            return 2;
        case 3:
            printf("Number system must be bigger 1\n");
            return 3;
        }
    }

    if (!strcmp(argv[1], "2")) {
        switch ((int)required_func(5, 2, 0.1, -0.0001, 0.5, 0.8, 0.13)) {
        case 1:
            printf("Fraction must be from 0 to 1\n");
            return 1;
        case 2:
            printf("No arguments given\n");
            return 2;
        case 3:
            printf("Number system must be bigger 1\n");
            return 3;
        }
    }

    return 0;
}
