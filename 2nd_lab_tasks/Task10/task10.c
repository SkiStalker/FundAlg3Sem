#include "functions.h"

int main()
{
    double *coefs = NULL;
    int i, degree, a;

    degree = 2;
    a = 2;

    switch (calculate_coefs_polinom_with_shift(a, &coefs, degree, 1., 2., 3.))
    {
    case MEMORY_ERROR:
        printf("Memory error\n");
        return MEMORY_ERROR;

    case INPUT_ERROR:
        printf("Error degree < 0\n");
        return INPUT_ERROR;

    case SUCCESS:
        puts("Coefs of g(x-a):");
        for (i = 0; i <= degree; i++)
        {
            printf("g_%d = %lf\n", i, coefs[i]);
        }
        free(coefs);
    }
}
