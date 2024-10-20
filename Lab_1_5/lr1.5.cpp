#include <stdio.h>
#include <math.h>
#include <ctype.h>

enum ret_type_t {
    SUCCESS,
    ERROR_NO_VALUE,
    ERROR_ZERO_VAL,
    ERROR_NOT_NUMBER,
    ERROR_NEGATIVE_VALUE,
    ERROR_TOO_LONG_STR,
    ERROR_NUMBER_OUT_OF_RANGE,
    ERROR_WRONG_NUMB_OF_ARGS,
    ERROR_ARG_IS_TOO_BIG
};

typedef double(*callback)(double, int);

ret_type_t isnt_zero(const char* s) {
    int is_zero = 0;
    while (*s == ' ') s++;
    if (*s == '-') {
        return ERROR_NEGATIVE_VALUE;
    }
    while (isdigit(*s) || (*s == '.')) {
        if (*s != '0' && *s != '.') {
            is_zero = 1;
        }
        ++s;
    }
    if (!is_zero && *s == '\0') {
        return ERROR_ZERO_VAL;
    }
    return SUCCESS;
}

ret_type_t is_number(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;
    while (*s == ' ') s++;
    int len = 0;
    int point_was = 0;
    while (isdigit(*s) || (*s == '.' && !point_was) || (*s == '-')) {
        if (*s == '.') point_was = 1;
        len++;
        s++;
        if (len > 10) {
            return ERROR_TOO_LONG_STR;
        }
    }
    if (*s == '\0') return SUCCESS;
    return ERROR_NOT_NUMBER;
}

void HandlingError(int code) {
    switch (code) {
        case ERROR_NO_VALUE:
            printf("No number was entered.\n");
            break;
        case ERROR_NOT_NUMBER:
            printf("Value is not a number.\n");
            break;
        case ERROR_NEGATIVE_VALUE:
            printf("Epsilon can not be negative.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case ERROR_NUMBER_OUT_OF_RANGE:
            printf("Epsilon is too big.\n");
            break;
        case ERROR_ARG_IS_TOO_BIG:
            printf("The function diverges.\n");
            break;
        case ERROR_ZERO_VAL:
            printf("Epsilon is zero.\n");
            break;
        default:
            break;
    }
}

double Pow(const double x, const int n) {
    double res = 1;

    if(n == 0) {
        res = 1.0;
        return res;
    }

    for(int _ = 0; _ < abs(n); ++_) {
        res *= x;
    }
    if (n < 0) {
        res = 1.0 / res;
    }
    return res;
}

ret_type_t Row(const double eps, callback RowFunc, const double x, const int n, const double start) {
    int m = n;
    double cur_delta = start;
    double summ = cur_delta;
    int iter_cnt = 0;
    do {
        m += 1;
        cur_delta = cur_delta * RowFunc(x, m);
        summ = summ + cur_delta;
        if (iter_cnt > 10000) {
            return ERROR_ARG_IS_TOO_BIG;
        }
        iter_cnt++;
    } while (fabs(cur_delta) > eps);
    printf("Answer ====> %.20f\n", summ);
    return SUCCESS;
}

double Afunc(const double x, const int n) {
    double res;
    res = x / (double)(n);
    return  res;
}

double Bfunc(const double x, const int n) {
    double res;
    res = -(x * x / (2*n - 1) / (2*n));
    return res;
}

double Cfunc(const double x, const int n) {
    double res;
    res = (((9 * Pow(n, 3) * x * x /(3*n - 1)) / (3*n - 2)) / n);
    return res;
}

double Dfunc(const double x, const int n) {
    double res;
    res = (-(2 * n - 1) * x * x / (2 * n));
    if (n % 2 == 1) {
        res = -res;
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments.\n");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }
    ret_type_t code;
    for (int i = 1; i <= 2; ++i) {
        if ((code = is_number(argv[i]))) {
            HandlingError(code);
            return code;
        }
    }
    if ((code = isnt_zero(argv[1]))) {
        HandlingError(code);
        return code;
    }
    double eps = atof(argv[1]);

    if (eps > 0.2) {
        HandlingError(ERROR_NUMBER_OUT_OF_RANGE);
        return ERROR_NUMBER_OUT_OF_RANGE;
    }

    if ((code = is_number(argv[2]))) {
        HandlingError(code);
        return code;
    }
    double x = atof(argv[2]);

    callback funcs[] = {&Afunc, &Bfunc, &Cfunc, &Dfunc};
    const int ns[] = {0, 0, 0, 1};
    const double starts[] = {1, 1, 1, (-x * x / 2)};

    for (int i = 0; i < 4; ++i) {
        HandlingError(Row(eps, funcs[i], x, ns[i], starts[i]));
    }
    return SUCCESS;
}