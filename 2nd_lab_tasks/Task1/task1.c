#include "functions.h"

int main(int argc, char *argv[])
{
    if (validate_input(argc, argv) == INPUT_ERROR)
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }
    
    char *new_string = NULL;
    int count_strings = argc - 3;
    unsigned long seed;

    switch (argv[1][1])
    {
    case 'l':
        if (argc != 3)
        {
            printf("Input error\n");
            return INPUT_ERROR;
        }
        printf("String length - %d\n", my_strlen(argv[2]));
        break;

    case 'r':
        if (argc != 3)
        {
            printf("Input error\n");
            return INPUT_ERROR;
        }

        char *reverse_string = NULL;
        if (my_strrev(argv[2], &reverse_string) == MEMORY_ERROR)
        {
            printf("Memory error\n");
            return MEMORY_ERROR;
        }

        printf("Reversed string: \"%s\"\n", reverse_string);
        free(reverse_string);
        break;

    case 'u':
        // char *new_string = NULL;
        if (up_odd_str(argv[2], &new_string) == MEMORY_ERROR)
        {
            printf("Memory error\n");
            return MEMORY_ERROR;
        }

        printf("New string: \"%s\"\n", new_string);
        free(new_string);
        break;
    case 'n':
        if (argc != 3)
        {
            printf("Input error\n");
            return INPUT_ERROR;
        }
        char *transformed_string = NULL;
        if (transform_str(argv[2], &transformed_string) == MEMORY_ERROR)
        {
            printf("Memory error\n");
            return MEMORY_ERROR;
        }
        printf("New string: \"%s\"\n", transformed_string);
        free(transformed_string);
        break;

    case 'c':
        // int count_strings = argc - 3;
        // unsigned long seed;
        if (string_to_uint(argv[2], &seed) != SUCCESS || seed == 0)
        {
            printf("Input error\n");
            return INPUT_ERROR;
        }

        char *conccatenated_str = NULL;
        if (randomly_concatenate(argv, count_strings, seed, &conccatenated_str, 3) != SUCCESS)
        {
            printf("Memory error\n");
            return MEMORY_ERROR;
        }
        printf("Concatenated string: \"%s\"\n", conccatenated_str);
        free(conccatenated_str);
        break;

    default:
        printf("Input error: abnormal key\n");
        return INPUT_ERROR;
        break;
    }

    return SUCCESS;
}
