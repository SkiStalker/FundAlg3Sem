#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    BASE_ERROR
} status;

unsigned long long gcd(unsigned long long a, unsigned long long b) {
    unsigned long long tmp;
    while (b != 0) {
        tmp = a;
        a = b;
        b = tmp % b;
    }
    return a;
}

int has_representation(int base, double num) {
    unsigned long long numerator, denominator = 1;

    while (fabs(num - floor(num)) > DBL_EPSILON && ULLONG_MAX / 10 >= denominator)
    {
        num *= 10;
        denominator *= 10;
    }
    numerator = (unsigned long long)num;
    denominator /= gcd(numerator, denominator);

    if (!(denominator & 1) && (base & 1))
        return 0; // если знаменатель четный, а основание нет

    while (!(denominator & 1))
        denominator /= 2;

    for (int i = 3; i <= base; i += 2) {
        while (!(denominator % i)){ // если i делитель знаменателя
            if (base % i){
                return 0; // но не основания
            }
            denominator /= i;
        }
    }

    return (denominator == 1) ? 1 : 0;
}

status has_a_finite_representation(int base, double *res, int *len_ans, int count_nums, ...) {
    if (!res) return INPUT_ERROR;
    if (base < 2) return BASE_ERROR;

    *len_ans = 0;

    status state = SUCCESS;
    double number;
    va_list args;
    va_start(args, count_nums);
    *len_ans = 0;

    for (int i = 0; i < count_nums; ++i) {
        number = va_arg(args, double);
        if (number > 1.0 - DBL_EPSILON || number < DBL_EPSILON) {
            state = INPUT_ERROR;
            continue;
        }

        if (has_representation(base, number)) {
            res[(*len_ans)++] = number;
        }
    }

    va_end(args);
    return state;
}

int main() {
    double res[128];

    int len_ans = 0;
    double a1 = 1.0 / 3.0;
    switch (has_a_finite_representation(2, res, &len_ans, 5, 0.5, a1, 0.25,
                                        0.2, 0.75)) {
        case BASE_ERROR:
            printf("Base error\n");
            return BASE_ERROR;

        case INPUT_ERROR:
            printf("Some numbers are entered incorrectly\n");
            break;
        case SUCCESS:
            if (len_ans) {
                for (int i = 0; i < len_ans; ++i) {
                    printf("Answer: %lf\n", res[i]);
                }
            } else {
                printf("There are no such numbers\n");
            }
            break;
    }
    return 0;
}