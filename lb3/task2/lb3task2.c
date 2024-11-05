#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define EPSILON 1e-10

typedef enum {
    SUCCESS = 0,
    NULL_POINTER_ERROR,
    DIMENSION_ERROR,
    MEMORY_ALLOCATION_ERROR,
    MATRIX_ERROR
} ErrorCode;

typedef struct {
    int dimension;
    double* coordinates;
} Vector;

// Функция для вычисления нормы по бесконечности
double infinity_norm(const Vector* v) {
    if (v == NULL || v->coordinates == NULL) {
        return -1;
    }
    double max = fabs(v->coordinates[0]);
    for (int i = 1; i < v->dimension; i++) {
        double abs_value = fabs(v->coordinates[i]);
        if (abs_value > max) {
            max = abs_value;
        }
    }
    return max;
}

// Функция для вычисления p-нормы
double p_norm(const Vector* v, double p) {
    if (v == NULL || v->coordinates == NULL || p < 1.0) {
        return -1; // Ошибка
    }
    double sum = 0.0;
    for (int i = 0; i < v->dimension; i++) {
        sum += pow(fabs(v->coordinates[i]), p);
    }
    return pow(sum, 1.0 / p);
}

// Функция для вычисления A-нормы
double a_norm(const Vector* v, double** A) {
    if (v == NULL || v->coordinates == NULL || A == NULL) {
        return -1;
    }

    double dot_product = 0.0;
    double* Ax = (double*)malloc(v->dimension * sizeof(double));
    if (Ax == NULL) {
        return -1; // Ошибка выделения памяти
    }

    for (int i = 0; i < v->dimension; i++) {
        Ax[i] = 0;
        for (int j = 0; j < v->dimension; j++) {
            Ax[i] += A[i][j] * v->coordinates[j];
        }
    }

    for (int i = 0; i < v->dimension; i++) {
        dot_product += Ax[i] * v->coordinates[i];
    }

    free(Ax);
    return sqrt(dot_product);
}

// Создание положительно определенной матрицы
double** create_positive_definite_matrix(int n) {
    double** B = (double**)malloc(n * sizeof(double*));
    if (B == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        B[i] = (double*)malloc(n * sizeof(double));
        if (B[i] == NULL) {
            for (int k = 0; k < i; k++) free(B[k]);
            free(B);
            return NULL;
        }
        for (int j = 0; j < n; j++) {
            B[i][j] = (i + 1) * (j + 1);
        }
    }

    double** A = (double**)malloc(n * sizeof(double*));
    if (A == NULL) {
        for (int i = 0; i < n; i++) free(B[i]);
        free(B);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        if (A[i] == NULL) {
            for (int k = 0; k <= i; k++) free(A[k]);
            free(A);
            for (int i = 0; i < n; i++) free(B[i]);
            free(B);
            return NULL;
        }
        for (int j = 0; j < n; j++) {
            A[i][j] = 0;
            for (int k = 0; k < n; k++) {
                A[i][j] += B[k][i] * B[k][j];
            }
        }
    }

    for (int i = 0; i < n; i++) free(B[i]);
    free(B);

    return A;
}

// Вывод длиннейших векторов
void print_max_vector(Vector* v, int count, int norm_type) {
    printf("For norm %d:\n", norm_type);
    for (int i = 0; i < count; i++) {
        printf("Vector with coordinates: (");
        for (int j = 0; j < v[i].dimension; j++) {
            printf("%lf ", v[i].coordinates[j]);
        }
        printf(")\n");
    }
}

// Функция для нахождения длиннейших векторов
ErrorCode find_longest_vectors(int n, double (*inf_fun)(const Vector*), double (*p_fun)(const Vector*, double), double (*a_fun)(const Vector*, double**), int vector_count, ...) {
    if (n <= 0 || vector_count <= 0) {
        return DIMENSION_ERROR;
    }

    va_list args;
    double maxx1 = 0, maxx2 = 0, maxx3 = 0;
    int count_m1 = 0, count_m2 = 0, count_m3 = 0;
    Vector* ma1 = (Vector*)malloc(sizeof(Vector) * vector_count);
    Vector* ma2 = (Vector*)malloc(sizeof(Vector) * vector_count);
    Vector* ma3 = (Vector*)malloc(sizeof(Vector) * vector_count);

    if (ma1 == NULL || ma2 == NULL || ma3 == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    double** A = create_positive_definite_matrix(n);
    if (A == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    va_start(args, vector_count);
    for (int i = 0; i < vector_count; i++) {
        Vector* v = va_arg(args, Vector*);
        double length1 = inf_fun(v);
        double length2 = p_fun(v, 2);
        double length3 = a_fun(v, A);

        if (length1 > maxx1) {
            maxx1 = length1;
            count_m1 = 0;
            ma1[count_m1++] = *v;
        }
        else if (fabs(maxx1 - length1) < EPSILON) {
            ma1[count_m1++] = *v;
        }

        if (length2 > maxx2) {
            maxx2 = length2;
            count_m2 = 0;
            ma2[count_m2++] = *v;
        }
        else if (fabs(maxx2 - length2) < EPSILON) {
            ma2[count_m2++] = *v;
        }

        if (length3 > maxx3) {
            maxx3 = length3;
            count_m3 = 0;
            ma3[count_m3++] = *v;
        }
        else if (fabs(maxx3 - length3) < EPSILON) {
            ma3[count_m3++] = *v;
        }
    }
    va_end(args);

    print_max_vector(ma1, count_m1, 1);
    print_max_vector(ma2, count_m2, 2);
    print_max_vector(ma3, count_m3, 3);

    for (int i = 0; i < n; i++) free(A[i]);
    free(A);
    free(ma1);
    free(ma2);
    free(ma3);

    return SUCCESS;
}

int main() {
    int n = 3;
    int vector_count = 4;

    Vector vector1 = { n, (double[]) { 1.0, 2.0, 3.0 } };
    Vector vector2 = { n, (double[]) { 3.0, -4.0, 1.0 } };
    Vector vector3 = { n, (double[]) { 0.5, 2.5, -3.0 } };
    Vector vector4 = { n, (double[]) { 3.0, -4.0, 1.0 } };

    ErrorCode result = find_longest_vectors(n, &infinity_norm, &p_norm, &a_norm, vector_count, &vector1, &vector2, &vector3, &vector4);

    if (result != SUCCESS) {
        printf("An error occurred: ");
        switch (result) {
        case NULL_POINTER_ERROR:
            printf("Null pointer error\n");
            break;
        case DIMENSION_ERROR:
            printf("Dimension error\n"); 
            break;
        case MEMORY_ALLOCATION_ERROR: 
            printf("Memory allocation error\n");
            break;
        case MATRIX_ERROR:
            printf("Matrix error\n"); 
            break;
        default:
            printf("Unknown error\n"); 
            break;
        }
    }

    return 0;
}


















