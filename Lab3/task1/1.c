#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int isNumeric(const char* str) {
    if (*str == '-') {
        str++;
    }

    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

void printInBase(unsigned int num, int base) {
    if (num >= (unsigned int)base) {
        printInBase(num / base, base);
    }
    int digit = num % base;
    if (digit < 10) {
        printf("%d", digit);
    } else {
        printf("%c", 'A' + (digit - 10));
    }
}

void convertToBase(int num) {
    if (num == 0) {
        for (int r = 1; r <= 5; r++) {
            printf("Результат в системе 2^%d: 0\n", r);
        }
        return;
    }

    int isNegative = num < 0;
    if (isNegative) {
        num = -num;
    }

    for (int r = 1; r <= 5; r++) {
        unsigned int base = 1 << r;
        unsigned int mask = base - 1;
        unsigned int temp = (unsigned int)num;
        unsigned int output[32];
        int index = 0;

        while (temp) {
            output[index++] = temp & mask;
            temp >>= r;
        }

        printf("Результат в системе 2^%d: ", r);
        if (isNegative) {
            printf("-");
        }
        for (int i = index - 1; i >= 0; i--) {
            printInBase(output[i], base);
            if (i > 0) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Ошибка: Использование: ./a.out <число>\n");
        return 1;
    }

    if (!isNumeric(argv[1])) {
        printf("Ошибка: Ввод должен быть числом.\n");
        return 1;
    }

    int number = atoi(argv[1]);
    printf("Преобразование числа %d в разные системы счисления:\n", number);
    convertToBase(number);

    return 0;
}
