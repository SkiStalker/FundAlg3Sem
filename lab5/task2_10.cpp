#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

enum ret_type_t {
    SUCCESS,
    ERROR_MALLOC,
    INCOR_EPS,
    INCOR_CNT
};

void print_err(enum ret_type_t res) {
    switch (res) {
    case 1:
        printf("Memory allocation error");
        break;
    case 2:
        printf("Epsilone must be positive");
        break;
    case 3:
        printf("Numbers count must be positive");
        break;
    }
}

long long factorial(long long x) {
    long long res = 1, i;
    for (i = 1; i <= x; i++)
        res *= i;
    return res;
}

double derivative(long long n, double a, double* arr) {
    long long i;
    double res = 0;
    for (i = 0; i < n - 1; i++) {
        res += arr[i] * pow(a, i);
        arr[i] = (i + 1) * arr[i + 1];
    }
    res += arr[i] * pow(a, i);
    arr[i] = 0;
    return res;
}

enum ret_type_t expansion(double eps, double a, double** res_arr, long long n, ...) {
    if (n < 1)
        return INCOR_CNT;
    if (eps < 0 || fabs(eps) < DBL_MIN)
        return INCOR_EPS;
    double* input_arr = (double*)malloc(n * sizeof(double));
    if (!input_arr) {
        free(input_arr);
        return ERROR_MALLOC;
    }
    *res_arr = (double*)malloc(n * sizeof(double));
    if (!(*res_arr)) {
        free(*res_arr);
        return ERROR_MALLOC;
    }
    if (!(*res_arr))
        return ERROR_MALLOC;
    va_list factor;
    va_start(factor, n);
    long long i;
    for (i = 0; i < n; i++)
        input_arr[i] = va_arg(factor, double);
    va_end(factor);
    for (i = 0; i < n; i++) {
        double cur_coeff = derivative(n - i, a, input_arr) / factorial(i);
        cur_coeff = fabs(cur_coeff) < eps ? 0. : cur_coeff;
        (*res_arr)[i] = cur_coeff;
    }
    free(input_arr);
    return SUCCESS;
}

void print_arr(long long n, double a, double* arr) {
    for (long long i = 0; i < n; i++) {
        if (!i)
            printf("%lf", arr[i]);
        else {
            if (arr[i] > 0)
                printf(" +");
            printf(" %lf*(x - a)", arr[i]);
            if (i != 1)
                printf("^%lld", i);
        }
    }
}

int main() {

    /*
    f(x) = x^4 - 3x^2 + x - 2 : {-2, 1, -3, 0, 1}
    f'(x) = 4x^3 - 6x + 1
    f''(x) = 12x^2 - 6
    f'''(x) = 24x
    f''''(x) = 24
    g(x) = 55 + 91(x - 3) + 51(x - 3)^2 + 12(x - 3)^3 + (x - 3)^4
    */

    double* res_arr = NULL;
    enum ret_type_t res = expansion(0.001, 3., &res_arr, 5, -2., 1., -3., 0., 1.);
    if (!res)
        print_arr(5, 3., res_arr);
    else
        print_err(res);
    free(res_arr);

    return 0;
}
