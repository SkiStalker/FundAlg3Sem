#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

enum {
    MALLOC_FAILED,
    R_INVALID,
    NUM_IS_TOO_BIG,
    SUCCESS
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case MALLOC_FAILED: printf("malloc failed\n"); break;
        case R_INVALID: printf("error: 1 <= r <= 5\n"); break;
        case NUM_IS_TOO_BIG: printf("error: NUM IS TOO BIG\n"); break;
        case SUCCESS: break;
        default: printf("Unknown error code\n"); break;
    }
}

int func(unsigned int n, char* res, const unsigned int r) {
    int i = 31;
    int onesymb_1;
    switch (r) {
        case 1: onesymb_1 = 1; break;
        case 2: onesymb_1 = 3; break;
        case 3: onesymb_1 = 7; break;
        case 4: onesymb_1 = 15; break;
        case 5: onesymb_1 = 31; break;
        default: return R_INVALID;
    }
    while(n) {
        unsigned int tek_el = n & onesymb_1;
        if (tek_el < 10) {
            res[i--] = tek_el + '0';
        }
        else {
            res[i--] = tek_el - 10 + 'A';
        }
        n = n >> r;
    }
    return SUCCESS;
}

int printfunc(char str[], const unsigned int r) {
    if (strlen(str) > 9) {
        ValidateCode(NUM_IS_TOO_BIG);
        return NUM_IS_TOO_BIG;
    }
    unsigned int n = atoi(str);
    char* res = malloc(33 * sizeof(char));
    if (res == NULL) {
        return -1;
    }
    for(int i = 0; i < 32; i++) {
        res[i] = '0';
    }
    res[32] = '\0';
    if (func(n, res, r) != SUCCESS) {
        ValidateCode(R_INVALID);
        free(res);
        return -1;
    }
    printf("%s\n", res);
    free(res);
    return SUCCESS;
}

int main() {
    printfunc("8", 3);
    printfunc("10", 2);
    printfunc("231412", 5);
    printfunc("999999999", 1);
    printfunc("782346523522893457", 1);
    printfunc("782346", 7);
    return 0;
}