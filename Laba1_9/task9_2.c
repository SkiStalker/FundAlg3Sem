#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef enum {
    SUCCESS = 0,
    ERROR_MEMORY_ALLOCATION = 1
} Eode;

int generate_random(int min, int max) {
    return min + rand() % (max - min + 1);
}

int find_closest(int value, int arr[], int size) {
    int closest = arr[0];
    int min_diff = abs(value - closest);

    for (int i = 1; i < size; i++) {
        int diff = abs(value - arr[i]);
        if (diff < min_diff) {
            closest = arr[i];
            min_diff = diff;
        }
    }

    return closest;
}

int main() {
    srand(time(NULL));
    int sizeA = generate_random(10, 10000); 
    int sizeB = generate_random(10, 10000);
    int *A = malloc(sizeA * sizeof(int));
    int *B = malloc(sizeB * sizeof(int));
    if (A == NULL || B == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(A);
        free(B);
        return ERROR_MEMORY_ALLOCATION;
    }
    for (int i = 0; i < sizeA; i++) {
        A[i] = generate_random(-1000, 1000);
    }
    for (int i = 0; i < sizeB; i++) {
        B[i] = generate_random(-1000, 1000);
    }
    int *C = malloc(sizeA * sizeof(int));
    if (C == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(A);
        free(B);
        return ERROR_MEMORY_ALLOCATION;
    }
    for (int i = 0; i < sizeA; i++) {
        int closest_value = find_closest(A[i], B, sizeB);
        C[i] = A[i] + closest_value;
    }
    printf("Array A:\n");
    for (int i = 0; i < sizeA; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("Array B:\n");
    for (int i = 0; i < sizeB; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    printf("Array C:\n");
    for (int i = 0; i < sizeA; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");
    free(A);
    free(B);
    free(C);

    return 0;
}