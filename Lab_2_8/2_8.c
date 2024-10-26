#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

char* addStringsBase(const char* num1, const char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLength = len1 > len2 ? len1 : len2;

    char* result = (char*)malloc(maxLength + 2);
    result[maxLength + 1] = '\0';

    int carry = 0;
    for (int i = 0; i < maxLength; i++) {
        int digit1 = 0;
        int digit2 = 0;

        if (len1 - 1 - i >= 0) {
            if (num1[len1 - 1 - i] <= '9') {
                digit1 = num1[len1 - 1 - i] - '0';
            } else if (isalpha(num1[len1 - 1 - i])){
                digit1 = toupper(num1[len1 - 1 - i]) - 'A' + 10;
            }
        }

        if (len2 - 1 - i >= 0) {
            if (num2[len2 - 1 - i] <= '9') {
                digit2 = num2[len2 - 1 - i] - '0';
            } else if (isalpha(num2[len2 - 1 - i])){
                digit2 = toupper(num2[len2 - 1 - i]) - 'A' + 10;
            }
        }

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        result[maxLength - i] = (sum % base) + (sum % base < 10 ? '0' : 'A' - 10);
    }

    if (carry > 0) {
        result[0] = carry + (carry < 10 ? '0' : 'A' - 10);
        return result;
    } else {

        char* finalResult = (char*)malloc(maxLength + 1);
        strcpy(finalResult, result + 1);
        free(result);
        return finalResult;
    }
}

char* sumInBase(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    char* total = va_arg(args, char*);

    for (int i = 1; i < count; i++) {
        char* nextNum = va_arg(args, char*);
        total = addStringsBase(total, nextNum, base);
    }

    va_end(args);
    return total;
}

int main() {
    int base = 16;
    char* result = sumInBase(base, 3, "F", "F", "F");
    printf("Sum in base %d: %s\n", base, result);
    free(result);

    base = 10;
    result = sumInBase(base, 2, "12345678901234567890", "98765432109876543210");
    printf("Sum in base %d: %s\n", base, result);
    free(result);

    return 0;
}