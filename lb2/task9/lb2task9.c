#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define TOLERANCE DBL_EPSILON

typedef enum StatusCode
{
    OK,
    INVALID_INPUT,
    INVALID_BASE,
    MEMORY_FAIL
} StatusCode;

unsigned long long find_gcd(unsigned long long x, unsigned long long y)
{
    while (x > 0 && y > 0)
    {
        if (x > y)
            x %= y;
        else
            y %= x;
    }
    return (x > 0) ? x : y;
}

int check_representation(int base, double value)
{
    unsigned long long numerator, denominator = 1;
    int i;
    while (fabs(value - floor(value)) > TOLERANCE && ULLONG_MAX / 10 >= denominator)
    {
        value *= 10;
        denominator *= 10;
    }
    numerator = (unsigned long long)value;
    denominator /= find_gcd(numerator, denominator);

    i = 2;
    if (!(denominator & 1) && (base & 1))
        return 0;
    while (!(denominator & 1))
        denominator /= 2;
    for (i = 3; i <= base; i += 2)
    {
        if (denominator % i == 0 && base % i)
            return 0;
        while (denominator % i == 0)
            denominator /= i;
    }
    if (denominator != 1)
        return 0;
    return 1;
}

StatusCode find_finite_representations(int base, double* result, int* result_length, int num_count, ...)
{
    *result_length = 0;
    if (!result)
        return INVALID_INPUT;
    if (base < 2)
        return INVALID_BASE;

    int i;
    StatusCode status = OK;
    double number;
    va_list args;
    va_start(args, num_count);
    *result_length = 0;

    for (i = 0; i < num_count; ++i)
    {
        number = va_arg(args, double);
        if (number > 1. - TOLERANCE || number < TOLERANCE)
        {
            status = INVALID_INPUT;
            continue;
        }

        if (check_representation(base, number))
        {
            result[(*result_length)++] = number;
        }
    }

    va_end(args);
    return status;
}

int main()
{
    double result[128];

    int result_length = 0;
    switch (find_finite_representations(36, result, &result_length, 1, 0.25))
    {
    case INVALID_BASE:
        printf("Invalid base\n");
        return INVALID_BASE;

    case INVALID_INPUT:
        printf("Some numbers are entered incorrectly\n");
        break;

    case MEMORY_FAIL:
        printf("Memory allocation failed\n");
        return MEMORY_FAIL;

    case OK:
        if (result_length)
        {
            printf("answer: ");
            for (int i = 0; i < result_length; ++i)
            {
                printf("%lf ", result[i]);
            }
            putchar('\n');
        }
        else
        {
            printf("There are no such numbers\n");
        }
        break;
    }
    return 0;
}