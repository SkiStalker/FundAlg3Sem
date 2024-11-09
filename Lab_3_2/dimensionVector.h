#include "states.h"

typedef struct{
    int N;
    double *Coordinates;
} DimensionVector;

void DestroyDimensionVector(DimensionVector *v) {
    free(v->Coordinates);
    free(v);
}

DimensionVector *CopyDimensionVector(DimensionVector *v) {
    DimensionVector *result = malloc(sizeof(DimensionVector));
    double *coord = malloc(sizeof(double) * v->N);
    for(int i = 0; i < v->N; i++) {
        coord[i] = v->Coordinates[i];
    }
    result->Coordinates = coord;
    result->N = v->N;
    return result;
}

DimensionVector *NewDimensionVector(int n, ...) {
    va_list args;
    double *coords = malloc(sizeof(double) * n);
    DimensionVector *result = malloc(sizeof(DimensionVector));
    if (!coords) {
        return NULL;
    }

    if (!result) {
        free(coords);
        return NULL;
    }

    va_start(args, n);
    for (int i = 0; i < n; i++) {
        coords[i] = va_arg(args, double);
    }
    va_end(args);
    result->Coordinates = coords;
    result->N = n;
    return result;
}