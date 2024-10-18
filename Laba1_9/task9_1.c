#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_ARGS = 1,
    ERROR_INVALID_INPUT = 2
} Eode;

int generate_random(int min, int max) {
    return min + rand() % (max - min + 1);
}

int compare_integers(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int linear_search(int array[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (array[i] == target) {
            return i;
        }
    }
    return -1;
}

void process_array(int array[], int size, int min, int max) {
    for (int i = 0; i < size; i++) {
        array[i] = generate_random(min, max);
    }

    int *array_1 = malloc(size * sizeof(int));
    if (array_1 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        array_1[i] = array[i];
    }

    printf("Original array:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    qsort(array_1, size, sizeof(int), compare_integers);

    printf("Sorted array:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array_1[i]);
    }
    printf("\n");

    int min_value = array_1[0];
    int max_value = array_1[size - 1];
    int min_index = linear_search(array, size, min_value);
    int max_index = linear_search(array, size, max_value);

    if (min_index != -1 && max_index != -1) {
        int temp = array[min_index];
        array[min_index] = array[max_index];
        array[max_index] = temp;
    }

    printf("Modified array (min and max swapped):\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array_1);
}

int has_only_numbers(char* argument) {
    for (int i = 0; i < (int)strlen(argument); i++) {
        if (!((argument[i] >= '0' && argument[i] <= '9') || argument[i] == '.' || argument[i] == '-')) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Wrong amount of arguments!");
        return ERROR_INVALID_ARGS;
    }
    if (has_only_numbers(argv[1]) == 0 || has_only_numbers(argv[2]) == 0 || has_only_numbers(argv[3]) == 0){
        printf("Invalid input");
        return ERROR_INVALID_INPUT;
    }

    int array_size = atoi(argv[1]);
    int min_value = atoi(argv[2]);
    int max_value = atoi(argv[3]);

    if (array_size <= 0 || min_value > max_value) {
        printf("Invalid input: array size must be positive and min_value should be less than max_value.\n");
        return ERROR_INVALID_INPUT;
    }

    srand(time(NULL));

    int *array = malloc(array_size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return ERROR_INVALID_INPUT;
    }

    process_array(array, array_size, min_value, max_value);

    free(array);

    return SUCCESS;
}