#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

enum ret_type_t {
    SUCCESS,
    ERROR
};

bool is_number(const char *str) {
    if (*str == '-') {
        str++;
    }

    bool hasDot = false;
    if (*str == '\0') return false;

    while (*str) {
        if (*str == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

double e_limit(double eps) {
    double e_1 = 0.0, e_2 = 1.0;
    int n = 1;
    while (fabs(e_2 - e_1) > eps) {
        e_1 = e_2;
        e_2 = pow((1 + 1.0 / n), n);
        n++;
    }
    return e_2;
}

long long factorial(int n) {
    long long fact = 1;
    for (int i = 2; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

double e_series_sum(double eps) {
    double e_summa = 1.0, e_1 = 1.0;
    int n = 1;
    while (e_1 > eps) {
        e_1 = 1.0 / factorial(n);
        e_summa += e_1;
        n++;
    }
    return e_summa;
}

double e_equation(double eps) {
    double x = 2.5;
    double f, df;
    while (1) {
        f = logf(x) - 1;
        df = 1 / x;
        double next = x - f / df;
        if (fabs(next - x) < eps) {
            break;
        }
        x = next;
    }
    return x;
}

double pi_limit(double eps) {
    int n = 2;
    double pi_1 = 4.0, pi_2 = 0;
    do {
        pi_2 = pi_1;
        pi_1 *= (4.0 * n * (n - 1.0)) / pow(2.0 * n - 1.0, 2);
        n++;
    } while (fabs(pi_2 - pi_1) >= eps);
    return pi_1;
}
double pi_series_sum(double eps) {
    int n = 1;
    double temp, pi_1 = 0.0;
    do {
        temp = pow(-1.0, n - 1) / (2 * n - 1);
        pi_1 += temp;
        n += 1;
    } while (fabs(temp) > eps);
    return 4 * pi_1;
}


double pi_equation(double eps) {
    double x = 2.5;
    double f, df;
    while (1) {
        f = cos(x) + 1;
        df = -sin(x);
        double next = x - f / df;
        if (fabs(next - x) < eps) {
            break;
        }
        x = next;
    }
    return x;
}

double ln_limit(double eps) {
    double ln_1 = 0.0, ln_2 = 0.0;
    int n = 1;
    do {
        ln_1 = ln_2;
        ln_2 = n * (pow(2.0, 1.0 / n) - 1.0);
        n++;
    } while (fabs(ln_2 - ln_1) > eps);

    return ln_2;
}

double ln_series_sum(double eps) {
    double summa = 0.0;
    double e_1 = 1.0;
    int n = 1;
    while (fabs(e_1) > eps) {
        e_1 = pow(-1.0, n - 1) / n;
        summa += e_1;
        n++;
    }

    return summa;
}



double ln_equation(double eps) {
    double x = 2.5;
    double f, df;
    while (1) {
        f = exp(x) - 2;
        df = exp(x);
        double next = x - f / df;
        if (fabs(next - x) < eps) {
            break;
        }
        x = next;
    }
    return x;
}

double sqrt2_limit(double eps) {
    double x_n = 1.0, x_n1;
    do {
        x_n1 = x_n - (pow(x_n,2) / 2) + 1;
        if (fabs(x_n1 - x_n) < eps) break;
        x_n = x_n1;
    } while (1);
    return x_n1;
}


double sqrt2_series_sum(double eps) {
    double sqrt_1 = 1.0, sqrt_2, step, temp;
    int k = 2;
    do {
        sqrt_2 = sqrt_1;
        step = pow(2.0, -k);
        temp = pow(2.0, step);
        sqrt_1 *= temp;
        k++;

    } while (fabs(sqrt_1 - sqrt_2) > eps);

    return sqrt_1;
}


double sqrt2_equation(double eps) {
    double x = 2.5;
    double f, df;
    while (1) {
        f = pow(x, 2) - 2;
        df = 2 * x;
        double next = x - f / df;
        if (fabs(next - x) < eps) {
            break;
        }
        x = next;
    }
    return x;
}


double gamma_limit(double eps) {
    double summa = 0.0;
    double gamma_1 = 0.0;
    double gamma_2 = 0.0;
    int n = 1;
    do {
        summa += 1.0 / n;
        gamma_1 = gamma_2;
        gamma_2 = summa - log(n);
        n++;
    } while (fabs(gamma_2 - gamma_1) > eps);

    return gamma_2;
}


double gamma_series_sum(double eps) {
    double gamma = 0.0;
    double term;
    int k = 1;
    do {
        term = (1.0 / k) - log(1.0 + 1.0 / k);
        gamma += term;
        k++;
    } while (fabs(term) > eps);

    return gamma;
}


int is_prime(int x) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if (x % 2 == 0) return 0;
    for (int i = 3; i <= floor(sqrt(x)); i += 2) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
}


double gamma_equation(double eps) {
    int p = 2;
    double tek = log(2) * 0.5;
    double prev = 0;
    double proiz = 0.5;
    do {
        prev = tek;
        do {
            p++;
        } while (!is_prime(p));
        proiz *= (p - 1.0) / p;
        tek = log(p) * proiz;
    } while (fabs(prev - tek) >= eps);
    return -log(tek);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect args count\n");
        return ERROR;
    }
    if (!is_number(argv[1])){
        printf("Is not a number\n");
        return ERROR;
    }
    else{
        double eps = atof(argv[1]);
        if (eps <= 0) {
            printf("Must be positive numbers\n");
            return ERROR;
        }
        printf("e limit: %.15f\n", e_limit(eps));
        printf("e series sum: %.15f\n", e_series_sum(eps));
        printf("e equation: %.15f\n", e_equation(eps));

        printf("pi limit: %.15f\n", pi_limit(eps));
        printf("pi series sum: %.15f\n", pi_series_sum(eps));
        printf("pi equation: %.15f\n", pi_equation(eps));

        printf("ln 2 limit: %.15f\n", ln_limit(eps));
        printf("ln 2 series sum: %.15f\n", ln_series_sum(eps));
        printf("ln 2 equation: %.15f\n", ln_equation(eps));

        printf("sqrt2 limit: %.15f\n", sqrt2_limit(eps));
        printf("sqrt2 series sum: %.15f\n", sqrt2_series_sum(eps));
        printf("sqrt2 equation: %.15f\n", sqrt2_equation(eps));

        printf("gamma limit: %.15f\n", gamma_limit(eps));
        printf("gamma series sum: %.15f\n", gamma_series_sum(eps));
        printf("gamma equation: %.15f\n", gamma_equation(eps));
    }


    return SUCCESS;
}
