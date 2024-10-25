#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define MAX_NUM_LENGTH 50

int isValidNumber(const char* num, int base) {
    for (int i = 0; num[i] != '\0'; i++) {
        char c = num[i];
        int value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            value = c - 'A' + 10;
        } else {
            return 0; 
        }

        if (value >= base) {
            return 0;
        }
    }
    return 1;
}

char* addNumbersInColumn(const char* num1, const char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLen = len1 > len2 ? len1 : len2;

    char* result = (char*)malloc(maxLen + 2);
    if (!result) {
        return NULL;
    }
    
    result[maxLen + 1] = '\0';

    int carry = 0;
    int i;

    for (i = 0; i < maxLen || carry; i++) {
        int digit1 = (i < len1) ? (num1[len1 - i - 1] >= '0' && num1[len1 - i - 1] <= '9' ? num1[len1 - i - 1] - '0' : num1[len1 - i - 1] - 'A' + 10) : 0;
        int digit2 = (i < len2) ? (num2[len2 - i - 1] >= '0' && num2[len2 - i - 1] <= '9' ? num2[len2 - i - 1] - '0' : num2[len2 - i - 1] - 'A' + 10) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        result[i] = (sum % base) < 10 ? (sum % base) + '0' : (sum % base - 10) + 'A';
    }

    for (int k = 0; k < i / 2; k++) {
        char temp = result[k];
        result[k] = result[i - k - 1];
        result[i - k - 1] = temp;
    }

    result[i] = '\0';
    return result;
}

char* sumInBase(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    const char* firstNumber = va_arg(args, char*);

    char* total = (char*)malloc(strlen(firstNumber) + 1);
    if (!total) {
        va_end(args);
        return NULL;
    }
    strcpy(total, firstNumber);

    printf("Складываем числа в базе %d:\n", base);
    printf("%s", total);

    for (int i = 1; i < count; i++) {
        const char* nextNumber = va_arg(args, char*);
        printf(" + %s", nextNumber);
        char* newTotal = addNumbersInColumn(total, nextNumber, base);
        
        if (!newTotal) { 
            free(total);
            va_end(args);
            return NULL;
        }

        free(total);
        total = newTotal;
    }

    va_end(args);
    printf(" = %s\n", total);
    return total;
}

int main() {
    int base2 = 2;
    const char* numbersBase2[] = {"1010", "111", "1101"};
    printf("Сложение в двоичной системе (база %d):\n", base2);
    char* result2 = sumInBase(base2, 3, numbersBase2[0], numbersBase2[1], numbersBase2[2]);
    if (result2) {
        printf("Результат: %s\n\n", result2);
        free(result2);
    } else {
        printf("Ошибка: не удалось выполнить сложение в двоичной системе\n\n");
    }

    int base8 = 8;
    const char* numbersBase8[] = {"12", "7", "15"};
    printf("Сложение в восьмеричной системе (база %d):\n", base8);
    char* result8 = sumInBase(base8, 3, numbersBase8[0], numbersBase8[1], numbersBase8[2]);
    if (result8) {
        printf("Результат: %s\n\n", result8);
        free(result8);
    } else {
        printf("Ошибка: не удалось выполнить сложение в восьмеричной системе\n\n");
    }

    int base10 = 10;
    const char* numbersBase10[] = {"25", "37", "89"};
    printf("Сложение в десятичной системе (база %d):\n", base10);
    char* result10 = sumInBase(base10, 3, numbersBase10[0], numbersBase10[1], numbersBase10[2]);
    if (result10) {
        printf("Результат: %s\n\n", result10);
        free(result10);
    } else {
        printf("Ошибка: не удалось выполнить сложение в десятичной системе\n\n");
    }

    int base16 = 16;
    const char* numbersBase16[] = {"A", "1F", "B"};
    printf("Сложение в шестнадцатеричной системе (база %d):\n", base16);
    char* result16 = sumInBase(base16, 3, numbersBase16[0], numbersBase16[1], numbersBase16[2]);
    if (result16) {
        printf("Результат: %s\n\n", result16);
        free(result16);
    } else {
        printf("Ошибка: не удалось выполнить сложение в шестнадцатеричной системе\n\n");
    }

    return 0;
}
