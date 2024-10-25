#include <stdio.h>
#include <stdarg.h>
#include <math.h>


double avg_geom(int argc, ...) {
    va_list numb;
    va_start(numb, argc);
    double total = 1;
    for (int i = 0; i < argc; ++i) {
        total *= pow(va_arg(numb, double), 1.0/argc);
    }
    va_end(numb);
    return total;
}

double fast_pow(const double numb, int power) {
    if (power < 2) {
        if (power) {
            return numb;
        }
        return 1;
    }
    double res = fast_pow(numb, (power/2));
    res = res * res;
    if (power % 2 == 1) {
        res = res * numb;
    }
    return res;
}

int main() {
    double ans = avg_geom(3, 2.0, 4.0, 3.0);
    printf("%.10lf\n", ans);
    ans = avg_geom(3, 1.2, 5.2, 3.22);
    printf("%.10lf\n", ans);
    ans = avg_geom(3, 0.221, 4.53, 1.2345);
    printf("%.10lf\n", ans);
    double res = fast_pow(3.14, 6);
    printf("%.10lf\n", res);
    res = fast_pow(1.2345, 33);
    printf("%.10lf\n", res);
    res = fast_pow(21.14, 3);
    printf("%.10lf", res);
}
