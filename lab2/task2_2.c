#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>

enum ret_type_t {
    SUCCESS,
    NON_POSITIVE_CNT_ERROR,
    NON_POSITIVE_REAL_NUM_ERROR,
};

enum ret_type_t geometric_mean(double* ptd, int cnt, ...) {
    if (cnt < 1) {
        printf("No positive cnt of args");
        return NON_POSITIVE_CNT_ERROR;
    }
    double res = 1;
    va_list factor;
    va_start(factor, cnt);
    for (int i = 0; i < cnt; i++) {
        double cur = va_arg(factor, double);
        if (cur < 0 || fabs(cur) < DBL_EPSILON) {
            printf("No positive real num");
            return NON_POSITIVE_REAL_NUM_ERROR;
        }
        res *= cur;
    }
    va_end(factor);
    *ptd = pow(res, 1. / cnt);
    return SUCCESS;
}

double fast_pow(double x, int p) {
    if (p < 0)
        return fast_pow(1 / x, -p);
    if (p == 0)
        return 1;
    if (p % 2 == 0)
        return fast_pow(x * x, p / 2);
    return x * fast_pow(x * x, (p - 1) / 2);
}

int main(int argc, char* argv[])
{
    double res;
    if (argc != 2) {
        printf("Incorrect num of args");
        return 1;
    }
    if (!strcmp(argv[1], "1")) {
        if (!geometric_mean(&res, 3, 1., 2., 3.))
            printf("%f", res);
        else
            return 1;
    }
    if (!strcmp(argv[1], "2")) {
        if (!geometric_mean(&res, 3, -1., 2., 3.))
            printf("%f", res);
        else
            return 1;
    }

    if (!strcmp(argv[1], "3")) {
        res = fast_pow(-2., -3);
        printf("%f", res);
    }

    if (!strcmp(argv[1], "4")) {
        res = fast_pow(2., 8);
        printf("%f", res);
    }

    return 0;
}