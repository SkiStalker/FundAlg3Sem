#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>


enum return_code {
    SUCCESS,
    BAD_INPUT_ERROR,
    OVERFLOW_ERROR,
    BAD_POINTER_ERROR,
    MEMORY_ALLOC_ERROR,
    WRONG_ARGUMENTS_ERROR,
    NOT_A_DOUBLE_ERROR,
    NOT_AN_INTEGER_ERROR,
};

int is_double_overflow(double to_check) {
    if ((fabs(to_check) > DBL_MAX) || to_check == HUGE_VAL || to_check == -HUGE_VAL || isnan(to_check) || isinf(to_check)) {
        return 1;
    }
    return 0;
}

int str_to_double(const char* str, double* num) {
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
    return SUCCESS;
}

int calculate_a(double eps, double x, double* ans) {
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
    return SUCCESS;
}

int calculate_b(double eps, double x, double* ans) {
    *ans = 0;
    double result = 0, add = 1;
    long count = 0, i = 0;
    do {
        if ((i % 2) == 0) {
            result += add;
        }
        else {
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
    return SUCCESS;
}
int calculate_c(double eps, double x, double* ans) {
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
    return SUCCESS;
}
int calculate_d(double eps, double x, double* ans) {
    *ans = 0;
    double result = 0, add = x * x / 2;
    long count = 2, i = 1;
    if (fabs(x) >= 1) {
        return OVERFLOW_ERROR;
    }
    do {
        if ((i % 2) == 0) {
            result += add;
        }
        else {
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
    return SUCCESS;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Not enough arguments\n");
        return WRONG_ARGUMENTS_ERROR;
    }
    double eps, x;
    int res1 = str_to_double(argv[1], &eps);
    int res2 = str_to_double(argv[2], &x);
    if (res1 != SUCCESS || res2 != SUCCESS) {
        printf("Invalid input\n");
        return WRONG_ARGUMENTS_ERROR;
    }
    if (eps <= 0) {
        printf("Epsilon should be more than 0\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    double a, b, c, d;
    int status;
    status = calculate_a(eps, x, &a);
    if (status == SUCCESS) {
        printf("a: %.15lf\n", a);
    }
    else if (status == OVERFLOW_ERROR) {
        fprintf(stderr, "Overflow occurred while calculating a.\n");
        
    }
    status = calculate_b(eps, x, &b);
    if (status == SUCCESS) {
        printf("b: %.15lf\n", b);
    }
    else if (status == OVERFLOW_ERROR) {
        fprintf(stderr, "Overflow occurred while calculating b.\n");
        
    }
    status = calculate_c(eps, x, &c);
    if (status == SUCCESS) {
        printf("c: %.15lf\n", c);
    }
    else if (status == OVERFLOW_ERROR) {
        fprintf(stderr, "c: Overflow occurred while calculating c.\n");
        
    }
    status = calculate_d(eps, x, &d);
    if (status == SUCCESS) {
        printf("d: %.15lf\n", d);
    }
    else if (status == OVERFLOW_ERROR) {
        fprintf(stderr, "d: Overflow occurred while calculating d.\n");;
    }

    return 0;
}