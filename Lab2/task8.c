#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    SUCCESS,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_INPUT,
    ERROR_OVERFLOW
} status_code;

int char_to_value(char c)
{
    if (isdigit(c))
        return c - '0';
    if (isalpha(c))
        return toupper(c) - 'A' + 10;
    return -1; // Invalid character
}

char value_to_char(int value)
{
    if (value < 10)
        return '0' + value;
    return 'A' + value - 10;
}

status_code add_columnar(const char *num1, const char *num2, int base, char **result)
{
    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    size_t max_length = len1 > len2 ? len1 : len2;

    *result = calloc(max_length + 2, sizeof(char));
    if (*result == NULL)
        return ERROR_MEMORY_ALLOCATION;

    int carry = 0, sum = 0;
    size_t i;
    for (i = 0; i < max_length || carry; i++)
    {
        int digit1 = i < len1 ? char_to_value(num1[len1 - 1 - i]) : 0;
        int digit2 = i < len2 ? char_to_value(num2[len2 - 1 - i]) : 0;

        if (digit1 >= base || digit2 >= base || digit1 < 0 || digit2 < 0)
        {
            free(*result);
            return ERROR_INVALID_INPUT;
        }

        sum = digit1 + digit2 + carry;
        carry = sum / base;
        sum %= base;

        (*result)[i] = value_to_char(sum);
    }

    if (i >= max_length + 1 && carry)
    {
        free(*result);
        return ERROR_OVERFLOW;
    }

    for (size_t j = 0; j < i / 2; j++)
    {
        char temp = (*result)[j];
        (*result)[j] = (*result)[i - 1 - j];
        (*result)[i - 1 - j] = temp;
    }
    (*result)[i] = '\0';

    return SUCCESS;
}

status_code sum_numbers(int base, int count, const char **numbers, char **result)
{
    char *current_sum = strdup("0");
    if (current_sum == NULL)
        return ERROR_MEMORY_ALLOCATION;

    for (int i = 0; i < count; i++)
    {
        char *new_sum;
        status_code status = add_columnar(current_sum, numbers[i], base, &new_sum);
        free(current_sum);

        if (status != SUCCESS)
        {
            return status;
        }

        current_sum = new_sum;
    }

    char *non_zero = current_sum;
    while (*non_zero == '0' && *(non_zero + 1) != '\0')
        non_zero++;

    *result = strdup(non_zero);
    free(current_sum);

    if (*result == NULL)
        return ERROR_MEMORY_ALLOCATION;

    return SUCCESS;
}

int main()
{
    printf("Let's calculate the sum of the numbers 1A, 1B, 1C in the number system 16\n");
    const char *numbers[] = {"1A", "2B", "3C"};
    int base = 16;
    char *result = NULL;

    status_code status = sum_numbers(base, 3, numbers, &result);
    if (status == SUCCESS)
    {
        printf("Result: %s\n", result);
        free(result);
    }
    else
    {
        switch (status)
        {
        case ERROR_MEMORY_ALLOCATION:
            printf("ERROR: Memory allocation failed\n");
            break;
        case ERROR_INVALID_INPUT:
            printf("ERROR: Invalid input\n");
            break;
        case ERROR_OVERFLOW:
            printf("ERROR: Overflow occurred\n");
            break;
        default:
            printf("ERROR: Unknown error\n");
            break;
        }
    }

    return 0;
}