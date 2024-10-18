#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define min(a, b) ((a) <= (b))? a : b
#define max(a, b) ((a) <= (b))? b : a

enum ret_t{
    SUCCESS,
    MEMORY_ERROR
};

typedef struct{
    int index;
    int val;
}elem;

int has_only_numbers(char* str, int siz){
    for (int i = 0; i < siz; i++)
    {
        if(!isdigit(str[i]) && str[i] != '-'){
            return 0;
        }
    }
    return 1;
}

void int_array_cmpl(int** arr, int siz, int r_beg, int r_end){
    for (int i = 0; i < siz; i++)
    {
        int r_int = rand();
        (*arr)[i] = r_int % (r_end - r_beg + 1) + r_beg;
    }
}

int compare(const void* a, const void* b){
    int d1 = *(int*)a;
    int d2 = *(int*)b;
    if(d1 < d2){
        return -1;
    }
    else if(d1 == d2){
        return 0;
    }
    else{
        return 1;
    }
}

int binary_search(int* arr, int size, int val){
    int start = 0, end = size - 1, mid = (start + end) / 2;
    while (start <= end){
        mid = (start + end) / 2;
        if (val == arr[mid]){
            return mid;
        }
        if (val < arr[mid]){
            end = mid - 1;
        }
        else{
            start = mid + 1;
        }
    }
    return mid;
}

int func_for_second(){
    /* int a_siz = rand() % (9991) + 1; //раскомментарить до сдачи
    int b_siz = rand() % (9991) + 1; */
    int a_siz = 10, b_siz = 10;

    int* a_array = (int*)malloc(sizeof(int)*a_siz);
    int* b_array = (int*)malloc(sizeof(int)*b_siz);

    if(a_array == NULL && b_array != NULL){
        free(b_array);
        return MEMORY_ERROR;
    }
    else if(a_array != NULL && b_array == NULL){
        free(a_array);
        return MEMORY_ERROR;
    }
    else if(a_array == NULL && b_array == NULL){
        free(a_array);
        free(b_array);
        return MEMORY_ERROR;
    }

    int_array_cmpl(&a_array, a_siz, -1000, 1000);
    int_array_cmpl(&b_array, b_siz, -1000, 1000);

    printf("First array:\t");
    for (int i = 0; i < a_siz; i++)
    {
        printf("%d\t", a_array[i]);
    }
    printf("\nSecond array:\t");

    for (int i = 0; i < b_siz; i++)
    {
        printf("%d\t", b_array[i]);
    }
    printf("\n");
    
    //qsort(a_array, a_siz, sizeof(int), compare);
    qsort(b_array, b_siz, sizeof(int), compare);

    int* c_array = (int*)malloc(sizeof(int)*a_siz);
    if(c_array == NULL){
        free(a_array);
        free(b_array);
        free(c_array);
        return MEMORY_ERROR;
    }

    for (int i = 0; i < a_siz; i++)
    {
        c_array[i] = a_array[i] + b_array[binary_search(b_array, b_siz, a_array[i])];
    }
    
    printf("The resulting array:\t");
    for (int i = 0; i < a_siz; i++)
    {
        printf("%d\t", c_array[i]);
    }
    printf("\n");
    free(a_array);
    free(b_array);
    free(c_array);
    return 0;
    /* printf("%d  %d\n", a_array[4], b_array[binary_search(b_array, b_siz, a_array[4])]); */
}

int func_for_first(int a, int b){
    srand ( time(NULL)); 
    elem maxim;
    elem minim;
    int array[N];
    for (int i = 0; i < N; i++)
    {
        array[i] = rand() % (b - a + 1) + a;
    }
    minim.val = __INT32_MAX__;
    maxim.val = __INT32_MAX__ + 1;

    printf("Static array before:\t");

    for (int i = 0; i < N; i++)
    {
        printf("%d\t", array[i]);
    }
    printf("\n");

    for (int i = 0; i < N; i++)
    {
        if(minim.val > array[i]){
            minim.val = array[i];
            minim.index = i;
        }
        if(maxim.val < array[i]){
            maxim.val = array[i];
            maxim.index = i;
        }
    }
    array[maxim.index] = minim.val;
    array[minim.index] = maxim.val;

    printf("Static array after:\t");

    for (int i = 0; i < N; i++)
    {
        printf("%d\t", array[i]);
    }
    printf("\n");
    return 0;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Wrong amount of arguments!\n");
        return -1;
    }
    int a, b;
    //printf("%d  %d\n", has_only_numbers(argv[1], strlen(argv[1])), has_only_numbers(argv[2], strlen(argv[2])));
    if(has_only_numbers(argv[1], strlen(argv[1])) && has_only_numbers(argv[2], strlen(argv[2]))){
        a = atoi(argv[1]);
        b = atoi(argv[2]);
    }
    else{
        printf("All arguments must be integers!\n");
        return -1;
    }
    if(b < a){ //мб тут надо выводить ошибку
        printf("Wrong borders of the array! 'A' must be less than 'B'!\n");
        return -1;
    }
    else if(a == b){
        printf("Arguments mustn't be equal!\n");
        return -1;
    }
    func_for_first(a, b);
    func_for_second();
    return 0;
}