#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define MAX_NUM_LENGTH 50
#define MAX_INPUTS 100

// Функция для проверки, является ли строка числом в заданной системе счисления
int isValidNumber(const char* num, int base) {
    for (int i = 0; num[i] != '\0'; i++) {
        char c = num[i];
        int value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            value = c - 'A' + 10;
        } else {
            return 0; // Неверный символ
        }

        if (value >= base) {
            return 0; // Значение больше основания
        }
    }
    return 1; // Корректное число
}

// Функция для сложения двух чисел в заданной системе счисления
char* addNumbersInColumn(const char* num1, const char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLen = len1 > len2 ? len1 : len2;

    char* result = (char*)malloc(maxLen + 2); // +1 для возможного переноса, +1 для '\0'
    if (!result) {
        fprintf(stderr, "Memory allocation failed in addNumbersInColumn.\n");
        exit(EXIT_FAILURE);
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

    // Реверс результата
    for (int k = 0; k < i / 2; k++) {
        char temp = result[k];
        result[k] = result[i - k - 1];
        result[i - k - 1] = temp;
    }

    result[i] = '\0'; // Завершаем строку нулевым символом
    return result;
}

// Функция для вычисления суммы переданных чисел
char* sumInBase(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    const char* firstNumber = va_arg(args, char*);

    // Выделяем память под первое число
    char* total = (char*)malloc(strlen(firstNumber) + 1);
    if (!total) {
        fprintf(stderr, "Memory allocation failed in sumInBase.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(total, firstNumber);

    for (int i = 1; i < count; i++) {
        const char* nextNumber = va_arg(args, char*);
        char* newTotal = addNumbersInColumn(total, nextNumber, base);
        
        free(total); // Освобождаем старый результат
        total = newTotal; // Обновляем сумму
    }

    va_end(args);
    return total;
}

// Функция для получения входных данных
void getInput(int* base, int* count, char numbers[][MAX_NUM_LENGTH]) {
    while (1) {
        printf("Enter the base (between 2 and 36): ");
        if (scanf("%d", base) != 1 || *base < 2 || *base > 36) {
            printf("Error: Invalid base. Please enter a number between 2 and 36.\n");
            while (getchar() != '\n'); // Очистка буфера ввода
            continue;
        }
        break;
    }

    while (1) {
        printf("Enter the number of inputs: ");
        if (scanf("%d", count) != 1 || *count <= 0 || *count > MAX_INPUTS) {
            printf("Error: Invalid count. Please enter a positive integer less than or equal to %d.\n", MAX_INPUTS);
            while (getchar() != '\n'); // Очистка буфера ввода
            continue;
        }
        break;
    }

    printf("Enter the numbers:\n");
    for (int i = 0; i < *count; i++) {
        while (1) {
            printf("Number %d: ", i + 1);
            scanf("%s", numbers[i]);
            if (isValidNumber(numbers[i], *base)) {
                break; // Число корректно
            } else {
                printf("Error: Invalid number. Please enter a valid number in base %d.\n", *base);
            }
        }
    }
}

// Демонстрация работы функций
int main() {
    int base;
    int count;
    char numbers[MAX_INPUTS][MAX_NUM_LENGTH]; // Массив для хранения чисел

    // Получаем входные данные
    getInput(&base, &count, numbers);

    // Вычисляем сумму
    char* result = sumInBase(base, count, numbers[0], numbers[1], numbers[2]); // Здесь можно адаптировать, чтобы передать все числа динамически

    printf("Sum in base %d: %s\n", base, result);

    free(result); // Освобождаем память
    return 0;
}
