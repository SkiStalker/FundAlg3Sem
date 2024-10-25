#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

status is_overflow(double x)
{
    if (isnan(x) || isinf(x))
        return OVERFLOW_ERROR;
    return SUCCESS;
}

status geometric_mean(double *res, int count, ...)
{
    if (count < 1)
        return INPUT_ERROR;

    va_list args;
    va_start(args, count);
    *res = 1;
    double cur;
    for (int i = 0; i < count; ++i)
    {
        cur = va_arg(args, double);
        if (cur < 0)
            return INPUT_ERROR;
        *res *= cur;
        if (is_overflow(*res))
        {
            va_end(args);
            return OVERFLOW_ERROR;
        }
    }
    *res = pow(*res, (double)1.0 / count);
    va_end(args);
    return SUCCESS;
}

status fast_double_pow(double base, int degree, double *res)
{
    status state = SUCCESS;
    if (degree < 0)
        return fast_double_pow(1.0 / base, -degree, res);

    else if (!degree)
    {
        *res = 1.0;
        state = SUCCESS;
    }

    else if (degree & 1)
    {
        state = fast_double_pow(base, degree - 1, res);
        *res *= base;
        return is_overflow(*res);
    }
    else
    {
        state = fast_double_pow(base, degree / 2, res);
        *res *= *res;
        return is_overflow(*res);
    }
    return state;
}

