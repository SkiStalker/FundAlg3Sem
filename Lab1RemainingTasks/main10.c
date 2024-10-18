#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

enum ret_t{
    SUCCESS,
    WRONG_S_N,
    MEMORY_ERROR,
    MUST_BE_A_NUMBER
};

typedef struct 
{
    int val;
    char* str;
}value;

char* x_from_10_to_n(int x, int n){
    char nw[BUFSIZ];
    int del = 0, iter = 0;
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
        return "0";
    }
}


long long int gorner(char* _str, int size, int base){
    char str[BUFSIZ];
    for (int i = 0; i < size; i++)
    {
        str[i] = _str[i];
    }
    str[size - 1] = '\0';
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

void error_handler(int ret){
    switch (ret)
    {
        case MEMORY_ERROR:
            printf("Memory error!\n");
            break;
        case WRONG_S_N:
            printf("Wrong number system!\n");
            break;
        case MUST_BE_A_NUMBER:
            printf("The argument must be a number!\n");
            break;
        default:
            break;
    }
}

int has_only_numbers(char* str, int siz){
    for (int i = 0; i < siz - 1; i++)
    {
        if(!isalnum(str[i]) && str[i] != '-'){
            return 0;
        }
    }
    return 1;
}

int is_upper_than_x(char* str, int siz, int num_sys){
    char chr = (num_sys <= 9)? '0' + num_sys : 'A' + num_sys;
    if(num_sys < 0 || num_sys > 36){
        return 0;
    }
    int i = (str[0] == '-')? 1: 0;
    for (; i < siz - 1; i++)
    {
        if(str[i] >= chr){
            return 1;
        }
    }
    return 0;
}

void func(char** strings, int cnt, int num_sys){
    value s;
    s.val = __INT32_MAX__ + 1;
    for (int i = 0; i < cnt; i++)
    {
        long long int v = gorner(strings[i], strlen(strings[i]), num_sys);
        if(s.val < v){
            s.val = v;
            s.str = strings[i];
        }
    }

    if(s.str[0] == '0' || (s.str[0] == '-' && s.str[1] == '0')){
        while(*s.str == '0'){
            s.str++;
        }
        char* save = s.str;
        while(*s.str != '\n'){
            s.str++;
        }
        *s.str = '\0';
        s.str = save;
        /* if(*s.str == '\n'){
            *--s.str = '0';
            *(s.str + 1) = '\0';
        } */
    }
    else{
        char* save = s.str;
        while(*s.str != '\n'){
            s.str++;
        }
        *s.str = '\0';
        s.str = save;
    }
    printf("%s\t", s.str);
    for (int i = 9; i <= 36; i = i + 9)
    {
        printf("%s\t", x_from_10_to_n(s.val, i));
    }
    
    for (int i = 0; i < cnt; i++)
    {
        free(strings[i]);
    }
    free(strings);
    return;
}

char** input(int num_sys, int* cnt, int* ret){
    int memo = 4;
    char** strings = (char**)malloc(sizeof(char*)*memo);
    char buffer[1024];
    buffer[0] = '\0';
    while(strcmp(buffer, "Stop\n")){
        fgets(buffer,1023,stdin);
        if(!has_only_numbers(buffer, (int)strlen(buffer))){
            for (int i = 0; i < *cnt; i++)
            {
                if(strings[i] != NULL){
                    free(strings[i]);
                }
            }
            *ret = MUST_BE_A_NUMBER;
            free(strings);
            return 0;
        }
        //printf("%d\n", !strcmp(buffer, "Stop\n"));
        if(!strcmp(buffer, "Stop\n")){
            break;
        }
        else if(strcmp(buffer, "Stop\n") && is_upper_than_x(buffer, (int)strlen(buffer), num_sys)){
            for (int i = 0; i < *cnt; i++)
            {
                if(strings[i] != NULL){
                    free(strings[i]);
                }
            }
            free(strings);
            *ret = WRONG_S_N;
            return 0;
        }

        char* str = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));

        strcpy(str, buffer);

        if(str == NULL){
            for (int i = 0; i < *cnt; i++)
            {
                if(strings[i] != NULL){
                    free(strings[i]);
                }
            }
            free(strings);
            *ret = MEMORY_ERROR;
            return 0;
        }
        strings[(*cnt)++] = str;
        if(*cnt == memo){
            memo *= 2;
            char** strings_new = (char**)realloc(strings, memo);
            if(strings_new == NULL){
                for (int i = 0; i < *cnt; i++)
                {
                    if(strings[i] != NULL){
                        free(strings[i]);
                    }
                }
                free(strings);
                *ret = MEMORY_ERROR;
                return 0;
            }
            strings = strings_new;
        }
    }
    return strings;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Wrong number of arguments!\n");
        return -1;
    }
    if(!has_only_numbers(argv[1], strlen(argv[1]))){
        printf("The argument must be a number!\n");
        return -1;
    }
    int ret = SUCCESS, cnt = 0;
    int num_sys = atoi(argv[1]);
    if(num_sys <= 1 || num_sys > 36){
        printf("The number system must be in range of [2, 36]\n!");
        return -1;
    }
    char** strings = input(num_sys, &cnt, &ret);
    if(ret != SUCCESS){
        error_handler(ret);
        return -1;
    }
    func(strings, cnt, num_sys);
    return 0;
}