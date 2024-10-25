#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

typedef enum
{
    SUCCESS,
    ERROR_SEED,
    ERROR_INVALID_INPUT,
    ERROR_INVALID_STRING,
    ERROR_INVALID_STRING_LENGTH,
    ERROR_UNKNOWN_FLAG,
    ERROR_MEMORY_ALLOCATION

} status_code;

status_code function_for_l(const char *string, int *result)
{
    if (!string || !result)
    {
        return ERROR_INVALID_STRING;
    }

    int length = 0;

    while (string[length] != '\0')
    {
        length++;
    }

    *result = length;
    return SUCCESS;
}

status_code function_for_r(const char *string, char **result)
{
    if (!string || !result)
    {
        return ERROR_INVALID_STRING;
    }

    int result_count_length_string;
    status_code status = function_for_l(string, &result_count_length_string);

    if (status != SUCCESS)
    {
        return ERROR_INVALID_STRING_LENGTH;
    }

    *result = (char *)malloc(sizeof(char) * (result_count_length_string + 1));

    if (!*result)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < result_count_length_string; i++)
    {
        (*result)[i] = string[result_count_length_string - 1 - i];
    }

    (*result)[result_count_length_string] = '\0';
    return SUCCESS;
}

status_code function_for_u(const char *string, char **result)
{
    if (!string || !result)
    {
        return ERROR_INVALID_STRING;
    }

    int result_count_length_string;
    status_code status = function_for_l(string, &result_count_length_string);

    if (status != SUCCESS)
    {
        return ERROR_INVALID_STRING_LENGTH;
    }

    *result = (char *)malloc(sizeof(char) * (result_count_length_string + 1));

    if (!*result)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < result_count_length_string; i++)
    {
        if (i % 2 != 0)
        {
            (*result)[i] = toupper(string[i]);
        }
        else
        {
            (*result)[i] = string[i];
        }
    }

    (*result)[result_count_length_string] = '\0';
    return 0;
}

status_code function_for_n(const char *string, char **result)
{
    if (!string || !result)
    {
        return ERROR_INVALID_STRING;
    }

    int result_count_length_string;
    status_code status = function_for_l(string, &result_count_length_string);

    if (status != SUCCESS)
    {
        return ERROR_INVALID_STRING_LENGTH;
    }

    *result = (char *)malloc(sizeof(char) * (result_count_length_string + 1));

    if (!*result)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    char *digits = (char *)malloc(sizeof(char) * (result_count_length_string + 1));
    char *letters = (char *)malloc(sizeof(char) * (result_count_length_string + 1));
    char *others = (char *)malloc(sizeof(char) * (result_count_length_string + 1));

    if (!digits || !letters || !others)
    {
        free(digits);
        free(letters);
        free(others);
        free(*result);
        return ERROR_MEMORY_ALLOCATION;
    }

    int digits_index = 0;
    int letters_index = 0;
    int others_index = 0;

    for (int i = 0; i < result_count_length_string; i++)
    {
        if (isdigit(string[i]))
        {
            digits[digits_index++] = string[i];
        }
        else if (isalpha(string[i]))
        {
            letters[letters_index++] = string[i];
        }
        else
        {
            others[others_index++] = string[i];
        }
    }

    digits[digits_index] = '\0';
    letters[letters_index] = '\0';
    others[others_index] = '\0';

    int position = 0;

    for (int i = 0; i < digits_index; i++, position++)
    {
        (*result)[position] = digits[i];
    }

    for (int i = 0; i < letters_index; i++, position++)
    {
        (*result)[position] = letters[i];
    }

    for (int i = 0; i < others_index; i++, position++)
    {
        (*result)[position] = others[i];
    }

    (*result)[position] = '\0';

    free(digits);
    free(letters);
    free(others);

    return SUCCESS;
}

