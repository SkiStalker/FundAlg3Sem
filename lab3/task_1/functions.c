#include "functions.h"

int add(int a, int b){
    int carry;
    while (b != 0){
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

int size_of_str(const char *str) {
    if (!str) {
        return 0;
    }
    int k = 0;
    while (str[k] != '\0') {
        k = add(k, 1);
    }
    return k;
}

void reverse(char* array){
    int i = 0;
    int b = add(size_of_str(array), -1);
    while (i < b){
        char t = array[i];
        array[i] = array[b];
        array[b] = t;
        i = add(i, 1);
        b = add(b, -1);
    }
}

void right_shift(char* array){
    int size = size_of_str(array);
    array[add(size, 1)] = '\0';
    for (int i = size; i > 0; i = add(i, -1)){
        array[i] = array[add(i, -1)];
    }
}

enum return_code translate_to_system(int decimal, char* result, int r){
    if (!result) {
        return BAD_POINTER_ERROR;
    }
    if (decimal == 0){
        result[0] = '0';
        result[1] = '\0';
        return OK;
    }
    int a  = decimal;
    int mask = 1;
    for (int i = 0; i < r - 1; i = add(i, 1)){
        mask = mask << 1;
        mask = mask | 1;
    }
    int sign = 0;
    if (decimal < 0){
        a = add(~decimal, 1);
        sign = 1;
    }
    char* alphbet = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    for (int i = 0; i <= 31 && a > 0; i = add(i, 1)){
        result[i] = alphbet[a & mask];
        a = a >> r;
    }
    reverse(result);
    if (sign){
        right_shift(result);
        result[0] = '-';
    }
    return OK;
}

