#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

double PI(double x) {
    return -1 * sin(x);
}

double neg_PI(double x) {
    return sin(x);
}

int dichotomy(double* res, const double eps, double (*func)(double), const double borders[]) {
    if (eps <= 0 || eps > 1) {
        printf("Epsilon must be bigger 0 and less 1");
        return 1;
    }
    if (borders[0] > borders[1]) {
        printf("Incorrect interval");
        return 1;
    }
    double mid, left = borders[0], right = borders[1];
    int is_grow;
    is_grow = (func(left) < func(right)) ? 1 : -1;
    do {
        mid = (left + right) / 2;
        if (is_grow * func(mid) > 0)
            right = mid;
        else
            left = mid;
    } while (fabs(func(mid)) > eps);

    *res = mid;
    return 0;
}

int main(int argc, char* argv[]) {

    double res, borders[] = { 2, 4 };
    if (argc != 2) {
        printf("Incorrect number of arguments");
        return 1;
    }
    if (!strcmp(argv[1], "1")) {
        if (!dichotomy(&res, 0.0001, PI, borders))
            printf("%f", res);
        else
            return 1;
    }
    if (!strcmp(argv[1], "2")) {
        if (!dichotomy(&res, 0.0001, neg_PI, borders))
            printf("%f", res);
        else
            return 1;
    }
    if (!strcmp(argv[1], "3")) {
        if (!dichotomy(&res, -0.0001, PI, borders))
            printf("%f", res);
        else
            return 1;
    }

    return 0;
}