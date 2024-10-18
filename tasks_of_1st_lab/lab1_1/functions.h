#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_DIGITS 100 

enum ret_type_t{
    SUCCESS,    //Успех
    NOR_PRIME_NOR_COMPOSITE,    //result for H
    COMPOSITE,                  //result for H
    PRIME,                      //result for H
    ERROR_FULL, //Переполнение
    ERROR_NAN,   //Не число
    ERROR_NO_VALUES, //Недостаточно аргументов
    ERROR_LIMIT_H, //Нет чисел, кратных введенному числу меньше 100
    ERROR_LIMIT_NAT_E, //Число должно быть натуральным и меньшу 11
    ERROR_LIMIT_NAT   //Число должно быть натуральным
};
typedef int (*callback)(int);

//Перевод из строкового представления в int-овое
int chToInt(char* x){
    int xInt = 0;
    int m = 1;
    int mBuf = 1;

    for (int i = strlen(x) - 1; i >= 0; --i){
        xInt += (x[i] - '0') * m;
        m *= 10;
        if (mBuf > m){
            return ERROR_FULL;
        }
        mBuf *= 10;
    }
    return xInt;
}

//Проверка флага
int findFlag(char* qarg, const char** flags, int size){
    for (int i = 0; i < size; ++i)
    {
        if (!(strcmp(qarg, flags[i])))
        {
            return i;
        }
        
    }
   return -1; 
}

//Выводит все числа до 100, которые делятся на х
int funcForH(const int x){
    if ((x > 100) || (x < 1)){
        return ERROR_LIMIT_H;
    }
    int i = x;
    while (i <= 100){
        printf("%d\n", i);
        i += x;
    }
    return SUCCESS;
}

//Проверяет число х на простоту
int funcForP(const int x){
    if (x < 2){
        return NOR_PRIME_NOR_COMPOSITE;
    }
    else {
        for (int i = 2; i < (int)sqrt(x) + 1; i++){
            if (x % i == 0){
                return COMPOSITE;
            }
        }
    }
    return PRIME;
}

//Выводит цифры числа х в шестнадцатеричной системе счисления
int funcForS(const int x){
    int del = x;
    char digits[MAX_DIGITS];
    int count = 0;
    while(del != 0){
        int rem = del % 16;
        if (rem <= 9){
           digits[count++] = rem;
        }
        else{
           digits[count++] = rem - 10 + 'A';
        }
        del /= 16;
    }
    for (int i = count - 1; i >= 0; --i) {
        if (digits[i] <= 9) {
            printf("%d ", digits[i]);
        } else {
            printf("%c ", digits[i]);
        }
    }
    printf("\n");
    return SUCCESS;
}

//Выводит таблицу степеней
int funcForE(const int x){
    if (x > 10 || x == 0){
        return ERROR_LIMIT_NAT_E;
    }
    for (int i = 1; i <= x; i++){
        for (int j = 1; j < 11; j++){
            printf("%d^%d=%d ", j, i, (int)pow(j, i));
        }
        printf("\n");
    }

    return SUCCESS;
}

//Высчитывает сумму всех чисел от 1 до х
int funcForA(const int x){
    if (x < 1){
        printf("Your number must be natural");
        return ERROR_LIMIT_NAT;
    }
    printf("%d", (x*(x+1))/2);
    return SUCCESS;
}

//Вычисляет файкториал х
int funcForF(const int x){
    if (x <= 1){
        return 1;
    }
    else{
        int sum = 1;
        int prevSum = sum;
        for (int i = 2; i <= x; i++){
            sum *= i;
            if (sum <= prevSum){
                return ERROR_FULL;
            }
        }
        printf("%d", sum);
        return SUCCESS;
    }
}

//Проверяет является ли строковое представление числом
int checkNumber(char* x){
    for (int i = 0; i < (int)strlen(x); i++){
        if (x[i] < '0' || x[i] > '9'){
            return ERROR_NAN;
        }
    }
    return SUCCESS;
}
