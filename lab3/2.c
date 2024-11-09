#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct {
    int n;
    double *coordinates;
} Vector;

double normInfinity(const Vector *v) {
    double max = fabs(v->coordinates[0]);
    for (int i = 1; i < v->n; i++) {
        if (fabs(v->coordinates[i]) > max) {
            max = fabs(v->coordinates[i]);
        }
    }
    return max;
}

double normP(const Vector *v, double p) {
    double sum = 0.0;
    for (int i = 0; i < v->n; i++) {
        sum += pow(fabs(v->coordinates[i]), p);
    }
    return pow(sum, 1.0 / p);
}

double normA(const Vector *v, const double *const *A) {
    double temp[v->n];
    double result = 0.0;

    for (int i = 0; i < v->n; i++) {
        temp[i] = 0.0;
        for (int j = 0; j < v->n; j++) {
            temp[i] += A[i][j] * v->coordinates[j];
        }
    }

    for (int i = 0; i < v->n; i++) {
        result += temp[i] * v->coordinates[i];
    }

    return sqrt(result);
}

void findMaxNorm(int n, int numVectors, const Vector *vectors, int numNorms, ...) {
    va_list norms;
    va_start(norms, numNorms);

    for (int k = 0; k < numNorms; k++) {
        double (*normFunc)(const Vector *) = va_arg(norms, double (*)(const Vector *));
        double maxNorm = -1.0;
        Vector *maxVectors = (Vector *)malloc(numVectors * sizeof(Vector));
        int maxCount = 0;

        for (int i = 0; i < numVectors; i++) {
            double currentNorm = normFunc(&vectors[i]);
            if (currentNorm > maxNorm) {
                maxNorm = currentNorm;
                maxVectors[0] = vectors[i];
                maxCount = 1;
            } else if (currentNorm == maxNorm) {
                maxVectors[maxCount++] = vectors[i];
            }
        }

        printf("Норма %d: максимальная норма = %lf\n", k + 1, maxNorm);
        for (int i = 0; i < maxCount; i++) {
            printf("Вектор: ");
            for (int j = 0; j < n; j++) {
                printf("%lf ", maxVectors[i].coordinates[j]);
            }
            printf("\n");
        }

        free(maxVectors);
    }

    va_end(norms);
}

double normAExample(const Vector *v) {
    static const double row0[] = {2, 0};
    static const double row1[] = {0, 2};
    static const double *A[] = {row0, row1};

    return normA(v, A);
}

double normPExample(const Vector *v) {
    double p = 2.0;
    return normP(v, p);
}

int main() {
    int n = 2;
    Vector a = {n, (double[]){0.5, -5}};
    Vector b = {n, (double[]){5.0, 0.0}};
    Vector c = {n, (double[]){4, 4}};
    Vector d = {n, (double[]){-4, 4}};
    Vector vectors[] = {a, b, c, d};

    findMaxNorm(n, 4, vectors, 3, normInfinity, normPExample, normAExample);

    int n1 = 3;
    printf("\nПримеры трёхмерного пространства:\n");
    Vector vectors3d[2] = {
        {n1, (double[]){1.0, -2.0, 3.0}},
        {n1, (double[]){-4.0, 5.0, -6.0}}
    };

    findMaxNorm(n1, 2, vectors3d, 2, normInfinity, normPExample);

    return 0;
}
