#include "dimensionVector.h"

typedef struct{
    int len;
    int capacity;
    DimensionVector **buffer;
} Vector;

Vector *CreateVector(int initialCapacity) {
    Vector *v = malloc(sizeof(Vector));
    if (!v) {
        return NULL;
    }
    v->buffer = malloc(sizeof(DimensionVector *) * initialCapacity);
    if (!v->buffer) {
        free(v);
        return NULL;
    }
    v->capacity = initialCapacity;
    v->len = 0;
    return v;
}

void DestroyVector(Vector *v) {
    for (int i = 0; i < v->len; i++) {
        free(v->buffer[i]->Coordinates);
        free(v->buffer[i]);
    }
    free(v->buffer);
    free(v);
};

void VectorPush(Vector *v, DimensionVector *value) {
    if (v->len + 1 >= v->capacity) {
        DimensionVector **newBuffer = malloc(v->capacity * sizeof(DimensionVector *) * 2);
        for (int i = 0; i < v->len; i++) {
            newBuffer[i] = v->buffer[i];
        }

        free(v->buffer);
        v->buffer = newBuffer;
        v->capacity *= 2;
    }
    v->buffer[v->len] = value;
    v->len++;
}

void VectorPop(Vector *v) {
    v->len--;
    free(v->buffer[v->len]->Coordinates);
    free(v->buffer[v->len]);
}