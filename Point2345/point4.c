#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point {
    double x;
    double y;
} Point;

double zFunc(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

int isConvex(int count, ...) {
    if (count < 3) {
        return 0;
    }

    va_list args;
    va_start(args, count);

    Point *points = (Point *)malloc(count * sizeof(Point));
    if (points == NULL) {
        printf("Memory allocation error\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        points[i].x = va_arg(args, double);
        points[i].y = va_arg(args, double);
    }

    va_end(args);

    int sign = 0;

    for (int k = 0; k < count; k++) {
        Point a = points[k];
        Point b = points[(k + 1) % count];
        Point c = points[(k + 2) % count];

        double z = zFunc(a, b, c);

        if (z != 0) {
            if (sign == 0) {
                sign = (z > 0) ? 1 : -1;
            } else if ((sign < 0 && z > 0) || (sign > 0 && z < 0)) {
                free(points);
                return 0;
            }
        }
    }

    free(points);
    return 1;
}

double polynomial(double x, int n, ...) {
    va_list args;
    va_start(args, n);

    double sum = 0;

    for (int i = n; i >= 0; i--) {
        sum += va_arg(args, double) * pow(x, i);
    }

    va_end(args);

    return sum;
}

int isValidNumber(const char* str, int base) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (base <= 10) {
            if (c < '0' || c >= '0' + base) {
                return 0;
            }
        } else {
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c < 'A' + (base - 10)) || (c >= 'a' && c < 'a' + (base - 10)))) {
                return 0;
            }
        }
    }

    return 1;
}

long long strToIntBase(const char* str, int base) {
    return strtoll(str, NULL, base);
}

int isKaprekarBase(long long num, int base) {
    long long square = num * num;
    long long basePower = base;

    while (basePower <= square) {
        long long A = square / basePower;
        long long B = square % basePower;

        if (B > 0 && B < basePower && num == A + B) {
            return 1;
        }

        basePower *= base;
    }

    return 0;
}

void findKaprekarNumbersBase(int base, int numArgs, ...) {
    va_list args;
    va_start(args, numArgs);

    for (int i = 0; i < numArgs; i++) {
        const char* numStr = va_arg(args, const char*);
        long long num = strToIntBase(numStr, base);

        if (!isValidNumber(numStr, base)) {
            printf("Error: String \"%s\" is not a valid non-negative integer in base %d.\n", numStr, base);
            continue;
        }

        if (isKaprekarBase(num, base)) {
            printf("The number %s in base %d is a Kaprekar number.\n", numStr, base);
        } else {
            printf("The number %s in base %d is not a Kaprekar number.\n", numStr, base);
        }
    }

    va_end(args);
}

int main() {
    findKaprekarNumbersBase(10, 4, "1", "9", "45", "297");
    findKaprekarNumbersBase(16, 3, "1", "62FCA", "A");
    findKaprekarNumbersBase(12, 1, "9293");

    return 0;
}