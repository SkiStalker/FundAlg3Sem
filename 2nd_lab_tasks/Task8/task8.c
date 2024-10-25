#include "functions.h"

int main()
{
    char *res;

    puts("\n\ttest 1\n123 + FFFFFFFFFF + ABCFF; base = 16;\nexpected result: 100000ABE21");
    switch (sum_any_base_all_numbers(&res, 16, 3, "123", "FFFFFFFFFF", "ABCFF"))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case MEMORY_ERROR:
        puts("Memory error\n");
        return MEMORY_ERROR;
        break;
    case SUCCESS:
        printf("\t result: %s\n", res);
        free(res);
        break;
    }

    puts("\n\ttest 2\nAHAHAHAHA + LOloLO + DOTA2; base = 36;\nexpected result: AHB3CRSD0");
    switch (sum_any_base_all_numbers(&res, 36, 3, "AHAHAHAHA", "LOloLO", "DOTA2"))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case MEMORY_ERROR:
        puts("Memory error\n");
        return MEMORY_ERROR;
        break;
    case SUCCESS:
        printf("\t result: %s\n", res);
        free(res);
        break;
    }

    puts("\n\ttest 3\n999999998 + 0 + 0 + 2 + 1; base = 10;\nexpected result: 1000000001");
    switch (sum_any_base_all_numbers(&res, 10, 5, "999999998", "0", "0", "2", "1"))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case MEMORY_ERROR:
        puts("Memory error\n");
        return MEMORY_ERROR;
        break;
    case SUCCESS:
        printf("\t result: %s\n", res);
        free(res);
        break;
    }

    puts("\n\ttest 4\n999999998 + 0 + 0 + 2 + 1; base = 10000;\nexpected result: Input error");
    switch (sum_any_base_all_numbers(&res, 10000, 5, "999999998", "0", "0", "2", "1"))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        // return INPUT_ERROR;
        break;
    case MEMORY_ERROR:
        puts("Memory error\n");
        return MEMORY_ERROR;
        break;
    case SUCCESS:
        printf("\t result: %s\n", res);
        free(res);
        break;
    }

    puts("\n\ttest 5\n5 + abc; base = 10;\nexpected result: Input error");
    switch (sum_any_base_all_numbers(&res, 10, 2, "5", "abc"))
    {
    case INPUT_ERROR:
        puts("Input error\n");
        return INPUT_ERROR;
        break;
    case MEMORY_ERROR:
        puts("Memory error\n");
        return MEMORY_ERROR;
        break;
    case SUCCESS:
        printf("\t result: %s\n", res);
        free(res);
        break;
    }
}
