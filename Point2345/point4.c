#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point {
    double x;
    double y;
} Point;

double crossProduct(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

int checkConvexPolygon(int vertex_count, ...) {
    if (vertex_count < 3) {
        return 0;
    }

    va_list args;
    va_start(args, vertex_count);

    Point *vertices = (Point *)malloc(vertex_count * sizeof(Point));
    if (vertices == NULL) {
        printf("Error: Unable to allocate memory\n");
        return 0;
    }

    for (int i = 0; i < vertex_count; i++) {
        vertices[i].x = va_arg(args, double);
        vertices[i].y = va_arg(args, double);
    }

    va_end(args);

    int initial_sign = 0;

    for (int i = 0; i < vertex_count; i++) {
        Point a = vertices[i];
        Point b = vertices[(i + 1) % vertex_count];
        Point c = vertices[(i + 2) % vertex_count];

        double z = crossProduct(a, b, c);

        if (z != 0) {
            if (initial_sign == 0) {
                initial_sign = (z > 0) ? 1 : -1;
            } else if ((initial_sign < 0 && z > 0) || (initial_sign > 0 && z < 0)) {
                free(vertices);
                return 0;
            }
        }
    }

    free(vertices);
    return 1;
}

double calculatePolynomial(double x, int degree, ...) {
    va_list args;
    va_start(args, degree);

    double result = 0;

    for (int i = degree; i >= 0; i--) {
        result += va_arg(args, double) * pow(x, i);
    }

    va_end(args);

    return result;
}

int validateNumber(const char* num_str, int base) {
    if (num_str == NULL || *num_str == '\0') {
        return 0;
    }

    for (int i = 0; num_str[i] != '\0'; i++) {
        char c = num_str[i];

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

long long convertToBase10(const char* num_str, int base) {
    return strtoll(num_str, NULL, base);
}

int checkKaprekarProperty(long long num, int base) {
    long long square = num * num;
    long long base_power = base;

    while (base_power <= square) {
        long long left_part = square / base_power;
        long long right_part = square % base_power;

        if (right_part > 0 && right_part < base_power && num == left_part + right_part) {
            return 1;
        }

        base_power *= base;
    }

    return 0;
}

void identifyKaprekarNumbers(int base, int total_nums, ...) {
    va_list args;
    va_start(args, total_nums);

    for (int i = 0; i < total_nums; i++) {
        const char* num_str = va_arg(args, const char*);
        long long num = convertToBase10(num_str, base);

        if (!validateNumber(num_str, base)) {
            printf("Error: \"%s\" is not a valid number in base %d.\n", num_str, base);
            continue;
        }

        if (checkKaprekarProperty(num, base)) {
            printf("The number %s in base %d is a Kaprekar number.\n", num_str, base);
        } else {
            printf("The number %s in base %d is not a Kaprekar number.\n", num_str, base);
        }
    }

    va_end(args);
}

int main() {
    printf("Checking Kaprekar numbers in base 10:\n");
    identifyKaprekarNumbers(10, 5, "1", "9", "45", "297", "703");

    printf("\nChecking Kaprekar numbers in base 16:\n");
    identifyKaprekarNumbers(16, 3, "1", "FE", "A");

    printf("\nChecking Kaprekar numbers in base 12:\n");
    identifyKaprekarNumbers(12, 2, "9293", "505");

    return 0;
}