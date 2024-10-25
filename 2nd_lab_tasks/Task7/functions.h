#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum Status
{
    SUCCESS,
    NO_ROOTS,
    INPUT_ERROR
} status;


status dichotomy_method(double *res, double left, double right, double eps, double (*f)(double))
{
    if (eps <= 0.)
        return INPUT_ERROR;
    double fl = f(left);
    if (fl * f(right) > 0.)
        return NO_ROOTS;
    double mid;

    while (fabs(right - left) >= eps)
    {
        mid = (left + right) / 2;
        if (fl * f(mid) <= 0.)
            right = mid;
        else
            left = mid;
    }
    *res = (right + left) / 2;
    return SUCCESS;
}

double func1(double x)
{
    return x - 11.2232;
}

double func2(double x)
{
    return sin(x) - 1 / x;
}

double func3(double x)
{
    return x * x + 10 * x - 4;
}

double func4(double x)
{
    return x + x * x + 3 * x * x * x + 1;
}

double func5(double x)
{
    return sqrt(x) - 4.4;
}
