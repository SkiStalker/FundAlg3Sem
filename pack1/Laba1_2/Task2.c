#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum return_code {
    OK,
    BAD_INPUT_ERROR,
    OVERFLOW_ERROR,
    BAD_POINTER_ERROR,
    MEMORY_ALLOC_ERROR,
    WRONG_ARGUMENTS_ERROR,
    NOT_A_DOUBLE_ERROR,
    NOT_AN_INTEGER_ERROR,
    TOO_SMALL_EPSILON_ERROR,
};


typedef double (*function)(double);

int is_double_overflow(double to_check) {
    if ((fabs(to_check) > DBL_MAX) || to_check == HUGE_VAL || to_check == -HUGE_VAL || isnan(to_check) || isinf(to_check)) {
        return 1;
    }
    return 0;
}

double calculate_harm_number(double number) {
    double sum = 1;
    for (int i = 2; i <= number; i++) {
        sum += 1.0 / i;
    }
    return sum;
}

int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int  str_to_double(const char* str, double* num) {
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
    return OK;
}

int calculate_e_limit(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double current_e = 1, prev_e = 0;
    int i = 1;
    while (fabs(current_e - prev_e) > eps) {
        prev_e = current_e;
        current_e = pow(1 + 1.0 / i, i);
        ++i;
    }

    *result = current_e;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_pi_limit(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double current = 4, prev, add = 4;
    long n = 2;
    do {
        prev = current;
        add *= ((double)n / (2 * n - 1)) * ((double)n / (2 * n - 1)) * ((double)n / (2 * n)) * ((double)n / (2 * n));
        add *= 16;
        current = add / n;
        ++n;
    } while (fabs(current - prev) > eps);

    *result = current;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_ln_limit(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double current = 1, prev;
    long n = 2;
    do {
        prev = current;
        current = (pow(2, 1.0 / n) - 1) * n;
        ++n;
    } while (fabs(current - prev) > eps);
    *result = current;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_sqrt2_limit(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double current = -0.5, prev;
    long n = 1;
    do {
        prev = current;
        current = (current - (current * current / 2) + 1);
        ++n;
    } while (fabs(current - prev) > eps);
    *result = current;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_gamma_limit(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double previous = 0, current = 1;
    long n = 1;
    while (fabs(previous - current) > eps) {
        previous = current;
        n *= 2;
        current = calculate_harm_number(n) - log(n);
    }
    *result = current;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_e_row(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double e = 1.0, add = 1;
    int i = 2;
    while (add > eps) {
        e += add;
        add /= i;
        ++i;
    }
    *result = e;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_pi_row(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double pi = 0, add;
    int i = 1;
    do {
        add = ((double)1) / (2 * i - 1);

        if ((i - 1) % 2 == 0) {
            pi += add;
        }
        else {
            pi -= add;
        }
        ++i;
    } while (add > eps);
    *result = pi * 4;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_ln_row(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double ln = 0, add = 100;
    int i = 1;
    while (fabs(add) > eps) {
        add = ((double)1) / i;

        if ((i - 1) % 2 == 0) {
            ln += add;
        }
        else {
            ln -= add;
        }
        ++i;
    }
    *result = ln;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_sqrt2_row(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double sqrt2 = 1, degree2 = 0.25;
    while (fabs(degree2) > eps) {
        sqrt2 *= pow(2, degree2);
        degree2 *= 0.5;
    }
    *result = sqrt2;
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_gamma_row(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    eps = eps / 100000;
    double sigma = 0, add;
    long k = 2, sqrt_floor;
    do {
        sqrt_floor = (long)(sqrt(k));
        if (sqrt_floor * sqrt_floor == k) {
            ++k;
            sqrt_floor = (long)sqrt(k);
        }
        add = (1.0 / (sqrt_floor * sqrt_floor)) - (1.0 / k);
        sigma += add;
        ++k;
    } while (fabs(add) > eps);

    double pi;
    calculate_pi_row(eps, &pi);
    *result = sigma - (pi * pi / 6);
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

double calculate_equation_by_func(double left, double right, double equals, function f, double eps) {
    double middle = (left + right) / 2;
    double res = f(middle);

    if (fabs(res - equals) < eps) {
        return middle;
    }
    else if (res > equals) {
        return calculate_equation_by_func(left, middle, equals, f, eps);
    }
    else {
        return calculate_equation_by_func(middle, right, equals, f, eps);
    }
}

int calculate_e_equation(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    *result = calculate_equation_by_func(1, 3, 1, &log, eps);
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_pi_equation(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    double f;
    double x = 3;
    double df;

    f = cos(x) + 1.0;
    while (fabs(f) > eps) {
        df = -sin(x);
        x = x - f / df;
        f = cos(x) + 1.0;
    }
    *result = x;
    if (isnan(x)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_ln_equation(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    *result = calculate_equation_by_func(0, 1, 2, &exp, eps);
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

double square_function(double x) {
    return x * x;
}

int calculate_sqrt2_equation(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    *result = calculate_equation_by_func(1, 3, 2, &square_function, eps);
    if (isnan(*result)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

int calculate_gamma_equation(double eps, double* result) {
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (eps <= 0) {
        return BAD_INPUT_ERROR;
    }
    int p = 2;
    double current = log(2) * 0.5, previous = 0, product = 0.5;
    do
    {
        previous = current;
        do
        {
            p++;
        } while (!is_prime(p));
        product *= (p - 1.0) / p;
        current = log(p) * product;


    } while (fabs(previous - current) >= eps);
    *result = -log(current);
    if (isnan(current)) {
        return TOO_SMALL_EPSILON_ERROR;
    }
    return OK;
}

void get_results(double eps, double results[15]) {
    calculate_e_limit(eps, &results[0]);
    calculate_pi_limit(eps, &results[1]);
    calculate_ln_limit(eps, &results[2]);
    calculate_sqrt2_limit(eps, &results[3]);
    calculate_gamma_limit(eps, &results[4]);

    calculate_e_row(eps, &results[5]);
    calculate_pi_row(eps, &results[6]);
    calculate_ln_row(eps, &results[7]);
    calculate_sqrt2_row(eps, &results[8]);
    calculate_gamma_row(eps, &results[9]);

    calculate_e_equation(eps, &results[10]);
    calculate_pi_equation(eps, &results[11]);
    calculate_ln_equation(eps, &results[12]);
    calculate_sqrt2_equation(eps, &results[13]);
    calculate_gamma_equation(eps, &results[14]);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Not enough arguments\n");
        return -1;
    }
    double eps;
    int res1 = str_to_double(argv[1], &eps);

    if (res1 != OK) {
        printf("Invalid input\n");
        return WRONG_ARGUMENTS_ERROR;
    }

    if (eps <= 0) {
        printf("Epsilon should be positive\n");
        return -1;
    }

    double results[15];
    get_results(eps, results);
    char* constants[5] = { "e", "pi", "ln2", "sqrt2", "gamma" };
    printf("Calculation using limits\n");
    for (int i = 0; i < 5; ++i) {
        if (isnan(results[i])) {
            printf("%s Impossible to calculate with a specific epsilon\n", constants[i % 5]);
            continue;
        }

        printf("%s = %lf\n", constants[i % 5], results[i]);
    }
    printf("\n");

    printf("Calculation using rows\n");
    for (int i = 5; i < 10; ++i) {
        if (isnan(results[i])) {
            printf("%s Impossible to calculate with a specific epsilon\n", constants[i % 5]);
            continue;
        }
        printf("%s = %lf\n", constants[i % 5], results[i]);
    }
    printf("\n");

    printf("Calculation using equations\n");
    for (int i = 10; i < 15; ++i) {
        if (isnan(results[i])) {
            printf("%s Impossible to calculate with a specific epsilon\n", constants[i % 5]);
            continue;
        }

        printf("%s = %lf\n", constants[i % 5], results[i]);
    }
    printf("\n");
}