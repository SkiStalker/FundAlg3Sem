#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum return_code {
    OK = 0,
    BAD_INPUT_ERROR = 1,
    OVERFLOW_ERROR = 2,
    BAD_POINTER_ERROR = 3,
    MEMORY_ALLOC_ERROR = 4,
    WRONG_ARGUMENTS_ERROR = 5,
    NOT_A_DOUBLE_ERROR = 6,
    NOT_AN_INTEGER_ERROR = 7,
};

int is_double_overflow(double to_check) {
    if ((fabs(to_check) > DBL_MAX) || to_check == HUGE_VAL || to_check == -HUGE_VAL || isnan(to_check) || isinf(to_check)) {
        return 1;
    }
    return 0;
}

int transfer_to_double(const char* str, double* num) {
    if (!str || !num) {
        return BAD_POINTER_ERROR;
    }
    char* endptr;
    double value = strtod(str, &endptr);

    if (*endptr != '\0') {
        return NOT_A_DOUBLE_ERROR;
    }
    if (is_double_overflow(value)) {
        return OVERFLOW_ERROR;
    }
    *num = value;
    return 0;
}

int calculate_a(double eps, double x, double *ans) {
    *ans = 0;
    double result = 0, add = 1;
    long count = 0;
    do {
        result += add;
        ++count;
        add *= (x / count);
        if (is_double_overflow(add) || is_double_overflow(result)) {
            return OVERFLOW_ERROR;
        }

    } while (fabs(add) > eps);
    *ans = result;
    return 0;
}

double normalize_x(double x) {
    double two_pi = 8 * atan(1);
    return fmod(fabs(x), two_pi);
}

int calculate_b(double eps, double x, double *ans) {
    *ans = 0;
    double result = 0, add = 1;
    long count = 0, i = 0;
    do {
        if ((i % 2) == 0) {
            result += add;
        } else {
            result -= add;
        }
        add *= ((x / (count + 1)) * (x / (count + 2)));
        if (is_double_overflow(add) || is_double_overflow(result)) {
            return OVERFLOW_ERROR;
        }
        count += 2;
        ++i;

    } while (fabs(add) > eps);
    *ans = result;
    return 0;
}

int calculate_c(double eps, double x, double *ans) {
    *ans = 0;
    double add = 1, result = 0;
    long count1 = 0, count3 = 0;

    do {
        result += add;

        add /= (((count3 + 1) * (count3 + 2) * (count3 + 3)));
        add *= (count1 + 1);

        add *= (x * x * 27);
        count3 += 3;

        ++count1;
        add *= (count1 * count1);
        if (is_double_overflow(add) || is_double_overflow(result)) {
            return OVERFLOW_ERROR;
        }
    } while (fabs(add) > eps);
    *ans = result;
    return 0;
}

int calculate_d(double eps, double x, double *ans) {
    *ans = 0;
    double result = 0, add = x * x / 2;
    long count = 2, i = 1;
    if (fabs(x) >= 1) {
        return OVERFLOW_ERROR;
    }
    do {
        if ((i % 2) == 0) {
            result += add;
        } else {
            result -= add;
        }
        count += 2;

        add /= (count);
        add *= (count - 1);
        add *= (x * x);

        if (is_double_overflow(add) || is_double_overflow(result)) {
            return OVERFLOW_ERROR;
        }
        ++i;
    } while (fabs(add) > eps);
    *ans = result;
    return 0;
}

int print_result(int result_func, double ans, char point) {
    if (result_func == 0) {
        printf("%c. Result: %.15lf\n", point, ans);
    } else if (result_func == OVERFLOW_ERROR) {
        printf("%c. Result: Расходится\n", point);
    } else {
        printf("%c. Error occurred: code %d\n", point, result_func);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: Not enough arguments. Please provide epsilon and x.\n");
        return -1;
    }
    double eps, x;
    int res1 = transfer_to_double(argv[1], &eps);
    int res2 = transfer_to_double(argv[2], &x);

    if (eps <= 0) {
        printf("Error: Epsilon should be positive.\n");
        return -1;
    }

    if (res1 != OK || res2 != OK) {
        printf("Error: Invalid input. Please enter valid numeric values for epsilon and x.\n");
        return WRONG_ARGUMENTS_ERROR;
    }
    double a, b, c, d;
    double res_a, res_b, res_c, res_d;

    res_a = calculate_a(eps, x, &a);
    res_b = calculate_b(eps, x, &b);
    res_c = calculate_c(eps, x, &c);
    res_d = calculate_d(eps, x, &d);

    print_result(res_a, a, 'a');
    print_result(res_b, b, 'b');
    print_result(res_c, c, 'c');
    print_result(res_d, d, 'd');


    printf("Success: Calculations completed for epsilon = %.10lf and x = %.10lf\n", eps, x);
    return 0;
}
