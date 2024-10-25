#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

status validate_base(char *number, int base, int len)
{
    if (base < 10)
    {
        for (int i = 0; i < len; ++i)
        {
            if (number[i] < '0' || number[i] > base - '0' - 1)
                return INPUT_ERROR;
        }
    }
    else
    {
        for (int i = 0; i < len; ++i)
        {
            number[i] = (char)toupper(number[i]);
            if (!isdigit(number[i]) && (number[i] < 'A' || number[i] > base - 11 + 'A'))
                return INPUT_ERROR;
        }
    }
    return SUCCESS;
}

void str_reverse(char *str, int len)
{
    char temp;
    for (int i = 0; i < len / 2; i++)
    {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void sum_any_base_two_numbers(char *number1_res, char *number2, int base, int *len_res, int len_number)
{
    int digit_shift = 0, i, digit_1, digit_2, sum;
    for (i = 0; i < *len_res && i < len_number; ++i)
    {
        digit_1 = isdigit(number1_res[i]) ? number1_res[i] - '0' : number1_res[i] - 'A' + 10;
        digit_2 = isdigit(number2[i]) ? number2[i] - '0' : number2[i] - 'A' + 10;
        sum = digit_1 + digit_2 + digit_shift;
        number1_res[i] = (sum % base) < 10 ? (sum % base) + '0' : (sum % base) + 'A' - 10;
        digit_shift = sum / base;
    }

    if (len_number > *len_res)
    {
        for (; i < len_number; ++i)
        {
            digit_2 = isdigit(number2[i]) ? number2[i] - '0' : number2[i] - 'A' + 10;
            sum = digit_2 + digit_shift;
            number1_res[i] = (sum % base) < 10 ? (sum % base) + '0' : (sum % base) + 'A' - 10;
            digit_shift = sum / base;
        }
    }
    else
    {
        for (; i < *len_res; ++i)
        {
            digit_1 = isdigit(number1_res[i]) ? number1_res[i] - '0' : number1_res[i] - 'A' + 10;
            sum = digit_1 + digit_shift;
            number1_res[i] = (sum % base) < 10 ? (sum % base) + '0' : (sum % base) + 'A' - 10;
            digit_shift = sum / base;
        }
    }
    *len_res = (len_number > *len_res) ? len_number : *len_res;
    number1_res[*len_res] = '\0';
    if (digit_shift)
    {
        number1_res[i++] = digit_shift < 10 ? digit_shift + '0' : digit_shift + 'A' - 10;
        number1_res[i] = '\0';
        *len_res = i;
    }
}

status sum_any_base_all_numbers(char **result, int base, int count_numbers, ...)
{
    *result = NULL;
    if (base < 2 || base > 36)
        return INPUT_ERROR;

    char *ptr_number, *number;
    int capacity, len_num, len_res;

    va_list args;
    va_start(args, count_numbers);

    ptr_number = va_arg(args, char *);
    len_num = strlen(ptr_number);
    number = (char *)malloc((len_num + 1) * sizeof(char));
    if (!number)
    {
        va_end(args);
        return MEMORY_ERROR;
    }
    strcpy(number, ptr_number);
    capacity = len_num * 2 + 1;
    len_res = len_num;
    char *res = (char *)malloc(capacity * sizeof(char));
    if (!res)
    {
        va_end(args);
        free(number);
        return MEMORY_ERROR;
    }

    if (validate_base(number, base, len_num) != SUCCESS)
    {
        va_end(args);
        free(number);
        free(res);
        return MEMORY_ERROR;
    }
    str_reverse(number, len_num);
    strcpy(res, number);
    free(number);
    for (int i = 0; i < count_numbers - 1; ++i)
    {
        ptr_number = va_arg(args, char *);
        len_num = strlen(ptr_number);
        number = (char *)malloc((len_num + 1) * sizeof(char));
        if (!number)
        {
            va_end(args);
            free(res);
            return MEMORY_ERROR;
        }
        strcpy(number, ptr_number);

        if (validate_base(number, base, len_num) != SUCCESS)
        {
            va_end(args);
            free(number);
            free(res);
            return INPUT_ERROR;
        }

        if (len_num + 2 > capacity)
        {
            char *temp_realloc;
            capacity = len_num * 2 + 1;
            temp_realloc = (char *)realloc(res, capacity * sizeof(char));
            if (!res)
            {
                va_end(args);
                free(number);
                free(res);
                return MEMORY_ERROR;
            }
            res = temp_realloc;
        }

        str_reverse(number, len_num);
        sum_any_base_two_numbers(res, number, base, &len_res, len_num);
        free(number);
    }
    str_reverse(res, len_res);
    *result = res;
    va_end(args);
    return SUCCESS;
}


