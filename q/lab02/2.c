#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define MAX_ARRAY_SIZE 1024
#define MAX_LINE_LENGTH 1024

typedef enum StatusCode {
    SUCCESS = 0,
    ERROR,
    ERROR_FILE_OPEN,
    ERROR_FILE_SAVE,
    ERROR_MEMORY_ALLOCATION
} StatusCode;

typedef struct Array {
    int *data;
    size_t size;
} Array;

int cmp_asc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int cmp_desc(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

StatusCode load_array(Array *arr, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file for loading.\n");
        return ERROR_FILE_OPEN;
    }
    arr->size = 0;
    arr->data = malloc(MAX_ARRAY_SIZE * sizeof(int));
    if (!arr->data) {
        perror("Memory allocation error.\n");
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \t\n");  
        while (token != NULL) {
            char *endptr;
            int value = strtol(token, &endptr, 10);
            if (*endptr == '\0') {
                if (arr->size >= MAX_ARRAY_SIZE) {
                    fclose(file);
                    return SUCCESS;
                }
                arr->data[arr->size++] = value;
            }
            token = strtok(NULL, " \t\n"); 
        }
    }
    fclose(file);
    return SUCCESS;
}


StatusCode save_array(const Array *arr, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for saving.\n");
        return ERROR_FILE_SAVE;
    }
    for (size_t i = 0; i < arr->size; i++) {
        fprintf(file, "%d ", arr->data[i]);
    }
    fclose(file);
    return SUCCESS;
}

StatusCode rand_array(Array *arr, size_t count, int lb, int rb) {
    arr->data = realloc(arr->data, count * sizeof(int));
    if (!arr->data) {
        perror("Memory allocation error.\n");
        return ERROR_MEMORY_ALLOCATION;
    }
    arr->size = count;
    for (size_t i = 0; i < count; i++) {
        arr->data[i] = lb + rand() % (rb - lb + 1);
    }
    return SUCCESS;
}

StatusCode concat_arrays(Array *arrA, const Array *arrB) {
    arrA->data = realloc(arrA->data, (arrA->size + arrB->size) * sizeof(int));
    if (!arrA->data) {
        perror("Memory allocation error.\n");
        return ERROR_MEMORY_ALLOCATION;
    }
    memcpy(arrA->data + arrA->size, arrB->data, arrB->size * sizeof(int));
    arrA->size += arrB->size;
    return SUCCESS;
}

StatusCode free_array(Array *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    return SUCCESS;
}

StatusCode remove_elements(Array *arr, size_t start, size_t count) {
    if (start >= arr->size) {
        return ERROR;
    }
    size_t end = start + count < arr->size ? start + count : arr->size;
    memmove(arr->data + start, arr->data + end, (arr->size - end) * sizeof(int));
    arr->size -= (end - start);
    return SUCCESS;
}

StatusCode copy_elements(const Array *arrA, size_t start, size_t end, Array *arrB) {
    if (start > end || end >= arrA->size) {
        return ERROR;
    }
    size_t count = end - start + 1;
    arrB->data = realloc(arrB->data, count * sizeof(int));
    if (!arrB->data) {
        perror("Memory allocation error.\n");
        return ERROR_MEMORY_ALLOCATION;
    }
    arrB->size = count;
    memcpy(arrB->data, arrA->data + start, count * sizeof(int));
    return SUCCESS;
}

StatusCode sort_array(Array *arr, int descending) {
    qsort(arr->data, arr->size, sizeof(int), descending ? (int (*)(const void *, const void *)) cmp_desc : (int (*)(const void *, const void *)) cmp_asc);
    return SUCCESS;
}

StatusCode shuffle_array(Array *arr) {
    for (size_t i = 0; i < arr->size; i++) {
        size_t j = rand() % arr->size;
        int temp = arr->data[i];
        arr->data[i] = arr->data[j];
        arr->data[j] = temp;
    }
    return SUCCESS;
}

StatusCode print_stats(const Array *arr) {
    if (arr->size == 0) {
        printf("Array is empty.\n");
        return ERROR;
    }
    int min = arr->data[0], max = arr->data[0];
    size_t min_idx = 0, max_idx = 0;
    int sum = 0;
    int freq[10000] = {0};
    int most_freq = arr->data[0];
    for (size_t i = 0; i < arr->size; i++) {
        if (arr->data[i] < min) {
            min = arr->data[i];
            min_idx = i;
        }
        if (arr->data[i] > max) {
            max = arr->data[i];
            max_idx = i;
        }
        sum += arr->data[i];
        freq[arr->data[i]]++;
        if (freq[arr->data[i]] > freq[most_freq] || (freq[arr->data[i]] == freq[most_freq] && arr->data[i] > most_freq)) {
            most_freq = arr->data[i];
        }
    }
    double avg = sum / (double)arr->size;
    double max_deviation = 0;
    for (size_t i = 0; i < arr->size; i++) {
        double deviation = fabs(arr->data[i] - avg);
        if (deviation > max_deviation) {
            max_deviation = deviation;
        }
    }
    printf("Size: %zu\n", arr->size);
    printf("Min: %d (index: %zu)\n", min, min_idx);
    printf("Max: %d (index: %zu)\n", max, max_idx);
    printf("Most frequent: %d\n", most_freq);
    printf("Average: %.2f\n", avg);
    printf("Max deviation: %.2f\n", max_deviation);
    return SUCCESS;
}

StatusCode print_array(const Array *arr, size_t start, size_t end) {
    if (end >= arr->size) {
        end = arr->size - 1;
    }
    for (size_t i = start; i <= end; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    return SUCCESS;
}


int main() {
    Array arrays[26] = {0};
    if (load_array(&arrays[0], "in.txt") != SUCCESS) {
        printf("Ошибка загрузки массива из файла.\n");
        return ERROR_FILE_OPEN;
    }
    if (save_array(&arrays[0], "out.txt") != SUCCESS) {
        printf("Ошибка сохранения массива в файл.\n");
        return ERROR_FILE_SAVE;
    }
    if (rand_array(&arrays[1], 10, 1, 100) != SUCCESS) {
        printf("Ошибка генерации случайного массива.\n");
        return ERROR_MEMORY_ALLOCATION;
    }
    if (concat_arrays(&arrays[0], &arrays[1]) != SUCCESS) {
        printf("Ошибка конкатенации массивов.\n");
        return ERROR_MEMORY_ALLOCATION;
    }
    free_array(&arrays[1]);
    if (remove_elements(&arrays[0], 2, 5) != SUCCESS) {
        printf("Ошибка удаления элементов из массива.\n");
        return ERROR;
    }
    if (copy_elements(&arrays[0], 1, 4, &arrays[2]) != SUCCESS) {
        printf("Ошибка копирования элементов в массив.\n");
        return ERROR;
    }
    if (sort_array(&arrays[0], 0) != SUCCESS) {
        printf("Ошибка сортировки массива.\n");
        return ERROR;
    }
    print_stats(&arrays[0]);
    print_array(&arrays[0], 0, arrays[0].size - 1);
    shuffle_array(&arrays[0]);
    print_array(&arrays[0], 0, arrays[0].size - 1);
    for (int i = 0; i < 26; i++) {
        free_array(&arrays[i]);
    }
    return SUCCESS;
}
