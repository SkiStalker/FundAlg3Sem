#include "vector.h"

typedef double (*NormaCalculator)(const DimensionVector *, const void *);

double MaxNorma(const DimensionVector *v) {
    double max = fabs(v->Coordinates[0]);
    for (int i = 1; i < v->N; i++) {
        if (fabs(v->Coordinates[i]) > max) {
            max = fabs(v->Coordinates[i]);
        }
    }
    return max;
}

double SumNorma(const DimensionVector *v, const void *arg) {
    double p = *(double *) arg;
    double sum = 0.0;
    for (int i = 0; i < v->N; i++) {
        sum += pow(fabs(v->Coordinates[i]), p);
    }
    return pow(sum, 1.0 / p);
}

double MatrixNorma(const DimensionVector *v, const void *arg) {
    double *A = (double*) arg;
    double sum = 0.0;
    for (int i = 0; i < v->N; i++) {
        for (int j = 0; j < v->N; j++) {
            sum += v->Coordinates[i] * A[i * v->N + j] * v->Coordinates[j];
        }
    }
    return sqrt(sum);
}

void FreeVectors(Vector **vectors, int count) {
    for (int i = 0; i < count; i++) {
        if (vectors[i]) {
            DestroyVector(vectors[i]);
        }
    }
    free(vectors);
}

Vector **GetMaxNorma(int *code, int numberOfVectors, int numberOfNormas, ...) {
    va_list args;
    va_start(args, numberOfNormas);

    DimensionVector **vectors = (DimensionVector**)malloc(sizeof(DimensionVector *) * numberOfVectors);
    if (!vectors) {
        va_end(args);
        *code = ERROR_MALLOC;
        return NULL;
    }

    Vector **output = (Vector**)malloc(sizeof(Vector *) * numberOfNormas);
    if (!output) {
        free(vectors);
        va_end(args);
        *code = ERROR_MALLOC;
        return NULL;
    }

    for (int i = 0; i < numberOfVectors; i++) {
        vectors[i] = va_arg(args, DimensionVector*);
    }

    for (int k = 0; k < numberOfNormas; k++) {
        output[k] = CreateVector(1);
        if (!output[k]) {
            FreeVectors(output, k);
            free(vectors);
            va_end(args);
            *code = ERROR_MALLOC;
            return NULL;
        }

        NormaCalculator f = va_arg(args, NormaCalculator);
        void *arg = va_arg(args, void*);
        double longestNorma = -DBL_MAX;

        for (int i = 0; i < numberOfVectors; i++) {
            double norma = f(vectors[i], arg);

            if (longestNorma < norma) {
                longestNorma = norma;
                DestroyVector(output[k]);
                output[k] = CreateVector(1);
                VectorPush(output[k], CopyDimensionVector(vectors[i]));
            } else if (fabs(longestNorma - norma) < EPS) {
                VectorPush(output[k], CopyDimensionVector(vectors[i]));
            }
        }
    }
    free(vectors);
    va_end(args);
    *code = SUCCESS;
    return output;
}