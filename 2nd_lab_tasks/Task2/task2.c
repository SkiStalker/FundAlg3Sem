#include "functions.h"

int main()
{
    double result;
    switch (geometric_mean(&result, 4, 1.2, 1.3, 1.8, 5.5))
    {
    case INPUT_ERROR:
        printf("Input error\n");
        return INPUT_ERROR;
        break;

    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("Geometric mean: %lf\n", result);
        break;
    }

    switch (fast_double_pow(2.0, 10, &result))
    {

    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("Fast pow: %lf\n", result);
        break;
    }

    return SUCCESS;
}
