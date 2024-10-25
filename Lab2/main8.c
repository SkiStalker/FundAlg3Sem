#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long long int gorner(char* _str, int size, int base){
    char str[BUFSIZ];
    for (int i = 0; i < size; i++)
    {
        str[i] = _str[i];
    }
    str[size] = '\0';
    long long int res = 0;
    char *ps = str;
    ps = str - 1;
    
    if(*(ps + 1) == '-'){
        ++ps;
    }
    while(*++ps){
        res = (res * base) + ((isalpha(*ps))? *ps - 'A' + 10: *ps - '0');
    }
    return str[0] != '-'? res : -res;
}

char* x_from_10_to_n(int x, int n){
    char nw[BUFSIZ];
    int del = 0, iter = 0, flag = 0;
    if(x < 0){
        //nw[iter++] = '-';
        flag = 1;
        x = -x;
    }
    while(x != 0){
        del = x % n;
        x = x / n;
        if(del >= 0 && del <= 9){
            nw[iter++] = del + '0';
        }
        else if(del > 9)
        {
            nw[iter++] = del + 'A' - 10;
        }
    }
    if(flag == 1){
        nw[iter++] = '-';
    }
    nw[iter] = '\0';
    char* ans = (char*)malloc(sizeof(char)*iter);
    if(ans != NULL){
        for (int i = iter - 1, j = 0; i >= 0; i--, j++)
        {
            ans[j] = nw[i];
        }
        ans[iter] = '\0';
        return ans;
    }
    else{
        char *s;
        *s = '0';
        return s;
    }
}

char* sum_in_ns(int num_sys, int number_of_args, ...){
    if(num_sys > 36 || num_sys < 2){
        return x_from_10_to_n(-1, num_sys);
    }
    va_list list;
    va_start(list, number_of_args);
    long long int sum = 0;
    char buf[BUFSIZ];
    for (int i = 0; i < number_of_args; i++)
    {
        strcpy(buf, va_arg(list, char*));
        long long int num = gorner(buf, strlen(buf), num_sys);
        sum += num;
    }
    va_end(list);
    return x_from_10_to_n(sum, num_sys);
}

int main(){
    int mode;
    char* ans;
    scanf("%d", &mode); //Test mode
    switch (mode)
    {
    case 1:
        ans = sum_in_ns(16, 3, "FF", "-FF", "10FA");
        break;
    case 2:
        ans = sum_in_ns(10, 3, "1", "9", "10");
        break;
    case 3:
        ans = sum_in_ns(2, 3, "1", "1", "10");
        break;
    default:
        break;
    }
    
    printf("%s\n", ans);
    free(ans);
    return 0;
}