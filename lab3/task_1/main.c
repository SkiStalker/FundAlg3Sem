#include "main.h"

#include <stdio.h>
#include <string.h>

int main() {
    char result1[34] = {0};
    translate_to_system(-65536, result1, 4);
    printf("-65536 in 16 base: %s\n", result1);
    
    char result2[34] = {0};
    translate_to_system(127, result2, 1);
    printf("127 in 2 base: %s\n", result2);

    char result3[34] = {0};
    translate_to_system(0, result3, 2);
    printf("0 in 4 base: %s\n", result3);

    char result4[34] = {0};
    translate_to_system(1234, result4, 3);
    printf("1234 in 8 base: %s\n", result4);
}