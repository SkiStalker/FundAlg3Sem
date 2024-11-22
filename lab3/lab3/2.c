#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct {
    int n;  
    double *x;
} Vector;

double MaxNorm(Vector *v) {
    double maxVal = fabs(v->x[0]);
    for (int i = 1; i < v->n; i++) {
        if (fabs(v->x[i]) > maxVal) {
            maxVal = fabs(v->x[i]);
        }
    }
    return maxVal;
}

double PNorm(Vector *v, double p) {
    double sum = 0.0;
    for (int i = 0; i < v->n; i++) {
        sum += pow(fabs(v->x[i]), p);
    }
    return pow(sum, 1.0 / p);
}

double ANorm(Vector *v, double **A) {
    double result = 0.0;
    double *Ax = malloc(v->n * sizeof(double));

    if (Ax == NULL) {
        printf("malloc error\n");
        return -1.0;
    }

    for (int i = 0; i < v->n; i++) {  // считает Ax
        Ax[i] = 0;
        for (int j = 0; j < v->n; j++) {
            Ax[i] += A[i][j] * v->x[j];
        }
    }

    for (int i = 0; i < v->n; i++) {  // считает (Ax, x)
        result += Ax[i] * v->x[i];
    }

    free(Ax);
    return sqrt(result);
}

void FindMaxNorm(int n, ...) {
    va_list args;
    va_start(args, n);

    Vector *maxVectors[BUFSIZ];
    int maxCount = 0;
    double maxNormVal = -1;

    while (1) {
        Vector *v = va_arg(args, Vector *);
        if (v == NULL) {
            break;
        }

        // берёт переданную норму из аргументов
        double (*normFunc)(Vector *, void *) = va_arg(args, double (*)(Vector *, void *));
        void *param = va_arg(args, void *);
        double normVal = normFunc(v, param);

        if (normVal > maxNormVal) {
            maxNormVal = normVal;
            maxVectors[0] = v;
            maxCount = 1;
        } else if (normVal == maxNormVal) {
            maxVectors[maxCount++] = v;
        }
    }

    va_end(args);

    printf("vector with max norm(%lf):\n", maxNormVal);
    for (int i = 0; i < maxCount; i++) {
        for (int j = 0; j < maxVectors[i]->n; j++) {
            printf("%.2lf ", maxVectors[i]->x[j]);
        }
        printf("\n");
    }
}

double MaxNormWrapper(Vector *v, void *param) {  // парам тут не нужен
    (void)param;
    return MaxNorm(v);
}

double PNormWrapper(Vector *v, void *param) {
    double p = *(double *)param;
    return PNorm(v, p);
}

double ANormWrapper(Vector *v, void *param) {
    return ANorm(v, (double **)param);
}

int main() {
    int n = 3;

    double **A = malloc(n * sizeof(double *));
    if (A == NULL) {
        printf("malloc error\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(double));
        if (A[i] == NULL) {
            printf("malloc error for line %d of matrix\n", i);
            for (int k = 0; k < i; k++) {
                free(A[k]);
            }
            free(A);
            return 1;
        }
        for (int j = 0; j < n; j++) {  // единичная матрица
            A[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    
    Vector v1 = {n, (double[]){1.0, -2.0, 7.0}};
    Vector v2 = {n, (double[]){-4.0, 5.0, -6.0}};
    Vector v3 = {n, (double[]){4.0, -5.0, 6.0}};
    
    printf("max norm:\n");  // = 7.0
    FindMaxNorm(n, &v1, MaxNormWrapper, NULL, 
                    &v2, MaxNormWrapper, NULL, 
                    &v3, MaxNormWrapper, NULL, NULL);  // v1

    double p = 2.0;  // = sqrt(16 + 25 + 36) = sqrt(77) = 8,7749643
    printf("\np norm with p = %.2lf:\n", p);
    FindMaxNorm(n, &v1, PNormWrapper, &p,
                  &v2, PNormWrapper, &p,
                  &v3, PNormWrapper, &p, NULL);  // v2, v3

    printf("\na norm:\n");  // в данном случае ровно такое же решение и ответ, как в pnorm
    FindMaxNorm(n, &v1, ANormWrapper, A,
                  &v2, ANormWrapper, A,
                  &v3, ANormWrapper, A, NULL);  

    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
