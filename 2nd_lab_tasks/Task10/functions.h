#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;


// вычисляет значение полинома с заданными коэффицентами в точке a
double calculation_of_polynomial(double *coefs, double a, int max_degree)
{
    double multiplier = 1, result = 0;
    int i;

    for (i = 0; i <= max_degree; ++i)
    {
        result += multiplier * coefs[i];
        multiplier *= a;
    }
    return result;
}

// По Биному Ньютона:
// g0 = f(a)
// g1 = f`(a)
// g2 = f``(a)/2!
// ...
status calculate_coefs_polinom_with_shift(double a, double **coefs, int degree, ...)
{
    if (degree < 0)
        return INPUT_ERROR;

    int i, j, cur_degree, multiply = 1;
    double *start_coefs = (double *)malloc((degree + 1) * sizeof(double));
    *coefs = NULL;
    if (!start_coefs)
        return MEMORY_ERROR;

    double *new_coefs = (double *)malloc((degree + 1) * sizeof(double));
    if (!new_coefs)
    {
        free(start_coefs);
        return MEMORY_ERROR;
    }

    va_list args;
    va_start(args, degree);
    for (i = 0; i <= degree; ++i)
    {
        start_coefs[i] = va_arg(args, double);
    }
    va_end(args);

    cur_degree = degree;
    for (i = 0; i <= degree; ++i)
    {
        new_coefs[i] = calculation_of_polynomial(start_coefs, a, cur_degree);
        if (i)
        {
            multiply *= i;
            new_coefs[i] /= (double)multiply;
        }
        cur_degree--;
        for (j = 0; j <= cur_degree; ++j)
        {
            start_coefs[j] = start_coefs[j + 1] * (j + 1);
        }
    }
    free(start_coefs);
    *coefs = new_coefs;
    return SUCCESS;
}