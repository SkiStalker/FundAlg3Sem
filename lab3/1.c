#include <stdio.h>
#include <limits.h>

unsigned int StringToUnsignedInt(const char *str) {
    unsigned int result = 0;

    while (*str) {
        if (*str >= '0' && *str <= '9') {
            result = (result << 3) + (result << 1) + (*str - '0');
        } else {
            return -1;
        }
        str++;
    }

    return result;
}

int GetNumber(unsigned int *number) {
    char numberStr[100];
    printf("Введите положительное целое число в 10-сс: ");
    scanf("%99s", numberStr);  // Ограничиваем количество символов для предотвращения переполнения

    unsigned int parsedNumber = StringToUnsignedInt(numberStr);
    if (parsedNumber != UINT_MAX) {
        *number = parsedNumber;
    } else {
        printf("Некорректный ввод\n");
        return -1;
    }
    return 0;
}

int ConvertToBase2r(unsigned int number, unsigned int r) {
    const char symbols[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    unsigned int base = 1 << r;
    unsigned int mask = base - 1;

    printf("Число в системе счисления с основанием 2^%u: ", r);
    
    char digits[32];
    int index = 0;

    while (number > 0) {
        unsigned int digit = number & mask;
        digits[index++] = symbols[digit];
        number >>= r;
    }

    
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", digits[i]);
    }
    printf("\n");
    return 0;
}

int main() {
    unsigned int number;
    while (GetNumber(&number) != 0) {}
    for (unsigned int r = 1; r <= 5; r++) {
        ConvertToBase2r(number, r);
    }

    return 0;
}
