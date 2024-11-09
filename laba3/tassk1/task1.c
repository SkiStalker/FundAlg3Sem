#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_DIGITS 32

int is_number(const char *str) {
    if (!*str) return 0;
    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

// Функция перевода числа num в систему счисления с основанием 2^r
void decimal_to_base2r(unsigned int num, unsigned int r) {
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    unsigned int mask = (1U << r) - 1;
    char buffer[MAX_DIGITS + 1];
    int index = 0;

    if (r < 1 || r > 5) {
        printf("Ошибка: значение r должно быть в диапазоне от 1 до 5.\n");
        return;
    }

    do {
        unsigned int digit = num & mask;
        buffer[index++] = digits[digit];
        num >>= r;
    } while (num != 0 && index < MAX_DIGITS);

    if (index == MAX_DIGITS && num != 0) {
        printf("Ошибка: превышен размер буфера для данного значения r.\n");
        return;
    }

    buffer[index] = '\0';

    printf("Число в системе счисления с основанием 2^%u: ", r);
    for (int i = index - 1; i >= 0; i--) {
        putchar(buffer[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    unsigned int r = 0;
    unsigned int number = 0;

    // Проверка количества аргументов
    if (argc != 3) {
        printf("Ошибка: Использование: ./a.out <число> <r>\n");
        return 1;
    }

    // Проверка, что первый аргумент является числом и не имеет отрицательного знака
    if (!is_number(argv[1])) {
        printf("Ошибка: Ввод должен быть положительным целым числом\n");
        return 1;
    }

    number = (unsigned int)strtoul(argv[1], NULL, 10);
    r = (unsigned int)strtoul(argv[2], NULL, 10);
    
    // Проверка диапазона r
    if (r < 1 || r > 5) {
        printf("Ошибка: Значение r должно быть в диапазоне от 1 до 5.\n");
        return 1;
    }

    // Выполнение перевода числа и вывод результата
    decimal_to_base2r(number, r);

    return 0;
}
