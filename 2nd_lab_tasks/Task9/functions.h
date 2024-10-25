#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define EPS DBL_EPSILON

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    BASE_ERROR,
    MEMORY_ERROR
} status;


unsigned long long gcd(unsigned long long a, unsigned long long b)
{
    while (a > 0 && b > 0)
    {
        if (a > b)
            a %= b;
        else
            b %= a;
    }
    return (a > 0) ? a : b;
}

int _has_representation(int base, double num)
{
    unsigned long long numerator, denominator = 1;
    int i;
    while (fabs(num - floor(num)) > EPS && ULLONG_MAX / 10 >= denominator)
    {
        num *= 10;
        denominator *= 10;
    }
    numerator = (unsigned long long)num;
    denominator /= gcd(numerator, denominator);

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

status has_a_finite_representation(int base, double *res, int *len_ans, int count_nums, ...)
{
    *len_ans = 0;
    if (!res)
        return INPUT_ERROR;
    if (base < 2)
        return BASE_ERROR;

    int i;
    status state = SUCCESS;
    double number;
    va_list args;
    va_start(args, count_nums);
    *len_ans = 0;

    for (i = 0; i < count_nums; ++i)
    {
        number = va_arg(args, double);
        if (number > 1. - EPS || number < EPS)
        {
            state = INPUT_ERROR;
            continue;
        }

        if (_has_representation(base, number))
        {
            res[(*len_ans)++] = number;
        }
    }

    va_end(args);
    return state;
}