#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef enum StatusCode {
    SUCCESS = 0,
} StatusCode;

typedef struct Vector {
    double *coords;  
} Vector;

typedef double (*NormFunction)(Vector, int, ...);

double norm_inf(Vector vec, int n) {
    double max_val = fabs(vec.coords[0]);
    for (int i = 1; i < n; i++) {
        if (fabs(vec.coords[i]) > max_val) {
            max_val = fabs(vec.coords[i]);
        }
    }
    return max_val;
}

double norm_p(Vector vec, int n, double p) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += pow(fabs(vec.coords[i]), p);
    }
    return pow(sum, 1.0 / p);
}

double norm_matrix(Vector vec, int n, double *matrix) {
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result += matrix[i * n + j] * vec.coords[j] * vec.coords[i];
        }
    }
    return result;
}

StatusCode find_longest_vectors(int n, int num_vectors, Vector *vectors, int num_norms, ...) {
    va_list args;
    va_start(args, num_norms);

    for (int k = 0; k < num_norms; k++) {
        NormFunction norm_func = va_arg(args, NormFunction);
        double max_norm = -1;
        Vector *longest_vectors[num_vectors];
        int longest_count = 0;

        for (int i = 0; i < num_vectors; i++) {
            double norm_value;
            if (norm_func == (NormFunction)norm_inf) {
                norm_value = norm_func(vectors[i], n);
            } else if (norm_func == (NormFunction)norm_p) {
                norm_value = norm_func(vectors[i], n, 2.0);
            } else if (norm_func == (NormFunction)norm_matrix) {
                double A[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  
                norm_value = norm_func(vectors[i], n, A);
            }

            if (norm_value > max_norm) {
                max_norm = norm_value;
                longest_count = 0;
                longest_vectors[longest_count++] = &vectors[i];
            } else if (norm_value == max_norm) {
                longest_vectors[longest_count++] = &vectors[i];
            }
        }

        printf("Норма %d: Максимальное значение = %f\n", k + 1, max_norm);
        printf("Самые длинные векторы:\n");
        for (int j = 0; j < longest_count; j++) {
            int vector_index = longest_vectors[j] - vectors;
            printf("Вектор %d: (", vector_index + 1);
            for (int i = 0; i < n; i++) {
                printf("%f", longest_vectors[j]->coords[i]);
                if (i < n - 1) printf(", ");
            }
            printf(")\n");
        }
        printf("\n");
    }

    va_end(args);

    return SUCCESS;
}

Vector create_vector(int n, double coords[]) {
    Vector vec;
    vec.coords = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        vec.coords[i] = coords[i];
    }
    return vec;
}

StatusCode free_vector(Vector vec) {
    free(vec.coords);
    return SUCCESS;
}

int is_positive_integer(const char *str) {
    if (*str == '0') return 0;  
    while (*str) {
        if (*str < '0' || *str > '9') {
            return 0; 
        }
        str++;
    }
    return 1;
}

int is_number(const char *str) {
    int dot_seen = 0;

    if (*str == '-' || *str == '+') {
        str++;
    }

    while (*str) {
        if (*str == '.') {
            if (dot_seen) return 0; 
            dot_seen = 1;
        } else if (*str < '0' || *str > '9') {
            return 0; 
        }
        str++;
    }
    return 1; 
}

int main() {
    int num_vectors, n;

    char input[100];

    printf("Введите количество векторов: ");
    scanf("%s", input);
    while (!is_positive_integer(input)) {
        printf("Ошибка: введите положительное целое число больше нуля.\n");
        printf("Введите количество векторов: ");
        scanf("%s", input);
    }
    num_vectors = atoi(input);


    printf("Введите размерность каждого вектора: ");
    scanf("%s", input);
    while (!is_positive_integer(input)) {
        printf("Ошибка: введите положительное целое число больше нуля.\n");
        printf("Введите размерность каждого вектора: ");
        scanf("%s", input);
    }
    n = atoi(input);


    Vector *vectors = malloc(num_vectors * sizeof(Vector));


    for (int i = 0; i < num_vectors; i++) {
        double *coords = malloc(n * sizeof(double));
        printf("Введите координаты для вектора %d:\n", i + 1);
        for (int j = 0; j < n; j++) {
            printf("Координата %d: ", j + 1);
            scanf("%s", input);
            while (!is_number(input)) {
                printf("Ошибка: введите число (целое или вещественное).\n");
                printf("Координата %d: ", j + 1);
                scanf("%s", input);
            }
            coords[j] = atof(input);
        }
        vectors[i] = create_vector(n, coords);
        free(coords);  
    }

    find_longest_vectors(n, num_vectors, vectors, 3, norm_inf, norm_p, norm_matrix);

    for (int i = 0; i < num_vectors; i++) {
        free_vector(vectors[i]);
    }
    free(vectors); 

    return SUCCESS;
}
