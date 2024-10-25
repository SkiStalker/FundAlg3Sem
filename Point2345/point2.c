#include <stdarg.h>
#include <stdio.h>
#include <math.h>

double midGeo(int count, ...) {
    int mult = 1;
    va_list args;

    va_start(args, count);

    for (int i = 0; i < count; i++) {
        mult *= va_arg(args, int);
    }

    double result = pow(mult, 1.0 / count);

    va_end(args);

    return result;
}

double fastPow(double x, int n) {
    double result = 1.0;
    int abs_n = 0;
 
    if (n < 0) {
        abs_n = -n;
    } else {
        abs_n = n;
    }

    while (abs_n > 0) {
        if (abs_n % 2 != 0) {
            result *= x;
        }
        x *= x;
        abs_n /= 2;
    }

    if (n < 0) {
        return 1 / result;
    } else {
        return result;
    }
}

int main() {
    printf("%f\n", midGeo(3, 4, 3, 6));
    printf("%f\n", fastPow(13.5, 14));
    return 0;
}