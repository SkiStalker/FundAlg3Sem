#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define min(x, y) (x <= y)? (x) : (y)
#define max(x, y) (x <= y)? (y) : (x)

/* typedef struct 
{
    double* data;
    int size;
    int count;
}vector;

vector init(vector v){
    v.count = 0;
    v.size = 4;
    v.data = (double*)malloc(sizeof(double) * v.size);
    return v;
}

int append(vector* v, double val){
    if(v->count == v->size){

    }
}
 */

typedef struct 
{
    double x, y;
}coord;

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

char* substr(char* to, char* from, int len, int begin){
    for (int i = begin, j = 0; j < len && from[i] != '\0'; i++, j++)
    {
        to[j] = from[i];
        to[j + 1] = '\0';
    }
    return to;
}

int is_Kaprekar(int base, int number, ...){
    if(base < 2 || base > 36){
        return -1;
    }
    va_list list;
    va_start(list, number);
    char buf[BUFSIZ];
    long long int number_in_10;
    for (int i = 0; i < number; i++)
    {
        strcpy(buf, va_arg(list, char*));
        if(strlen(buf) > 8){
            continue;
        }
        number_in_10 = gorner(buf, strlen(buf), base);
        char* in_base = x_from_10_to_n(number_in_10 * number_in_10, base);
        int len = strlen(in_base);
        for (int i = 1; i < len - 1; i++)
        {
            char* first_str = (char*)malloc(sizeof(char) * (i + 1));
            if(first_str == NULL){
                free(in_base);
                return -1;
            }
            char *second_str = (char*)malloc(sizeof(char) * (len - i + 1));
            if(second_str == NULL){
                free(in_base);
                return -1;
            }
            first_str = substr(first_str, in_base, i, 0);
            second_str = substr(second_str, in_base, len - i, i);
            long long sum = gorner(first_str, strlen(first_str), base) + gorner(second_str, strlen(second_str), base);
            if(sum == number_in_10){
                printf("%s\n", buf);
                free(first_str);
                free(second_str);
                break;
            }
            free(first_str);
            free(second_str);
        }
        free(in_base);
    }
    return 0;
}

long double polynomial(long double x, int n, ...){
    va_list list;
    va_start(list, n);
    long double res = va_arg(list, long double);
    for (int i = 0; i < n; i++)
    {
        res = res * x + va_arg(list, long double);
    }
    return res;
}

int is_convex(int n, ...){
    va_list list;
    va_start(list, n);
    int size = 4, counter = 0;
    coord* cds = (coord*)malloc(sizeof(coord) * size);

    for (int i = 0; i < n; i++)
    {
        if(size == counter){
            coord* cds_n = realloc(cds, size * 2);
            if(cds_n == NULL){
                free(cds);
                return -1;
            }
            size *= 2;
        }
        cds[counter].x = va_arg(list, double);
        cds[counter++].y = va_arg(list, double);
    }
    va_end(list);
    coord* cords = (coord*)malloc(sizeof(coord) * counter);

    for (int i = 0; i < counter; i++)
    {
        cords[i].x = cds[(i + 1) % counter].x - cds[i].x;
        cords[i].y = cds[(i + 1) % counter].y - cds[i].y;
        //printf("x: %f\ty: %f\n", cords[i].x, cords[i].y);
    }
    
    int flag_gtr = 0;
    if((cords[0].x * cords[1].y - cords[0].y * cords[1].x) > 0){
        int flag_gtr = 1;        
    }
    else if((cords[0].x * cords[1].y - cords[0].y * cords[1].x) < 0){
        int flag_gtr = 0; 
    }
    else{
        free(cords);
        free(cds);
        return 0;
    }
    for (int i = 0; i < counter; i++)
    {
        if((cords[i].x * cords[(i + 1) % counter].y - cords[i].y * cords[(i + 1) % counter].x) < 0 && flag_gtr){
            free(cords);
            free(cds);
            return 0;
        }
        if((cords[i].x * cords[(i + 1) % counter].y - cords[i].y * cords[(i + 1) % counter].x) > 0 && !flag_gtr){
            free(cords);
            free(cds);
            return 0;
        }
    }
    
    free(cords);
    free(cds);
    return 1;
}

int main(){
    int mode;
    scanf("%d", &mode);
    switch (mode)
    {
    case 1:
    {
        double coords[6];
        scanf("%lf %lf %lf %lf %lf %lf", &coords[0], &coords[1], &coords[2], &coords[3], &coords[4], &coords[5]);
        printf("%d\n", is_convex(3, coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]));
        break;
    }
        
    case 2:
    {
        double coords[8];
        scanf("%lf %lf %lf %lf %lf %lf %lf %lf", &coords[0], &coords[1], &coords[2], &coords[3], &coords[4], &coords[5], &coords[6], &coords[7]);
        printf("%d\n", is_convex(4, &coords[0], &coords[1], &coords[2], &coords[3], &coords[4], &coords[5], coords[6], coords[7]));
        break;
    }
    case 3:
    {
        long double x, f1, f2, f3;
        scanf("%Lf %Lf %Lf %Lf", &x, &f1, &f2, &f3);
        printf("%Lf\n", polynomial(x, 2, f1, f2, f3));
        break;
    }
    case 4:
    {
        for (int i = 0; i < 2; i++)
        {
            int sys;
            char num[15], num1[15], num2[15];
            scanf("%d %s %s %s", &sys, &num, &num1, &num2);
            printf("%d num system:\n", sys);
            is_Kaprekar(sys, 3, num, num1, num2);
        }
        
        
        break;
    }
    default:
        break;
    }
  
  /* printf("%Lf\n", polynomial(2.0, 3, 2.0, 3.0, 1.0, 1.0));
  is_Kaprekar(12, 1,"BBB"); */
}