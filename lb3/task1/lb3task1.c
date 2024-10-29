#include <stdio.h>
#include <stdlib.h>

typedef enum {
    SUCCESS,
    ERROR_INVALID_BASE,
    ERROR_INVALID_INPUT,
    ERROR_MEMORY_ALLOCATION
} ErrorCode;

void convert_to_base(int number, int base, int r, unsigned char** output, size_t* output_size, ErrorCode* error) {
    // Проверка на допустимость основания
    if (base < 2 || base > 32) {
        *error = ERROR_INVALID_BASE;
        return;
    }

    // Проверка на отрицательное число
    if (number < 0) {
        *error = ERROR_INVALID_INPUT;
        return;
    }

    // Выделяем память для результата
    *output_size = 0;
    *output = malloc(32); // Максимальная длина для 32-битного числа
    if (*output == NULL) {
        *error = ERROR_MEMORY_ALLOCATION;
        return;
    }

    // Перевод числа в заданную систему счисления
    unsigned char* buffer = *output;
    do {
        int digit = number & (base - 1); // Получаем последнюю цифру
        buffer[(*output_size)++] = digit + (digit < 10 ? '0' : 'A' - 10);
        number >>= r; // Делим на 2^r
    } while (number > 0);

    // Реверсируем результат для корректного вывода
    for (size_t i = 0; i < *output_size / 2; i++) {
        unsigned char temp = buffer[i];
        buffer[i] = buffer[*output_size - 1 - i];
        buffer[*output_size - 1 - i] = temp;
    }

    buffer[*output_size] = '\0'; // добавляем нулевой символ в конец
    *error = SUCCESS;
}

int main() {
    int number;
    unsigned char* result;
    size_t result_size;
    ErrorCode error;

    // Пример ввода
    number = 255; // число для перевода
    for (int r = 1; r <= 5; r++) {
        int base = 1 << r; // вычисляем 2^r
        convert_to_base(number, base, r, &result, &result_size, &error);

        if (error != SUCCESS) {
            switch (error) {
            case ERROR_INVALID_BASE:
                printf("Error: Invalid base %d\n", base);
                break;
            case ERROR_INVALID_INPUT:
                printf("Error: Invalid number input %d\n", number);
                break;
            case ERROR_MEMORY_ALLOCATION:
                printf("Error: Memory allocation error\n");
                break;
            default:
                break;
            }
        }
        else {
            printf("The number %d in base %d: %s\n", number, base, result);
        }

        // Освобождаем память
        free(result);
    }

    return 0;
}



