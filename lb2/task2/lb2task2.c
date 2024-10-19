#include <stdio.h>
#include <stdarg.h>
#include <math.h>

typedef enum {
    SUCCESS,
    ERROR_INVALID_ARGUMENTS,
    ERROR_NO_NUMBERS
} ErrorCode;


ErrorCode geometric_mean(double* result, int num_count, ...) {
    if (num_count <= 0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    va_list args;
    va_start(args, num_count); 

    double product = 1.0;
    for (int i = 0; i < num_count; i++) {
        double value = va_arg(args, double);
        if (value <= 0) {
            va_end(args); 
            return ERROR_NO_NUMBERS;
        }
        product *= value;
    }
    va_end(args);

    *result = pow(product, 1.0 / num_count);
    return SUCCESS;
}


double power(double base, int exponent) {
    if (exponent < 0) {
        return 1.0 / power(base, -exponent);
    }
    if (exponent == 0) {
        return 1.0;
    }
    if (exponent % 2 == 0) {
        double half = power(base, exponent / 2);
        return half * half;
    }
    else {
        return base * power(base, exponent - 1);
    }
}

int main() {
    
    double mean;
    ErrorCode result_g = geometric_mean(& mean, 3, 2.0, 8.0, 4.0);
    if (result_g == SUCCESS) {
        printf("Geometric Mean: %f\n", mean);
    }
    else {
        switch (result_g) {
        case ERROR_INVALID_ARGUMENTS:
            printf("Error: Invalid arguments provided.\n");
            break;
        case ERROR_NO_NUMBERS:
            printf("Error: All numbers must be positive.\n");
            break;
        default:
            break;
        }
    }
    double base = 3.0;
    int exponent = 10;
    double result = power(base, exponent);
    printf("%f raised to the power of %d is %f\n", base, exponent, result);

    return 0;
}

