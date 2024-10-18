#include "functions.h"

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("There must be two values");
        return ERROR_NO_VALUES;
    }

    const char* flags[] = {"-h", "/h", "-p", "/p", "-s", "/s", "-e", "/e", "-a", "/a", "-f", "/f"};
    callback cbs[] = {&funcForH, &funcForP, &funcForS, &funcForE, &funcForA, &funcForF};
    for (int i = 2; i < argc; ++i)
    {
        int ret = findFlag(argv[i], flags, sizeof(flags)/sizeof(char*));
        if (ret == -1)
        {
            printf("This flag doesn't exist or you entered number after the flag: %s\n", argv[i]);
            return -1;
        }
        else 
        {
            int findCbsInt = ret / 2;
            callback find = cbs[findCbsInt];
            if (checkNumber(argv[i-1])){
                printf("There must be a number");
                return ERROR_NAN;
            }
            
            if (chToInt(argv[i-1]) == ERROR_FULL && ((int)strlen(argv[i-1]) > 2)){
                printf("Your number is too big");
                return ERROR_FULL;
            }
            int arg = chToInt(argv[i-1]);

            //Обработка ошибок
            switch(find(arg)){
                case NOR_PRIME_NOR_COMPOSITE:
                    printf("The number isn't prime or composite");
                    break;
                case COMPOSITE:
                    printf("The number is composite");
                    break;
                case PRIME:
                    printf("The number is prime");
                    break;
                case ERROR_FULL:
                    printf("The number is too big");
                    return ERROR_FULL;
                case ERROR_NAN:
                    printf("Not a number");
                    break;
                case ERROR_LIMIT_H:
                    printf("This number is more than 100");
                    return ERROR_LIMIT_H;
                case ERROR_LIMIT_NAT_E:
                    printf("The number must be natural and less than 11");
                    return ERROR_LIMIT_NAT_E;
                case ERROR_LIMIT_NAT:
                    printf("The number must be natural");
                    break;
                case SUCCESS:
                    printf("");
            }

            i++;
        }
                
    }
    

    return 0;
}
