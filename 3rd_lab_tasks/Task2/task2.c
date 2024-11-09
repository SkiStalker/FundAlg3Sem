#include "functions.h"

int main()
{
    vector v_1 = {.data = (double[]){-1.0, -100.0, -3.0, 10.0}, .n = 4};
    vector v_2 = {.data = (double[]){0.0, 4.0, 6.0, 100.}, .n = 4};
    size_t count_res = 0;
    vector *res = NULL;
    double matrix_A[] = {120., 21.0, 44.0, 55.5, 13., 12., 3., 4.,
                         20, 9, 5, 3, 19, 25, 4, 25};
    double p = 11.232;

    switch (search_longest_vectors(&res, &count_res, 3, 3, v_1, v_2, v_2, spectr_norm, &matrix_A, inf_norm, NULL, p_norm, &p))
    {
    case MEMORY_ERROR:
        printf("Memory error\n");
        delete_vector_arr(&res, count_res);
        return MEMORY_ERROR;

    case INPUT_ERROR:
        printf("Input error\n");
        delete_vector_arr(&res, count_res);
        return MEMORY_ERROR;

    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        delete_vector_arr(&res, count_res);
        return OVERFLOW_ERROR;
        
    case SUCCESS:
        for (size_t i = 0; i < count_res - 1; ++i)
        {
            if (!res[i].data)
            {
                putchar('\n');
                continue;
            }
            printf("( ");
            for (size_t j = 0; j < res[i].n; ++j)
            {
                printf("%lf ", res[i].data[j]);
            }
            printf(")\n");
        }
    }

    delete_vector_arr(&res, count_res);
    return 0;
}