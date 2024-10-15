// lb1task9.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>


void fill_fixed_array(int* array, int size, int a, int b) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % (b - a + 1) + a;
    }
}


void find_and_swap_min_max(int* array, int size) {
    if (size <= 0) return;

    int min_index = 0, max_index = 0;
    int min_value = array[0], max_value = array[0];

    for (int i = 1; i < size; i++) {
        if (array[i] < min_value) {
            min_value = array[i];
            min_index = i;
        }
        if (array[i] > max_value) {
            max_value = array[i];
            max_index = i;
        }
    }

    if (min_index != max_index) {
        int temp = array[min_index];
        array[min_index] = array[max_index];
        array[max_index] = temp;
    }
}


void bubble_sort(int* array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}


int find_nearest(int value, int* array, int size) {
    bubble_sort(array, size);

    int left = 0, right = size - 1;
    int nearest_index = 0;
    int nearest_diff = abs(array[0] - value);

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int diff = abs(array[mid] - value);

        if (diff < nearest_diff) {
            nearest_diff = diff;
            nearest_index = mid;
        }

        if (array[mid] < value) {
            left = mid + 1;
        }
        else if (array[mid] > value) {
            right = mid - 1;
        }
        else {
            return array[mid];
        }
    }


    return array[nearest_index];
}

int* create_dynamic_array(int* size) {
    *size = rand() % (10000 - 10 + 1) + 10;
    int* array = (int*)malloc(*size * sizeof(int));
    if (!array) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *size; i++) {
        array[i] = rand() % 2001 - 1000;
    }
    return array;
}

int is_number(const char* str) {
    if (*str == '\0') {
        return 0;
    }
    if (*str == '-') {
        str++; 
    }
    if (*str == '\0') {
        return 0; 
    }
    for (const char* p = str; *p != '\0'; p++) {
        if (!isdigit(*p)) {
            return 0; 
        }
    }
    return 1; 
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Not enough arguments\n");
        return 1;
    }

    if (!is_number(argv[1]) || !is_number(argv[2])) {
        printf("Error: a and b must be valid integers.\n");
        return 1;
    }

    long a = strtol(argv[1], NULL, 10);
    long b = strtol(argv[2], NULL, 10);
    srand(time(NULL));
    if (a >= b) {
        printf("Error: a must be less than b.\n");
        return 1;
    }

    if (a < INT_MIN || b > INT_MAX || a > INT_MAX || b < INT_MIN) {
        printf("Error: a and b must be within the range of int.\n");
        return 1;
    }

    // Задача 1 
    int fixed_array[10];
    fill_fixed_array(fixed_array, 10, a, b);

    printf("Fixed array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", fixed_array[i]);
    }
    printf("\n");

    find_and_swap_min_max(fixed_array, 10);

    printf("Array after swapping min and max: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", fixed_array[i]);
    }
    printf("\n");

    // Задача 2
    int sizeA, sizeB;
    int* A = create_dynamic_array(&sizeA);
    int* B = create_dynamic_array(&sizeB);

    int* C = (int*)malloc(sizeA * sizeof(int));
    if (!C) {
        perror("Failed to allocate memory for C");
        free(A);
        free(B);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < sizeA; i++) {
        C[i] = A[i] + find_nearest(A[i], B, sizeB);
    }

    printf("Dynamic array C: ");
    for (int i = 0; i < sizeA; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");
    free(A);
    free(B);
    free(C);
    return 0;
}

