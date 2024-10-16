#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 100
#define MAX_NUMBERS 100

typedef enum {
    SUCCESS = 0,
    ERR_INVALID_BASE,
    ERR_INVALID_NUMBER,
    ERR_MEMORY_ALLOCATION,
    ERR_INVALID_INPUT,
    ERR_NO_NUMBERS_ENTERED
} StatusCode;


StatusCode convertToBase(const char* str, int base, long long* result) {
    *result = 0;
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        char c = str[i];
        int digitValue;

        if (isdigit(c)) {
            digitValue = c - '0'; 
        } else if (isupper(c)) {
            digitValue = c - 'A' + 10; 
        } else {
            return ERR_INVALID_NUMBER; 
        }

        if (digitValue >= base) {
            return ERR_INVALID_NUMBER;
        }

        *result = *result * base + digitValue; 
    }
    return SUCCESS;
}


StatusCode convertToBaseString(long long number, int base, char* result) {
    if (number == 0) {
        strcpy(result, "0");
        return SUCCESS;
    }

    char buffer[MAX_INPUT_LENGTH];
    int index = 0;
    long long n = llabs(number); 

    while (n > 0) {
        int digit = n % base;
        buffer[index++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A'); 
        n /= base;
    }
    buffer[index] = '\0';


    for (int i = 0; i < index; i++) {
        result[i] = buffer[index - 1 - i];
    }
    result[index] = '\0';
    return SUCCESS;
}


StatusCode processInput(int base, long long* numbers, int* count) {
    char input[MAX_INPUT_LENGTH];
    StatusCode status;

    printf("Введите числа (введите 'Stop' для завершения ввода):\n");

    while (1) {
        scanf("%s", input);
        if (strcmp(input, "Stop") == 0) {
            break;
        }

        long long number;
        status = convertToBase(input, base, &number);
        if (status != SUCCESS) {
            fprintf(stderr, "Некорректное число: %s\n", input);
            continue;
        }

        if (*count < MAX_NUMBERS) {
            numbers[*count] = number;
            (*count)++;
        } else {
            fprintf(stderr, "Превышено максимальное количество чисел.\n");
            return ERR_MEMORY_ALLOCATION;
        }
    }
    return SUCCESS;
}

int main() {
    int base;
    printf("Введите основание системы счисления (2..36): ");
    if (scanf("%d", &base) != 1 || base < 2 || base > 36) {
        fprintf(stderr, "Основание должно быть в диапазоне [2..36].\n");
        return EXIT_FAILURE;
    }

    long long numbers[MAX_NUMBERS] = {0};
    int count = 0;
    StatusCode status = processInput(base, numbers, &count);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }

    if (count == 0) {
        fprintf(stderr, "Не было введено ни одного числа.\n");
        return EXIT_FAILURE;
    }

    long long maxNumber = numbers[0];
    for (int i = 1; i < count; i++) {
        if (llabs(numbers[i]) > llabs(maxNumber)) {
            maxNumber = numbers[i];
        }
    }

    char maxNumberStr[MAX_INPUT_LENGTH];
    status = convertToBaseString(maxNumber, base, maxNumberStr);
    if (status == SUCCESS) {
        printf("Максимальное число по модулю в системе счисления %d: %s\n", base, maxNumberStr);
    } else {
        fprintf(stderr, "Ошибка преобразования числа в систему счисления %d.\n", base);
    }

    char result[MAX_INPUT_LENGTH];
    for (int i = 9; i <= 36; i += 9) {
        status = convertToBaseString(maxNumber, i, result);
        if (status == SUCCESS) {
            printf("Представление в системе счисления %d: %s\n", i, result);
        } else {
            fprintf(stderr, "Ошибка преобразования числа в систему счисления %d.\n", i);
        }
    }

    return EXIT_SUCCESS;
}
