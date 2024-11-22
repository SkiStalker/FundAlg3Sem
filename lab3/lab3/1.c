#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int IsInt(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int MyPlus(int a, int b) {
    while (b != 0) {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

void ToBase2r(unsigned int num, int r) {
    int mask = MyPlus((1 << r), -1);
    int size = 32; // sizeof(int) * 8
    char result[MyPlus(size, 1)];
    int index = 0;

    int shift = 0;
    while (shift < size) {
        unsigned int digit = (num >> shift) & mask;

        if (digit < 10) {
            result[index] = MyPlus('0', digit);
        } else {
            result[index] = MyPlus('A', MyPlus(digit, -10));
        }
        index++;
        shift = MyPlus(shift, r);
    }
    result[index] = '\0';

    // разворот
    int i = 0;
    int end = MyPlus(index, -1);
    while (i < end) {
        char temp = result[i];
        result[i] = result[end];
        result[end] = temp;
        i = MyPlus(i, 1);
        end = MyPlus(end, -1);
    }

    // удаление лидирующих нулей
    int leadZero = 0;
    while ((result[leadZero] == '0') && (leadZero < MyPlus(index, -1))) {
        leadZero = MyPlus(leadZero, 1);
    }

    printf("%u in 2^%d = %s\n", num, r, &result[leadZero]);
}

int main() {
    printf("enter a number: ");
    char chNum[10];
    scanf("%s", chNum);
    
    // Проверка, является ли ввод числом
    if (!IsInt(chNum)) {
        printf("invalid number\n");
        return 1;
    }

    int num = atoi(chNum);
    
    // Преобразование числа в разные системы счисления
    for (int r = 1; r <= 5; r++) {
        ToBase2r(num, r);
    }

    return 0;
}
