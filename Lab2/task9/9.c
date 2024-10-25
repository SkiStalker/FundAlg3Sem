#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int isFraction(const char *str) {
    int len = strlen(str);
    int hasDecimal = 0;

    if (len == 0 || str[0] == '.' || str[len - 1] == '.') {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (str[i] == '.') {
            if (hasDecimal) return 0;
            hasDecimal = 1;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }

    double num = atof(str);
    return num > 0.0 && num < 1.0;
}

int isInteger(const char *str) {
    if (*str == '-' || *str == '+') str++;

    if (*str == '\0') return 0;

    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }

    return 1;
}

int findGcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int hasFiniteRepresentation(double number, int base) {
    int denominator = 1;

    while (number != floor(number)) {
        number *= 10;
        denominator *= 10;
    }

    int gcdValue = findGcd((int)number, denominator);
    denominator /= gcdValue;

    int baseCopy = base;
    for (int divisor = 2; divisor <= baseCopy; divisor++) {
        while (baseCopy % divisor == 0) {
            baseCopy /= divisor;
        }

        while (denominator % divisor == 0) {
            denominator /= divisor;
        }
    }

    return denominator == 1;
}

void checkFractionRepresentation(int base, int count, double numbers[]) {
    for (int i = 0; i < count; i++) {
        double number = numbers[i];
        char formattedNumber[20];
        snprintf(formattedNumber, sizeof(formattedNumber), "%.12f", number);

        char *dot = strchr(formattedNumber, '.');
        if (dot) {
            char *end = formattedNumber + strlen(formattedNumber) - 1;
            while (end > dot && *end == '0') {
                *end-- = '\0';
            }
            if (end == dot) {
                *dot = '\0';
            }
        }

        if (hasFiniteRepresentation(number, base)) {
            printf("Число %s имеет конечное представление в системе с основанием %d.\n", formattedNumber, base);
        } else {
            printf("Число %s не имеет конечное представление в системе с основанием %d.\n", formattedNumber, base);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: ./a.out <основание> <дробь1> <дробь2> ...\n");
        return 1;
    }

    if (!isInteger(argv[1])) {
        printf("Ошибка: основание должно быть положительным целым числом.\n");
        return 1;
    }

    int base = atoi(argv[1]);
    if (base < 2) {
        printf("Ошибка: основание должно быть целым числом больше 1.\n");
        return 1;
    }

    int count = argc - 2;
    double *numbers = (double *)malloc(count * sizeof(double));

    if (numbers == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        if (!isFraction(argv[i + 2])) {
            printf("Ошибка: '%s' не является допустимым числом.\n", argv[i + 2]);
            free(numbers);
            return 1;
        }
        numbers[i] = atof(argv[i + 2]);
    }

    checkFractionRepresentation(base, count, numbers);

    free(numbers);

    return 0;
}
