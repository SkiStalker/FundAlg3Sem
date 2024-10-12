#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

enum ret_type_t{
    FLAG_ERROR = -1000,
    SUCCESS = 0,
    BREAK,
    CONTINUE,
    ERROR,
    MEMORY_ERROR,
    ZERO_DIVISION_ERROR,
    MULTIPLE,
    N_MULTIPLE,
    RECTANGLE,
    N_RECTANGLE,
    MUST_BE_A_NUMBER_ERROR,
    MUST_BE_A_NUMBER_ERROR_BREAK,
    MUST_BE_BIGGER_THAN_ZERO,
    MUST_BE_A_NON_ZERO_INT,
    EPS_MUST_NOT_BE_ZERO
};

typedef int (*callback)(double*);

int has_only_numbers(char* arg){
    for (int i = 0; i < (int)strlen(arg); i++)
    {
        if(!((arg[i] >= '0' && arg[i] <= '9') || arg[i] == '.' || arg[i] == '-')){
            return 0;
        }
    }
    return 1;
}

int findflag(char* qarg, const char** flags, int size){
    for (int i = 0; i < size; i++)
    {
        if(!strcmp(qarg, flags[i])){
            return qarg[1];
        }
    }
    return FLAG_ERROR;
}

int find_X2(double epsilon, double a, double b, double c){
    if(a == 0 && b != 0){
        printf("%fx^2 + %fx + %f = 0\nx1 = x2 = %f\n\n", a, b, c, -c / b);
        return SUCCESS;
    }
    else if(a == 0 && b == 0 && c == 0){
        printf("%fx^2 + %fx + %f = 0\nNo solvings!\n\n", a, b, c);
        return SUCCESS;
    }
    double discriminant = (b * b) - (4 * a * c);
    if(discriminant < 0){
        discriminant = discriminant * -1;
        printf("%fx^2 + %fx + %f = 0\nNo solvings in R! Only in C.\nx1/x2 = (-%f +- i(sqrt(%f))) / (2 * %f)\n\n", a, b, c, b, discriminant, a);
        return SUCCESS;
    }
    double x1 = (-b + sqrt(discriminant)) / (2 * a);
    double x2 = (-b - sqrt(discriminant)) / (2 * a);
    if(fabs(x1 - x2) < epsilon){
        printf("%fx^2 + %fx + %f = 0\nx1 = x2 = %f\n\n", a, b, c, x1);
        return SUCCESS;
    }
    printf("%fx^2 + %fx + %f = 0\nx1 = %f, x2 = %f\n\n", a, b, c, x1, x2);
    return SUCCESS;
}

int dbl_cmp(double epsilon, double x1, double x2){
    if(fabs(x1 - x2) < epsilon){
        return 1;
    }
    else{
        return 0;
    }
}

int func_for_q(double epsilon, double x1, double x2, double x3){
    double x[] = {x1, x2, x3};
    double combinations[6][3];
    double combination[3];
    int count = 0;
    for (int k = 0; k < 3; k++)
    {
        for (int i = 1; i <= 2; i++)
        {
            for (int j = k, n = 0; n < 3; j = (j + i) % 3, n++)
            {
                combination[n] = x[j % 3];
            }
            int flag = 0;
            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if(dbl_cmp(epsilon, combination[j], combinations[i][j])){
                        flag++;
                    }
                }
                if(flag == 3){
                    flag = -1;
                    break;
                }
                else{
                    flag = 0;
                }
            }
            if(!flag){
                find_X2(epsilon, combination[0], combination[1], combination[2]);
                for (int i = 0; i < 3; i++)
                {
                    combinations[count][i] = combination[i];
                }
                count++;
            }
            else{
                break;
            }


        }

    }

    return SUCCESS;
}

int is_miltiple(int x1, int x2){

    if(x1 % x2 == 0){

        return MULTIPLE;
    }
    else{
        return N_MULTIPLE;
    }
}

int is_int(const char *str) {
    if (*str == '\0' || *str == '0') return MUST_BE_A_NON_ZERO_INT;
    if (*str == '-') {
        str++;
    }

    if (*str == '\0') return MUST_BE_A_NON_ZERO_INT;

    while (*str) {
        if (!isdigit(*str)) {
            return MUST_BE_A_NON_ZERO_INT;
        }
        str++;
    }
    return SUCCESS;
}

int is_rectangular(double epsilon, double a, double b, double c){
    if(dbl_cmp(epsilon, 0.0, a) || dbl_cmp(epsilon, 0.0, b) || dbl_cmp(epsilon, 0.0, c) || a < 0 || b < 0 || c < 0){
        return MUST_BE_BIGGER_THAN_ZERO;
    }
    if(dbl_cmp(epsilon, a * a + b * b, c * c) || dbl_cmp(epsilon, a * a + c * c, b * b) || dbl_cmp(epsilon, b * b + c * c, a * a)){

        return RECTANGLE;
    }
    else{

        return N_RECTANGLE;
    }
}

