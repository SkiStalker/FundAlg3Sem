#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>


double avg_geom(int cnt, ...) {
    va_list num;
    va_start(num, cnt);
    double total = 1;
    for (int i = 0; i < cnt; ++i) {
        total *= pow(va_arg(num, double), 1.0/cnt);
    }
    va_end(num);
    return total;
}

double fast_pow(const double num, int st) {
    if (st < 2) {
        if (st) {
            return num;
        }
        return 1;
    }
    double res = fast_pow(num, (st / 2));
    res *= res;
    if (st % 2 == 1) {
        res *= num;
    }
    return res;
}

int main() {
    double ans = avg_geom(4, 1.0, 2.0, 3.0, 4.0);
    double res = fast_pow(3.67, 4);
    printf("%.5lf %.5lf\n", ans, res);
    ans = avg_geom(5, 1.0, 2.0, 3.0, 4.0, 7.6);
    res = fast_pow(4.28, 4);
    printf("%.5lf %.5lf\n", ans, res);
    ans = avg_geom(6, 3.8, 1.4, 7.1, 5.7, 7.4, 1.6);
    res = fast_pow(2.17, 5);
    printf("%.5lf %.5lf\n", ans, res);
}