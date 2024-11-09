#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vector {
    int n;
    float *coord;
} Vector;

typedef struct Array {
    int elem_size;
    int elem_count;
    int capacity; //мб комбинировать
    void *inner;
} Array;

typedef int (*norm)(float *res, const Vector *v);

int init(Vector *vec, float *coords) {
    vec->coord = coords;
    vec->n = sizeof(coords) / sizeof(coords[0]);

    return 0;
}

int print_result_vector(const Vector *v) {
    for (int i = 0; i < v->n; i++) {
        printf("%f", v->coord[i]);
        if (i < v->n - 1)
            printf(" ");
    }
    printf("\n");
    return 0;
}

int init_array(Array *a, int capacity, int elem_size) {
    a->elem_count = 0;
    
    a->capacity = capacity;
    a->elem_size = elem_size;
    a->inner = malloc(a->capacity * a->elem_size);
    if (!a->inner) return -1;
    
    return 0;
}

int free_array(Array *a) {
    a->capacity = 0;
    a->elem_count = 0;
    a->elem_size = 0;
    free(a->inner);
    return 0;
}

int add_array(Array *a, const void *val) {
    if (a->elem_count >= a->capacity) {
        int new_capacity = a->capacity * 2;
        void *new = realloc(a->inner, new_capacity * a->elem_size);
        if (!new) {
            return -1;
        }
        a->inner = new;
        a->capacity = new_capacity;
    }
    memcpy(a->inner + a->elem_count * a->elem_size, val, a->elem_size);
    a->elem_count++;
    return 0;
}

int get_array(void *val, Array *a, int i) {
    if (i >= a->elem_count)
        return -1;
    memcpy(val, a->inner + i * a->elem_size, a->elem_size);
    return 0;
}

int func(int n, ...) {
    if (n <= 0) {
        printf("Неверное значение размерности\n");
        return -1;
    }

    Array vecs;
    if (init_array(&vecs, 16, sizeof(Vector))) {
        printf("Невозможно выделить память для векторов\n");
        return -1;
    }
        
    Array norms;
    if (init_array(&norms, 16, sizeof(norm))) {
        free_array(&vecs);
        printf("Невозможно выделить память для норм\n");
        return -1;
    }

    va_list valist;
    va_start(valist, n);

    while (1) {
        Vector *v = va_arg(valist, Vector *);
        if (!v) {
            break;
        }
        if (v->n != n) {
            printf("Неверное значение размерности\n");
            free_array(&vecs);
            va_end(valist);
            return -1;
        }
        if (add_array(&vecs, v)) {
            printf("Невозможно выделить память для векторов\n");
            free_array(&vecs);
            va_end(valist);
            return -1;
        }
    }

    while (1) {
        norm n = va_arg(valist, norm);
        if (!n) {
            break;
        }
            
        if (add_array(&norms, &n)) {
            free_array(&norms);
            free_array(&vecs);
            printf("Невозможно выделить память для норм\n");
            va_end(valist);
            return -1;
        }
    }

    for (int i = 0; i < norms.elem_count; i++) {
        norm f;
        get_array(&f, &norms, i);
        float max = 0;
        float res;
        for (int j = 0; j < vecs.elem_count; j++) {
            Vector v;
            get_array(&v, &vecs, j);
            f(&res, &v);
            if (res > max) {
                max = res;
            }
        }
        printf("%d %f\n", i, max);
        for (int j = 0; j < vecs.elem_count; j++) {
            Vector v;
            get_array(&v, &vecs, j);
            f(&res, &v);
            if (res == max) {
                printf("\t");
                print_result_vector(&v);
            }
        }
    }
    va_end(valist);
    free_array(&vecs);
    free_array(&norms);
    return 0;
}

int calc_norm_max(float *res, const Vector *v) {
    float max = 0;
    for (int i = 0; i < v->n; i++) {
        if (fabsf(v->coord[i]) > max) {
            max = fabsf(v->coord[i]);
        }
    }
    *res = max;
    return 0;
}

int calc_norm_p(float *res, const Vector *v) {
    int p = 2;
    float sum = 0;
    for (int i = 0; i < v->n; i++) {
        float ab = fabsf(v->coord[i]);
        sum += powf(ab, p);
    }
    *res = powf(sum, 1.0 / p);
    return 0;
}

int calc_norm_mat(float *res, const Vector *v) {
    float mat[] = {2, 0, 0, 2};
    float sum = 0;
    for (int i = 0; i < v->n; i++) {
        float sum2 = 0;
        for (int j = 0; j < v->n; j++) {
            int pos = i * v->n + j;
            sum2 += mat[pos] * v->coord[j];
        }
        sum += sum2 * v->coord[i];
    }
    *res = sqrtf(sum);
    return 0;
}


int main(void) {
    Vector a, b, c, d;
    float a_coord[] = {0.5, -5};
    float b_coord[] = {5.0, 0.0};
    float c_coord[] = {4, 4};
    float d_coord[] = {-4, 4};
    
    init(&a, a_coord);
    init(&b, b_coord);
    init(&c, c_coord);
    init(&d, d_coord);


    func(2, &a, &b, &c, &d, NULL, calc_norm_max, calc_norm_p, calc_norm_mat, NULL);
}
