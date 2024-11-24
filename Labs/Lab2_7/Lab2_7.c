#include <stdio.h>
#include <math.h>

enum {
    SUCCESS,
    EP_INVALID,
    BORDERS_INVALID
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case EP_INVALID: printf("Epsilon must be positive and less than 1.\n"); break;
        case BORDERS_INVALID: printf("Left border must be less than right.\n"); break;
        default: printf("Unknown error code.\n"); break;
    }
}

typedef double(*callback)(double);

void equation(const double ep, const callback func, double l, double r) {
    double x = (l + r) / 2;
    double pred = 100;
    if (func(r) - func(l) > 0) {
        while (fabs(func(x) - pred) > ep) {
            if (func(x) > 0) {
                r = x;
            } else {
                l = x;
            }
            pred = func(x);
            x = (l + r) / 2;
        }
    } else {
        while (fabs(func(x) - pred) > ep) {
            if (func(x) < 0) {
                r = x;
            } else {
                l = x;
            }
            pred = func(x);
            x = (l + r) / 2;
        }
    }
    printf("%lf\n", l);
}

int solve(const double ep, const callback func, const double l, const double r) {
    if (ep <= 0 || ep >= 1) {
        return EP_INVALID;
    }
    if (l >= r) {
        return BORDERS_INVALID;
    }
    equation(ep, func, l, r);
    return SUCCESS;
}

double funcForEq1 (const double n) {
    const double result = log(n) + 1;
    return result;
}

double funcForEq2 (const double n) {
    const double result = cos(n);
    return result;
}

double funcForEq3 (const double n) {
    const double result = pow(2.7182818284, n) - 7;
    return result;
}

int main() {
    int result = solve(0.000001, &funcForEq1, 0, 7);
    if (result != SUCCESS) {
        ValidateCode(result);
    }
    result = solve(0.001, &funcForEq2, 3, 5);
    if (result != SUCCESS) {
        ValidateCode(result);
    }

    result = solve(0.00001, &funcForEq3, 0, 3);
    if (result != SUCCESS) {
        ValidateCode(result);
    }
    return 0;
}