double* checking_for_right(const char** flags, char* argv[], int argc, int* i, int right_amount, int* rt, int arg_size){
    int ii = (*i) + 1, counter = 0;
    (*rt) = SUCCESS;
    while(ii < argc){

        if(findflag(argv[ii], flags, arg_size) == FLAG_ERROR){

            ++counter;
            ++ii;
            continue;
        }
        else{
            break;
        }
    }
    if(counter != right_amount){
        printf("The wrong amount of arguments! Must be %d for %s key!\n", right_amount, argv[(*i)]);
        if((*i) + counter < argc){
            (*i) = (*i) + counter;
        }
        else{
            (*rt) = BREAK;
            return NULL;
        }
        (*rt) = CONTINUE;
        return NULL;
    }
    (*i)++;
    double* argq = (double*)malloc(sizeof(double)*right_amount);
    if(argq == NULL){
        (*rt) = ERROR;
        return NULL;
    }
    int flag = 0;
    for (int j = (*i); j < (*i) + right_amount; j++)
    {
        if(has_only_numbers(argv[j])){
            argq[j - (*i)] = atof(argv[j]);
        }
        else{
            (*rt) = MUST_BE_A_NUMBER_ERROR_BREAK;
            flag = 1;
            break;
        }
    }
    if(flag){
        (*i) = (*i) + right_amount - 1;
        return NULL;
    }
    (*i) = (*i) + right_amount - 1;
    return  argq;
}



int main(int argc, char* argv[]){
    const char* flags[] = {"-q", "/q", "-m", "/m", "-t", "/t"};

    if (argc < 2) {
        printf("Incorrect args count\n");
        return ERROR;
    }

    for (int i = 1; i < argc; i++)
    {
        int ret = findflag(argv[i], flags, sizeof(flags)/sizeof(char*));
        if(ret == FLAG_ERROR){
            printf("NOT EXISTING FLAG - %s\n", argv[i]);
            return -1;
        }
        else{
            switch ((char)ret)
            {
                case 'q':
                {
                    int rt = SUCCESS;
                    double* argq = checking_for_right(flags, argv, argc, &i, 4, &rt, sizeof(flags)/sizeof(char*));
                    if (!rt){
                        if (fabs(atof(argv[2]) - 0.0) < 0.000000001){
                            printf("eps must not be zero\n");
                            free(argq);
                            return EPS_MUST_NOT_BE_ZERO;
                        }
                    }
                    if(rt == BREAK){
                        free(argq);
                        return rt;
                    }
                    else if(rt == MUST_BE_A_NUMBER_ERROR_BREAK){
                        printf("The argument must be a number!\n");
                        free(argq);
                        return rt;
                    }
                    else if(rt == CONTINUE){
                        free(argq);
                        return rt;
                    }
                    else if(rt == ERROR){
                        printf("Memory error!\n");
                        free(argq);
                        return rt;
                    }
                    func_for_q(argq[0], argq[1], argq[2], argq[3]);
                    free(argq);
                    break;
                }

                case 'm':
                {
                    int rt = SUCCESS;
                    double* argq = checking_for_right(flags, argv, argc, &i, 2, &rt, sizeof(flags)/sizeof(char*));
                    if (!rt){
                        if (is_int(argv[2]) == MUST_BE_A_NON_ZERO_INT || is_int(argv[3]) == MUST_BE_A_NON_ZERO_INT){
                            printf("Must be a non_zero integer\n");
                            free(argq);
                            return MUST_BE_A_NON_ZERO_INT;
                        }
                    }
                    if(rt == BREAK){
                        free(argq);
                        return rt;
                    }
                    else if(rt == CONTINUE){
                        free(argq);
                        return rt;
                    }
                    else if(rt == ERROR){
                        printf("Memory error!\n");
                        free(argq);
                        return rt;
                    }
                    else if(rt == MUST_BE_A_NUMBER_ERROR_BREAK){
                        printf("The argument must be a number!\n");
                        free(argq);
                        return rt;
                    }
                    int x1 = atoi(argv[2]), x2 = atoi(argv[3]);
                    int r = is_miltiple(x1, x2);
                    if(r == MULTIPLE){
                        printf("The first number is a multiple of the second.\n");
                    }
                    else if(r == N_MULTIPLE){
                        printf("The first numbers is not multiple of the second.\n");
                    }
                    free(argq);
                    break;
                }
                case 't':
                {
                    int rt = SUCCESS;
                    double* argq = checking_for_right(flags, argv, argc, &i, 4, &rt, sizeof(flags)/sizeof(char*));
                    if (!rt){
                        if (fabs(atof(argv[2]) - 0.0) < 0.000000001){
                            printf("eps must not be zero\n");
                            free(argq);
                            return EPS_MUST_NOT_BE_ZERO;
                        }
                    }
                    if(rt == BREAK){
                        free(argq);
                        return rt;
                    }
                    else if(rt == MUST_BE_A_NUMBER_ERROR_BREAK){
                        printf("The argument must be a number!\n");
                        free(argq);
                        return rt;
                    }
                    else if(rt == CONTINUE){
                        free(argq);
                        return rt;
                    }
                    else if(rt == ERROR){
                        printf("Memory error!\n");
                        free(argq);
                        return rt;
                    }

                    int r = is_rectangular(argq[0], argq[1], argq[2], argq[3]);
                    if(r == RECTANGLE){
                        printf("It can be a rectangular.\n");
                    }
                    else if(r == N_RECTANGLE){
                        printf("It can't be a rectangular.\n");
                    }
                    else if(r == MUST_BE_BIGGER_THAN_ZERO){
                        printf("Sides must be bigger than 0\n");
                    }

                    free(argq);
                    break;
                }
            }
        }
    }

    return 0;
}