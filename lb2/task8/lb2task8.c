#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef enum {
    SUCCESS,
    BASE_ERROR,
    NUMBER_ERROR,
    OVERFLOW_ERROR
} ErrorCode;


int is_valid_char(char c, int base) {
    if (isdigit(c)) {
        return (c - '0') < base;
    }
    if (isalpha(c)) {
        return (tolower(c) - 'a' + 10) < base;
    }
    return 0;
}


ErrorCode validate_number(const char* num, int base) {
    for (size_t i = 0; i < strlen(num); i++) {
        if (!is_valid_char(num[i], base)) {
            return NUMBER_ERROR;
        }
    }
    return SUCCESS;
}


char* add_strings(const char* num1, const char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxlen = (len1 > len2 ? len1 : len2) + 1; 
    char* result = (char*)malloc(maxlen + 1);
    if (!result) {
        return NULL; 
    }
    result[maxlen] = '\0'; 

    int carry = 0, i = len1 - 1, j = len2 - 1, k = maxlen - 1;

    while (i >= 0 || j >= 0 || carry) {
        int digit1 = (i >= 0) ? (isdigit(num1[i]) ? (num1[i] - '0') : (tolower(num1[i]) - 'a' + 10)) : 0;
        int digit2 = (j >= 0) ? (isdigit(num2[j]) ? (num2[j] - '0') : (tolower(num2[j]) - 'a' + 10)) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        sum = sum % base;

        result[k--] = (sum < 10) ? (sum + '0') : (sum - 10 + 'A');
        i--;
        j--;
    }

  
    if (k < maxlen - 1) {
        return strdup(result + k + 1);
    }
    else {
        free(result);
        return strdup("0");
    }
}


ErrorCode sum_numbers(int base, int count, char** result, ...) {
    if (base < 2 || base > 36) {
        return BASE_ERROR;
    }

    va_list args;
    va_start(args, result);
    char* total = strdup("0");

    for (int i = 0; i < count; i++) {
        char* num = va_arg(args, char*);
        ErrorCode err = validate_number(num, base);
        if (err != SUCCESS) {
            va_end(args);
            free(total);
            return NUMBER_ERROR;
        }
        char* new_total = add_strings(total, num, base);
        free(total);
        total = new_total;

        if (!total) {
            va_end(args);
            return OVERFLOW_ERROR;
        }
    }

    va_end(args);
    *result = total; 
    return SUCCESS;
}

int main() {
    char* result = NULL;
    ErrorCode err = sum_numbers(16, 3, &result, "1A", "B2", "C");

    if (err == SUCCESS) {
        printf("Sum: %s\n", result);
        free(result);
    }
    else {
        switch (err) {
        case BASE_ERROR:
            printf("Error: Base is not between 2 and 36!\n"); 
            break;
        case NUMBER_ERROR:
            printf("Error: One of the numbers is not valid in the given base!\n"); 
            break;
        case OVERFLOW_ERROR:
            printf("Error: Overflow occurred during addition!\n"); 
            break;
        default:
            printf("An unknown error has occurred!\n");
            break;
        }
    }

    return 0;
}

