#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BASE 36
#define MAX_NUMBER_LENGTH 100
#define SUCCESS 0
#define ERROR_FILE_OPEN 1
#define ERROR_MEMORY_ALLOCATION 2
#define ERROR_INVALID_NUMBER 3
#define ERROR_CONVERSION 4
#define ERROR_INVALID_BASE 5

int get_min_base(const char *number, int *min_base) {
    *min_base = 2;

    for (size_t i = 0; i < strlen(number); i++) {
        char ch = tolower(number[i]);
        int value;

        if (isdigit(ch)) {
            value = ch - '0'; 
        } else if (isalpha(ch)) {
            value = ch - 'a' + 10; 
        } else {
            return ERROR_INVALID_NUMBER; 
        }

        if (value + 1 > *min_base) {
            *min_base = value + 1; // Обновляем минимальное основание
        }
    }

    return (*min_base <= MAX_BASE) ? SUCCESS : ERROR_INVALID_BASE; // Проверка на допустимость основания
}

// Функция для конвертации числа в десятичную систему
int convert_to_decimal(const char *number, int base, long long *result) {
    *result = 0;
    long long multiplier = 1; // Множитель для текущей цифры
    size_t len = strlen(number);

    for (size_t i = len; i > 0; i--) {
        char ch = tolower(number[i - 1]);
        int value;

        if (isdigit(ch)) {
            value = ch - '0'; // С
        } else if (isalpha(ch)) {
            value = ch - 'a' + 10; // Символ буквы
        } else {
            return ERROR_INVALID_NUMBER; // Некорректный символ
        }

        *result += value * multiplier; // Добавляем значение цифры
        multiplier *= base; // Увеличиваем множитель
    }

    return SUCCESS; // Возвращаем результат
}

int process_file(const char *input_path, const char *output_path) {
    FILE *input = fopen(input_path, "r");
    FILE *output = fopen(output_path, "w");

    if (!input) {
        return ERROR_FILE_OPEN; // Ошибка открытия входного файла
    }
    if (!output) {
        fclose(input);
        return ERROR_FILE_OPEN; // Ошибка открытия выходного файла
    }

    char number[MAX_NUMBER_LENGTH];
    while (fscanf(input, "%99s", number) == 1) {
        // Удаляем ведущие нули
        char *start = number;
        while (*start == '0') start++;
        if (*start == '\0') start--; // Если число было 0

        int min_base;
        int status = get_min_base(start, &min_base);
        if (status != SUCCESS) {
            fprintf(output, "Error: Invalid number %s\n", number);
            continue; // Пропускаем некорректные числа
        }

        long long decimal_value;
        status = convert_to_decimal(start, min_base, &decimal_value);
        if (status != SUCCESS) {
            fprintf(output, "Error: Invalid conversion for %s\n", start);
            continue; // Пропускаем некорректные числа
        }

        fprintf(output, "%s %d %lld\n", start, min_base, decimal_value);
    }

    fclose(input);
    fclose(output);
    return SUCCESS; // Успешное завершение
}

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return ERROR_INVALID_NUMBER; // Ошибка в аргументах
    }

    int status = process_file(argv[1], argv[2]);
    if (status != SUCCESS) {
        fprintf(stderr, "Error: Unable to process files. Error code: %d\n", status);
        return status; // Возврат кода ошибки
    }

    return SUCCESS; // Успешное завершение
}
