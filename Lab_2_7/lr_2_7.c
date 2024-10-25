#include <stdio.h>
#include <math.h>

enum {
    SUCCESS,
    ERROR_EPS_OUT_OF_RANGE,
    ERROR_BORDERS_INVALID
} ret_type_t;

typedef double(*callback)(double);

void HandlingError(int code) {
    switch (code) {
        case ERROR_EPS_OUT_OF_RANGE:
            printf("Epsilon must be positive and less than 1.\n");
            break;
        case ERROR_BORDERS_INVALID:
            printf("Left border can not be more than right.\n");
            break;
        default: printf("Unknown error code.\n"); break;
    }
}

void Equation(const double eps, callback func, double l, double r) {
    double x = (l + r) / 2;
    double prev = 100;

    if ((func(r) - func(l)) > 0) {
        while (fabs(func(x) - prev) > eps) {
            if (func(x) > 0) {
                r = x;
            } else {
                l = x;
            }
            prev = func(x);
            x = (l + r) / 2;
        }
    } else {
        while (fabs(func(x) - prev) > eps) {
            if (func(x) < 0) {
                r = x;
            } else {
                l = x;
            }
            prev = func(x);
            x = (l + r) / 2;
        }
    }
    printf("Answer ===> %lf\n", l);
}

int solve(const double eps, const callback func, double l, double r) {
    if (eps <= 0 || eps >= 1) {
        return ERROR_EPS_OUT_OF_RANGE;
    }
    if (l >= r) {
        return ERROR_BORDERS_INVALID;
    }
    Equation(eps, func, l, r);
    return SUCCESS;
}

double EEqFunc(double n) {
    double res = log(n) - 1;
    return res;
}

double PiEqFunc(double n) {
    double res = sin(n);
    return res;
}

double lnEqFunc(double n) {
    double res = pow(2.7182818284, n) - 2;
    return res;
}

int main() {
    int code;
    code = solve(0.00001, &EEqFunc, 1, 100);
    if (code != SUCCESS) {
        HandlingError(code);
    }

    code = solve(0.00001, &PiEqFunc, 2, 4);
    if (code != SUCCESS) {
        HandlingError(code);
    }

    code = solve(0.00001, &lnEqFunc, 0, 100);
    if (code != SUCCESS) {
        HandlingError(code);
    }
}