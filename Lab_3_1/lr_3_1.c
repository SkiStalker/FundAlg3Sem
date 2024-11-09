#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LenOfNumber(int number) {
    int len = 0;
    while (number) {
        number >>= 1;
        len++;
    }
    return len + 2;
}

int PowOf2(int base) {
    int p = 0;
    while (base) {
        base >>= 1;
        p++;
    }
    return p - 1;
}

int strLen(const char *str) {
    int res = 0;
    while (*str != '\0') {
        res++;
        str++;
    }
    return res;
}

char *reverse(const char *str) {
    int len = strLen(str);
    char *reversed = (char*)malloc(len + 1);
    if (reversed == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';
    return reversed;
}

int OstMask(int pow) {
    int mask = 0;
    switch(pow) {
        case 1: {
            mask = 1;
            break;
        }
        case 2: {
            mask = 3;
            break;
        }
        case 3: {
            mask = 7;
            break;
        }
        case 4: {
            mask = 15;
            break;
        }
        case 5: {
            mask = 31;
            break;
        }
        default: {
            mask = -1;
            break;
        }
    }
    return mask;
}

char *DecToBase(int base, int num) {
    if (base != 2 && base != 4 && base != 8 && base != 16 && base != 32) {
        char *error = "This base is not supported\0";
        return error;
    }
    int pow = PowOf2(base), len = LenOfNumber(num), mask = OstMask(pow);
    char *result = (char *)malloc(len);
    if (result == NULL) {
        printf("Error allocating memory\n");
        return NULL;
    }

    int ind = 0, div_ost;
    while (num > 1) {
        div_ost = num & mask;
        num >>= pow;
        result[ind++] = (div_ost > 9) ? (div_ost + 'a' - 10) : (div_ost + '0');
    }
    if (num == 1) {
        result[ind++] = '1';
    }
    result[ind] = '\0';

    char *ans = reverse(result);
    return ans;
}

void HandlingFunction(int base, int num) {
    char *res = DecToBase(base, num);
    if (strcmp(res, "This base is not supported\0") == 0) {
        printf("This base is not supported: %d.\n", base);
        return;
    }
    printf("%d in %d system is %s\n", num, base, res);
    free(res);
}

int main() {
    HandlingFunction(0, 37);
    HandlingFunction(1, 37);
    HandlingFunction(3, 37);
    HandlingFunction(33, 37);
    HandlingFunction(-1, 37);

    HandlingFunction(2, 37);
    HandlingFunction(4, 37);
    HandlingFunction(8, 37);
    HandlingFunction(16, 37);
    HandlingFunction(32, 37);

    HandlingFunction(2, 176);
    HandlingFunction(4, 176);
    HandlingFunction(8, 176);
    HandlingFunction(16, 176);
    HandlingFunction(32, 176);

    HandlingFunction(2, 253);
    HandlingFunction(4, 253);
    HandlingFunction(8, 253);
    HandlingFunction(16, 253);
    HandlingFunction(32, 253);

    return 0;
}