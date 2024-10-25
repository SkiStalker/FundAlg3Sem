#include "functions.h"

int main()
{
    double res[128];

    int len_ans = 0;
    switch (has_a_finite_representation(36, res, &len_ans, 1, 0.25))
    {
    case BASE_ERROR:
        printf("Base error\n");
        return BASE_ERROR;

    case INPUT_ERROR:
        printf("Some numbers are entered incorrectly\n");

    case SUCCESS:
        if (len_ans)
        {
            printf("Answer: ");
            for (int i = 0; i < len_ans; ++i)
            {
                printf("%lf ", res[i]);
            }
            putchar('\n');
        }
        else
        {
            printf("There are no such numbers\n");
        }
    }
    return 0;
}
