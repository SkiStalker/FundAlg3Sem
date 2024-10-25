#include "functions.h"

int main(int argc, char *argv[])
{
    double result;
    puts("\n\ttest 1\nx - 11.2232, eps = 1e-10\nexpected result: 11.2232");
    switch (dichotomy_method(&result, 11., 11.5, 1.e-10, func1))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    puts("\n\ttest 2\nx - 11.2232, eps = 0.01\nexpected result: 11.22(...)");
    switch (dichotomy_method(&result, 11., 11.5, 0.01, func1))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    puts("\n\ttest 3\nsin(x)-1/x, eps = 1e-10\nexpected result: 1.114157");
    switch (dichotomy_method(&result, 1., 2.5, 1.e-10, func2))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    puts("\n\ttest 4\nx^2 + 10x - 4, eps = 1e-10\nexpected result: -10.385165");
    switch (dichotomy_method(&result, -10., -12.5, 1.e-10, func3))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    puts("\n\ttest 5\n3x^3 + x^2 + x + 1, eps = 1e-10\nexpected result: -0.635024");
    switch (dichotomy_method(&result, -2., 2., 1.e-10, func4))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    puts("\n\ttest 6\nsqrt(x) - 4.4, eps = 1e-10\nexpected result: 19.36");
    switch (dichotomy_method(&result, 0., 20., 1.e-10, func5))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    puts("\n\ttest 6\nsqrt(x) - 4.4, eps = 1e-10\nexpected result: no roots in this interval");
    switch (dichotomy_method(&result, 0., 19.35, 1.e-10, func5))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    puts("\n\ttest 6\nsqrt(x) - 4.4, eps = -1e-10\nexpected result: Input error");
    switch (dichotomy_method(&result, 0., 20., -1.e-10, func5))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case NO_ROOTS:
        puts("No roots in this interval\n");
        break;
    case SUCCESS:
        printf("\t result: %lf\n", result);
        break;
    }

    return 0;
}