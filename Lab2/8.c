#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define MAX_NUM_LENGTH 50
#define MAX_INPUTS 100

int isValidNumber(const char* num, int base) {
    for (int i = 0; num[i] != '\0'; i++) {
        char c = num[i];
        int value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            value = c - 'A' + 10;
        } else {
            return 0; 
        }

        if (value >= base) {
            return 0;
        }
    }
    return 1;
}

char* addNumbersInColumn(const char* num1, const char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLen = len1 > len2 ? len1 : len2;

    char* result = (char*)malloc(maxLen + 2);
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

    for (int k = 0; k < i / 2; k++) {
        char temp = result[k];
        result[k] = result[i - k - 1];
        result[i - k - 1] = temp;
    }

    result[i] = '\0';
    return result;
}

char* sumInBase(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    const char* firstNumber = va_arg(args, char*);

    char* total = (char*)malloc(strlen(firstNumber) + 1);
    if (!total) {
        fprintf(stderr, "Memory allocation failed in sumInBase.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(total, firstNumber);

    for (int i = 1; i < count; i++) {
        const char* nextNumber = va_arg(args, char*);
        char* newTotal = addNumbersInColumn(total, nextNumber, base);
        
        free(total);
        total = newTotal;
    }

    va_end(args);
    return total;
}

void getInput(int* base, int* count, char numbers[][MAX_NUM_LENGTH]) {
    while (1) {
        printf("Enter the base (between 2 and 36): ");
        if (scanf("%d", base) != 1 || *base < 2 || *base > 36) {
            printf("Error: Invalid base. Please enter a number between 2 and 36.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    while (1) {
        printf("Enter the number of inputs: ");
        if (scanf("%d", count) != 1 || *count <= 0 || *count > MAX_INPUTS) {
            printf("Error: Invalid count. Please enter a positive integer less than or equal to %d.\n", MAX_INPUTS);
            while (getchar() != '\n');
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
                break;
            } else {
                printf("Error: Invalid number. Please enter a valid number in base %d.\n", *base);
            }
        }
    }
}

int main() {
    int base;
    int count;
    char numbers[MAX_INPUTS][MAX_NUM_LENGTH];

    getInput(&base, &count, numbers);

    char* result = sumInBase(base, count, numbers[0], numbers[1], numbers[2]);

    printf("Sum in base %d: %s\n", base, result);

    free(result);
    return 0;
}
