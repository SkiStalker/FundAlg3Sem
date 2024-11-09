#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>


#define EPSILON 0.000000001

typedef enum {
    SUCCESS,
    MEMORY_ERROR,
} ret_type_t;

typedef struct{
    int N;
    double* coords;
} VVector;

int VVector_delete(VVector* v) {
    if(v == NULL){
        return 0;
    }
    else if(v->coords == NULL){
        free(v);
    }
    free(v->coords);
    free(v);
    return 1;
}

VVector* Vvector_copy(VVector* v) {
    VVector* result = malloc(sizeof(VVector));
    double* coord = malloc(sizeof(double)*  v->N);
    for(int i = 0; i < v->N; i++) {
        coord[i] = v->coords[i];
    }
    result->coords = coord;
    result->N = v->N;
    return result;
}

VVector* Vvector_init(int n, ...) {
    va_list list;
    double* coords = malloc(sizeof(double)*  n);
    VVector* result = malloc(sizeof(VVector));
    if (coords == NULL) {
        return NULL;
    }

    if (!result) {
        free(coords);
        return NULL;
    }

    va_start(list, n);
    for (int i = 0; i < n; i++) {
        coords[i] = va_arg(list, double);
    }
    va_end(list);
    result->coords = coords;
    result->N = n;
    return result;
}

typedef double (*callback)(const VVector* , const void* );

typedef struct{
    VVector** data;
    int len;
    int _size;
} Vector;

Vector* Vector_init(int size) {
    Vector* v = malloc(sizeof(Vector));
    if (v == NULL) {
        return NULL;
    }

    v->data = malloc(sizeof(VVector* )*  size);
    if (v->data == NULL) {
        free(v);
        return NULL;
    }
    v->_size = size;
    v->len = 0;
    return v;
}

int Vector_add(Vector* v, VVector* value) {
    if (v->len + 1 >= v->_size) {
        VVector** newdata = malloc(v->_size*  sizeof(VVector* )*  2);
        for (int i = 0; i < v->len; i++) {
            newdata[i] = v->data[i];
        }
        free(v->data);
        v->data = newdata;
        v->_size *= 2;
    }
    v->data[v->len] = value;
    v->len++;
    return 1;
}

int Vector_pop(Vector* v) {
    if(v == NULL){
        return 0;
    }
    else if(v->len == 0 || v->data == NULL ){
        return 0;
    }
    v->len--;
    free(v->data[v->len]->coords);
    free(v->data[v->len]);
    return 1;
}

int Vector_delete(Vector* v) {
    if(v == NULL){
        return 0;
    }
    else if(v->data == NULL || v->len == 0){
        free(v);
        return 0;
    }
    for (int i = 0; i < v->len; i++) {
        if(v->data[i]->coords != NULL){
            free(v->data[i]->coords);
        }
        if(v->data[i] != NULL){
            free(v->data[i]);
        }
    }

    if(v->data != NULL){
        free(v->data);
    }
    if(v != NULL){
        free(v);
    }
    return 1;
};

double normMax(const VVector* v, const void* arg) {

    double max = fabs(v->coords[0]);

    for (int i = 1; i < v->N; i++) {
        if (fabs(v->coords[i]) > max) 
        {
            max = fabs(v->coords[i]);
        }
    }
    return max;
}

double normSum(const VVector* v, const void* arg) {

    double p =* (double* ) arg;
    double sum = 0.0;

    for (int i = 0; i < v->N; i++) {
        sum += pow(fabs(v->coords[i]), p);
    }
    return pow(sum, 1.0 / p);
}

double normMatrix(const VVector* v, const void* arg) {

    double* matrix = (double*) arg;
    double sum = 0.0;

    for (int i = 0; i < v->N; i++) {
        for (int j = 0; j < v->N; j++) {
            sum += v->coords[i] *  matrix[i * v->N + j] * v->coords[j];
        }
    }
    return sqrt(sum);
}

void Vectors_delete(Vector** vectors, int count) {
    for (int i = 0; i < count; i++) {
        if (vectors[i]) {
            Vector_delete(vectors[i]);
        }
    }
    free(vectors);
}

Vector** TaskFunc(int* ret, int dimension, int vctsAmount, int nmsAmount, ...) {
    
    va_list list;
    va_start(list, nmsAmount);
    VVector** vectors = (VVector**)malloc(sizeof(VVector*)*vctsAmount);
    if (!vectors) {
        va_end(list);
        *ret = MEMORY_ERROR;
        return NULL;
    }

    Vector** ans = (Vector**)malloc(sizeof(Vector*)*nmsAmount);
    if (!ans) {
        free(vectors);
        va_end(list);
        *ret = MEMORY_ERROR;
        return NULL;
    }
    for (int i = 0; i < vctsAmount; i++) {
        vectors[i] = va_arg(list, VVector*);
    }
    for (int k = 0; k < nmsAmount; k++) {
        ans[k] = Vector_init(1);
        if (!ans[k]) {
            Vectors_delete(ans, k);
            free(vectors);
            va_end(list);
            *ret = MEMORY_ERROR;
            return NULL;
        }

        callback f = va_arg(list, callback);
        void* arg = va_arg(list, void*);
        double longestNorm = -__DBL_MAX__;

        for (int i = 0; i < vctsAmount; i++) {
            double norm = f(vectors[i], arg);

            if (longestNorm < norm) {
                longestNorm = norm;
                Vector_delete(ans[k]);
                ans[k] = Vector_init(1);
                Vector_add(ans[k], Vvector_copy(vectors[i]));
            } 
            else if (fabs(longestNorm - norm) < EPSILON) {
                Vector_add(ans[k], Vvector_copy(vectors[i]));
            }
        }
    }
    free(vectors);
    va_end(list);
    *ret = SUCCESS;
    return ans;
}

int main() 
{
    VVector* v1 = Vvector_init(3, 1.0, 2.0, 3.0);
    VVector* v2 = Vvector_init(3, 15.0, 22.0, 33.0);
    VVector* v3 = Vvector_init(3, -1.0, 33.0, 0.0);

    
    int nmsAmount = 3;
    int vctsAmount = 3;
    int ret = SUCCESS;
    int p = 1;
    double matrix[9] = {1, 0, 0,
                   0, 1, 0,
                   0, 0, 1};

    Vector** result = TaskFunc(&ret, 3, vctsAmount, nmsAmount, v1, v2, v3, normMax, NULL, normSum, &p, normMatrix, &matrix);
    
    if (ret != SUCCESS) {
        VVector_delete(v1);
        VVector_delete(v2);
        VVector_delete(v3);
        return ret;
    }

    for (int i = 0; i < nmsAmount; i++) {
        printf("Longest vectors of %d-th norm: \n", i);
        for (int j = 0; j < result[i]->len; j++) {
            VVector* v = result[i]->data[j];
            for (int k = 0; k < v->N; k++) {
                printf("%lf ", v->coords[k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    VVector_delete(v1);
    VVector_delete(v2);
    VVector_delete(v3);
    Vectors_delete(result, nmsAmount);

    return 0;
}
