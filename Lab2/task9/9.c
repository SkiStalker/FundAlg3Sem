#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void parseFraction(const char *str, int *numerator, int *denominator) {
    char *dot = strchr(str, '.');
    if (dot) {
        int integerPart = atoi(str);
        int fractionalPart = atoi(dot + 1);
        int fractionalLength = strlen(dot + 1);
        
        *denominator = 1;
        for (int i = 0; i < fractionalLength; i++) {
            *denominator *= 10;
        }
        
        *numerator = integerPart * (*denominator) + fractionalPart;
    } else {
        *numerator = atoi(str);
        *denominator = 1;
    }
}

// Проверка, имеет ли дробь конечное представление в заданном основании
int hasFiniteRepresentation(int numerator, int denominator, int base) {
    // Упрощаем дробь
    int gcdValue = findGcd(numerator, denominator);
    denominator /= gcdValue;

    // Проверяем делимость знаменателя только на простые множители основания
    while (denominator > 1) {
        int factor = 2; // Начинаем с 2
        int found = 0;

        // Проверяем, можно ли разделить знаменатель на простые множители
        while (factor * factor <= base) {
            if (denominator % factor == 0) {
                denominator /= factor;
                found = 1;
            } else {
                factor++;
            }
        }

        // Проверка последнего возможного простого множителя
        if (base > 1 && denominator % base == 0) {
            denominator /= base;
            found = 1;
        }

        // Если не нашли никаких простых множителей, выходим из цикла
        if (!found) {
            break;
        }
    }

    return denominator == 1; // Если дошли до 1, значит конечное представление есть
}

void checkFractionRepresentation(int base, int count, char *numbers[]) {
    for (int i = 0; i < count; i++) {
        int numerator, denominator;
        parseFraction(numbers[i], &numerator, &denominator);
        
        if (hasFiniteRepresentation(numerator, denominator, base)) {
            printf("Число %s имеет конечное представление в системе с основанием %d.\n", numbers[i], base);
        } else {
            printf("Число %s не имеет конечное представление в системе с основанием %d.\n", numbers[i], base);
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

    for (int i = 0; i < count; i++) {
        if (!isFraction(argv[i + 2])) {
            printf("Ошибка: '%s' не является допустимым числом.\n", argv[i + 2]);
            return 1;
        }
    }

    checkFractionRepresentation(base, count, &argv[2]);

    return 0;
}
