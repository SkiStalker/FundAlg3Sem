#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ARRAY_SIZE 10  
#define MIN_DYNAMIC_SIZE 10
#define MAX_DYNAMIC_SIZE 10000


typedef enum Status {
    SUCCESS = 0,
    MEMORY_ALLOCATION_FAILURE = 1,
    INVALID_ARGUMENTS = 2,
    SIZE_OUT_OF_BOUNDS = 3,
    FILE_ERROR = 4
} Status;


Status validate_arguments(int argc, char *argv[], int *a, int *b) {
    if (argc != 3) {
        return INVALID_ARGUMENTS;
    }

    *a = atoi(argv[1]);
    *b = atoi(argv[2]);

    if (*a > *b) {
        return INVALID_ARGUMENTS;
    }

    return SUCCESS;
}


Status fill_array(int *arr, int size, int a, int b) {
    if (arr == NULL || size <= 0 || a > b) {
        return INVALID_ARGUMENTS;
    }

    for (int i = 0; i < size; i++) {
        arr[i] = a + rand() % (b - a + 1);
    }

    return SUCCESS;
}

Status find_and_swap_min_max(int *arr, int size) {
    if (arr == NULL || size <= 0) {
        return INVALID_ARGUMENTS;
    }

    int min_idx = 0, max_idx = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }


    int temp = arr[min_idx];
    arr[min_idx] = arr[max_idx];
    arr[max_idx] = temp;

    return SUCCESS;
}


int find_nearest(int a, const int *b_arr, int size) {
    if (b_arr == NULL || size <= 0) {
        return 0;  
    }

    int nearest = b_arr[0];
    int min_diff = abs(a - b_arr[0]);

    for (int i = 1; i < size; i++) {
        int diff = abs(a - b_arr[i]);
        if (diff < min_diff) {
            nearest = b_arr[i];
            min_diff = diff;
        }
    }

    return nearest;
}


Status free_memory(int *A, int *B, int *C) {
    if (A) {
        free(A);
    }
    if (B) { 
        free(B);
    }
    if (C) { 
        free(C);
    }
    return SUCCESS;
}

int main(int argc, char *argv[]) {
    int a, b;
    Status status;


    status = validate_arguments(argc, argv, &a, &b);
    if (status != SUCCESS) {
        printf("Error: invalid arguments. Usage: %s <a> <b>\n", argv[0]);
        return status;
    }

    srand(time(NULL));


    int arr[ARRAY_SIZE];

    status = fill_array(arr, ARRAY_SIZE, a, b);
    if (status != SUCCESS) {
        printf("Error: failed to fill array.\n");
        return status;
    }


    status = find_and_swap_min_max(arr, ARRAY_SIZE);
    if (status != SUCCESS) {
        printf("Error: failed to find and swap min and max.\n");
        return status;
    }

    printf("Array after swapping min and max:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");



    int dynamic_size = MIN_DYNAMIC_SIZE + rand() % (MAX_DYNAMIC_SIZE - MIN_DYNAMIC_SIZE + 1);

    int *A = (int *)malloc(dynamic_size * sizeof(int));
    int *B = (int *)malloc(dynamic_size * sizeof(int));
    int *C = (int *)malloc(dynamic_size * sizeof(int));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Error: memory allocation failed.\n");
        free_memory(A, B, C);
        return MEMORY_ALLOCATION_FAILURE;
    }

    status = fill_array(A, dynamic_size, -1000, 1000);
    if (status != SUCCESS) {
        printf("Error: failed to fill array A.\n");
        free_memory(A, B, C);
        return status;
    }

    status = fill_array(B, dynamic_size, -1000, 1000);
    if (status != SUCCESS) {
        printf("Error: failed to fill array B.\n");
        free_memory(A, B, C);
        return status;
    }


    for (int i = 0; i < dynamic_size; i++) {
        int nearest_in_B = find_nearest(A[i], B, dynamic_size);
        C[i] = A[i] + nearest_in_B;
    }


    printf("\nFirst 10 elements of array A:\n");
    for (int i = 0; i < 10 && i < dynamic_size; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("\nFirst 10 elements of array B:\n");
    for (int i = 0; i < 10 && i < dynamic_size; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    printf("\nFirst 10 elements of array C:\n");
    for (int i = 0; i < 10 && i < dynamic_size; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");


    free_memory(A, B, C);

    return SUCCESS;
}
