#include "functions.h"

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("There must be an epsillon");
        return ERROR;
    }

    if (checkNumber(argv[1]) == ERROR_NAN){
        printf("There must be a number");
        return ERROR;
    }

    double eps = chToD(argv[1]);
    if (eps <= 0) {
            printf("Must be positive numbers\n");
            return ERROR;
        }
    callback cbs[] = {&eLim, &eTaylor, &eUravn, &piLim, &piTaylor, &piUravn, &lnLim, &lnTaylor, &lnUravn, &sqrtTwoLim, &sqrtTwoTaylor, &sqrtTwoUravn, &gLim, &gTaylor, &gUravn};
    printf("|   Constant    |  Limit  |  Taylor |   Uravn   |");
    for (int i = 0; i < 15; ++i){
        switch(i){
            case 0:
                printf("\ne               ");
                break;
            case 3:
                printf("\npi              ");
                break;
            case 6:
                printf("\nln(2)           ");
                break;
            case 9:
                printf("\nsqrt(2)         ");
                break;
            case 12:
                printf("\ng               ");
                break;
        }
        callback find = cbs[i];
        printf("| %8f", find(eps));
    }
}