status_code function_for_c(int argc, char *argv[], char **result)
{
    if (argc < 4 || !argv || !result)
    {
        return ERROR_INVALID_INPUT;
    }

    char *ptr_end;
    unsigned long converted_seed = strtoul(argv[2], &ptr_end, 10);

    if (*ptr_end != '\0' || converted_seed > UINT_MAX)
    {
        return ERROR_SEED;
    }

    unsigned int local_seed = (unsigned int)converted_seed;
    srand(local_seed);

    int total_length = 0;

    for (int i = 3; i < argc; i++)
    {
        int result_count_length_string;
        int status = function_for_l(argv[i], &result_count_length_string);

        if (status != SUCCESS)
        {
            return ERROR_INVALID_STRING_LENGTH;
        }

        total_length += result_count_length_string;
    }

    *result = (char *)malloc(sizeof(char) * (total_length + 1));

    if (!*result)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    int current_index = 0;
    int *indices = (int *)malloc(sizeof(int) * (argc - 3));

    if (!indices)
    {
        free(*result);
        return ERROR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < argc - 3; i++)
    {
        indices[i] = i + 3;
    }

    for (int i = argc - 4; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    for (int i = 0; i < argc - 3; i++)
    {
        int str_index = indices[i];
        const char *current_str = argv[str_index];

        int result_count_length_string2;
        status_code status = function_for_l(current_str, &result_count_length_string2);

        if (status != SUCCESS)
        {
            free(indices);
            free(*result);
            return ERROR_INVALID_STRING_LENGTH;
        }

        for (int j = 0; j < result_count_length_string2; j++)
        {
            (*result)[current_index++] = current_str[j];
        }
    }

    (*result)[current_index] = '\0';
    free(indices);
    return SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("ERROR: Invalid input\n");
        return -1;
    }
    status_code status;

    switch (argv[1][1])
    {
    case 'l':
    {
        int result;
        status = function_for_l(argv[2], &result);
        if (status == ERROR_INVALID_STRING)
        {
            printf("ERROR: String length calculation failed\n");
            return -1;
        }
        printf("String length: %d\n", result);
        break;
    }
    case 'r':
    {
        char *result = NULL;
        status = function_for_r(argv[2], &result);
        if (status == ERROR_INVALID_STRING)
        {
            printf("ERROR: Invalid string\n");
            return -1;
        }
        else if (status == ERROR_MEMORY_ALLOCATION)
        {
            printf("ERROR: Memory allocation failed\n");
            return -1;
        }
        else if (status == ERROR_INVALID_STRING_LENGTH)
        {
            printf("ERROR: String length calculation failed\n");
            return -1;
        }
        printf("Reversed string: %s\n", result);
        free(result);
        break;
    }
    case 'u':
    {
        char *result = NULL;
        status = function_for_u(argv[2], &result);
        if (status == ERROR_INVALID_STRING)
        {
            printf("ERROR: Invalid string\n");
            return -1;
        }
        else if (status == ERROR_MEMORY_ALLOCATION)
        {
            printf("ERROR: Memory allocation failed\n");
            return -1;
        }
        else if (status == ERROR_INVALID_STRING_LENGTH)
        {
            printf("ERROR: String length calculation failed\n");
            return -1;
        }
        printf("Converted string: %s\n", result);
        free(result);
        break;
    }
    case 'n':
    {
        char *result = NULL;
        status = function_for_n(argv[2], &result);
        if (status == ERROR_INVALID_STRING)
        {
            printf("ERROR: Invalid string\n");
            return -1;
        }
        else if (status == ERROR_MEMORY_ALLOCATION)
        {
            printf("ERROR: Memory allocation failed\n");
            return -1;
        }
        else if (status == ERROR_INVALID_STRING_LENGTH)
        {
            printf("ERROR: String length calculation failed\n");
            return -1;
        }
        printf("Converted string: %s\n", result);
        free(result);
        break;
    }
    case 'c':
    {
        if (argc < 4)
        {
            printf("ERROR: Invalid input\n");
            return -1;
        }
        char *result = NULL;
        status = function_for_c(argc, argv, &result);
        if (status == ERROR_INVALID_INPUT)
        {
            printf("ERROR: Invalid input\n");
            return -1;
        }
        else if (status == ERROR_INVALID_INPUT)
        {
            printf("ERROR: Invalid input\n");
            return -1;
        }
        else if (status == ERROR_SEED)
        {
            printf("ERROR: error seed\n");
            return -1;
        }
        else if (status == ERROR_MEMORY_ALLOCATION)
        {
            printf("ERROR: Memory allocation failed\n");
            return -1;
        }
        else if (status == ERROR_INVALID_STRING_LENGTH)
        {
            printf("ERROR: String length calculation failed\n");
            return -1;
        }
        printf("Concatenated string: %s\n", result);
        free(result);
        break;
    }
    default:
        printf("ERROR: Invalid flag\n");
        return -1;
    }

    return SUCCESS;
}