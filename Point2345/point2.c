#include <stdarg.h>
#include <stdio.h>
#include <math.h>

double calculateGeoMean(int count, ...) {
    int product = 1;
    va_list args;

    va_start(args, count);

    for (int i = 0; i < count; i++) {
        product *= va_arg(args, int);
    }

    double geoMean = pow(product, 1.0 / count);

    va_end(args);

    return geoMean;
}

double computePower(double base, int exponent) {
    double result = 1.0;
    int absExponent = (exponent < 0) ? -exponent : exponent;

    while (absExponent > 0) {
        if (absExponent % 2 != 0) {
            result *= base;
        }
        base *= base;
        absExponent /= 2;
    }

    return (exponent < 0) ? 1 / result : result;
}

int main() {
    printf("Geometric mean result: %f\n", calculateGeoMean(3, 4, 3, 6));
    printf("Power calculation result: %f\n", computePower(13.5, 14));
    return 0;
}
