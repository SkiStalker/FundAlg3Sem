#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect args count");
        return -1;
    }
    printf("argv_1+argv_2=%ld", strtol(argv[1], NULL, 10) + strtol(argv[2], NULL, 10));
    return 0;
}