#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR
} status;


void concatenate(const char *src, char *dest)
{

    while (*dest++)
        ;
    dest--;
    for (int i = 0; src[i] != '\0'; ++i)
    {
        *dest++ = src[i];
    }
    *dest = '\0';
}

status validate_input(int argc, char *argv[])
{
    if (argc < 3 || argv[1][0] != '-' || argv[1][2] != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

int my_strlen(const char *str)
{
    const char *start = str;
    while (*str++)
        ;
    return str - start - 1;
}

status my_strrev(const char *src, char **dest)
{
    int len = my_strlen(src);
    *dest = (char *)malloc((len + 1) * sizeof(char));
    if (!(*dest))
        return MEMORY_ERROR;

    for (int i = 0; i < len; i++)
    {
        (*dest)[i] = src[len - i - 1];
    }
    (*dest)[len] = '\0';
    return SUCCESS;
}

status up_odd_str(const char *src, char **dest)
{
    int len = my_strlen(src) + 1;
    *dest = (char *)malloc(len * sizeof(char));
    if (!(*dest))
        return MEMORY_ERROR;

    for (int i = 0; i < len; i++)
    {
        (*dest)[i] = (i % 2 && isalpha(src[i])) ? toupper(src[i]) : src[i];
    }
    return SUCCESS;
}

status transform_str(const char *src, char **dest)
{
    int len = my_strlen(src), i;
    *dest = (char *)malloc((len + 1) * sizeof(char));
    if (!(*dest))
        return MEMORY_ERROR;
    char *pos_ptr = *dest;

    for (i = 0; i < len; i++)
    {
        if (isdigit(src[i]))
            *pos_ptr++ = src[i];
    }

    for (i = 0; i < len; i++)
    {
        if (isalpha(src[i]))
            *pos_ptr++ = src[i];
    }

    for (i = 0; i < len; i++)
    {
        if (!isalnum(src[i]))
            *pos_ptr++ = src[i];
    }
    *pos_ptr = '\0';

    return SUCCESS;
}

status randomly_concatenate(char **strings, int count_stings, unsigned long seed, char **dest, int first_param)
{
    int i, capasity = 0, rand_n, count_remained = count_stings;
    for (i = first_param; i < count_stings + first_param; i++)
    {
        if (INT_MAX - my_strlen(strings[i]) < capasity)
            return MEMORY_ERROR;
        capasity += my_strlen(strings[i]);
    }

    *dest = (char *)malloc((capasity + 1) * sizeof(char));
    if (!(*dest))
        return MEMORY_ERROR;
    (*dest)[0] = '\0';
    char *used = (char *)malloc(count_stings * sizeof(char));
    if (used == NULL)
    {
        free(*dest);
        return MEMORY_ERROR;
    }
    for (i = 0; i < count_stings; i++)
    {
        used[i] = '\0';
    }

    srand(seed);

    while (count_remained)
    {
        rand_n = rand() % count_stings;
        if (used[rand_n] == '1')
            continue;

        concatenate(strings[rand_n + first_param], *dest);
        used[rand_n] = '1';
        count_remained--;
    }
    free(used);
    return SUCCESS;
}


status string_to_uint(const char *str, unsigned long *result)
{
    if (str[0] == '-')
        return INPUT_ERROR;
    char *endinp;
    *result = strtoul(str, &endinp, 10);
    if (*result == ULONG_MAX)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}
