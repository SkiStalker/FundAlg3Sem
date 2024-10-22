#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

typedef enum {
    SUCCESS,
    ERROR_INVALID_ARGUMENTS,
    ERROR_NOT_CONVEX,
    ERROR_INVALID_BASE,
    ERROR_MEMORY,
    ERROR_INVALID_NUMBER
} ErrorCode;


ErrorCode is_convex_polygon(int num_points, int* is_convex, ...) {
    if (num_points < 3) {
        return ERROR_INVALID_ARGUMENTS;
    }

    va_list args;
    va_start(args, is_convex);

    // Динамически выделяем память для массива точек
    double* points = malloc(2 * num_points * sizeof(double));
    if (points == NULL) {
        return ERROR_MEMORY;
    }

    for (int i = 0; i < num_points; i++) {
        points[i * 2] = va_arg(args, double);
        points[i * 2 + 1] = va_arg(args, double);
    }
    va_end(args);

    double z_cross_product = 0;
    for (int i = 0; i < num_points; i++) {
        double x1 = points[(i % num_points) * 2];
        double y1 = points[(i % num_points) * 2 + 1];
        double x2 = points[((i + 1) % num_points) * 2];
        double y2 = points[((i + 1) % num_points) * 2 + 1];
        double x3 = points[((i + 2) % num_points) * 2];
        double y3 = points[((i + 2) % num_points) * 2 + 1];

        double cross_product = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);

        if (i == 0) {
            z_cross_product = cross_product;
        }
        else if ((z_cross_product > 0 && cross_product < 0) || (z_cross_product < 0 && cross_product > 0)) {
            *is_convex = 0; // Не выпуклый
            free(points);
            return SUCCESS;
        }
    }
    *is_convex = 1; // Выпуклый
    free(points); // Освобождаем память
    return SUCCESS;
}


ErrorCode polynomial_value(double point, double* result, int degree, ...) {
    if (degree < 0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    va_list args;
    va_start(args, degree);

    *result = va_arg(args, double);
    for (int i = 1; i <= degree; i++) {
        double coefficient = va_arg(args, double);
        *result = *result * point + coefficient;
    }
    va_end(args);

    return SUCCESS;
}

int isKaprekar(long long num) {
    if (num == 0) return 0; 

    long long square = num * num;
    int d = 0, temp = num;
    while (temp > 0) {
        temp /= 10;
        d++;
    }

    long long right = square % (long long)pow(10, d);
    long long left = square / (long long)pow(10, d);

    return (left + right == num);
}


ErrorCode find_kaprekar_numbers(int base, int count, int* result_count, char*** kaprekar_numbers, ...) {
    if (base < 2 || base > 36) {
        return ERROR_INVALID_BASE;
    }

    *kaprekar_numbers = malloc(count * sizeof(char*));
    if (*kaprekar_numbers == NULL) {
        return ERROR_INVALID_NUMBER; 
    }

    va_list args;
    va_start(args, kaprekar_numbers);

    *result_count = 0;

    for (int i = 0; i < count; i++) {
        const char* str = va_arg(args, const char*);
        char* endptr;
        long long num = strtoll(str, &endptr, base);
        if (*endptr != '\0') {
            free(*kaprekar_numbers);
            return ERROR_INVALID_NUMBER;
        }
        if (isKaprekar(num)) {
            (*kaprekar_numbers)[*result_count] = malloc(strlen(str) + 1);
            if ((*kaprekar_numbers)[*result_count] == NULL) {
                free(*kaprekar_numbers);
                return ERROR_INVALID_NUMBER;
            }
            strcpy((*kaprekar_numbers)[*result_count], str);
            (*result_count)++;
        }
    }

    va_end(args);
    *kaprekar_numbers = realloc(*kaprekar_numbers, (*result_count) * sizeof(char*));

    return SUCCESS;
}




int main() {
    int is_convex;
    ErrorCode err = is_convex_polygon(3, &is_convex, 0.0, 0.0, 2.0, 2.0, 1.0, 1.0);
    if (err == SUCCESS) {
        printf("Polygon is %s.\n", is_convex ? "convex" : "not convex");
    }
    else {
        printf("Error: %d\n", err);
    }
    double result;
    double coefficients[] = { 2.0, 3.0, 1.0 }; // 2x^2 + 3x + 1
    err = polynomial_value(1.0, &result, 2, coefficients[0], coefficients[1], coefficients[2]);
    if (err == SUCCESS) {
        printf("Polynomial value at x=1: %.2lf\n", result);
    }
    else {
        printf("Error: %d\n", err);
    }
    char** kaprekar_numbers;
    int count;
    err = find_kaprekar_numbers(10, 3, &count, &kaprekar_numbers, "294", "9", "45");

    if (err == SUCCESS) {
        printf("Kaprekar numbers:\n");
        for (int i = 0; i < count; i++) {
            printf("%s\n", kaprekar_numbers[i]);
            free(kaprekar_numbers[i]); 
        }
        free(kaprekar_numbers); 
    }
    else {
        printf("Error: %d\n", err);
    }


    return 0;
}
