#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>

typedef enum Status
{
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    FILE_OPEN_ERROR
} status;


int Ro(const char *str, int *shift)
{

    char symbols[13][3] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int value[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    int ind = 0, x = 0, lim;
    for (int i = 0; i < 13; ++i)
    {
        lim = 0;
        if (strlen(symbols[i]) == 1)
        {
            while (lim < 3 && str[ind] == symbols[i][0])
            {
                x += value[i];
                ind++;
                lim++;
            }
        }
        if (strlen(symbols[i]) == 2)
        {
            while (lim < 3 && str[ind] == symbols[i][0] && str[ind + 1] == symbols[i][1])
            {
                x += value[i];
                ind += 2;
                lim++;
            }
        }
        *shift = ind;
    }
    return x;
}

unsigned int Zr(const char *str, int *shift)
{
    int fib_prev = 0, fib_cur = 1;
    int idx = 0;
    unsigned int x = 0, temp;
    while (str[idx] == '0' || str[idx] == '1')
    {
        if (str[idx] == '1')
        {
            x += fib_cur;
            if (str[idx + 1] == '1')
            {
                idx += 2;
                break;
            }
        }
        if (str[idx + 1] != '0' && str[idx + 1] != '1')
        {
            x = 0;
            idx++;
            break;
        }

        temp = fib_cur;
        fib_cur += fib_prev;
        fib_prev = temp;
        idx++;
    }
    *shift = idx;
    return x;
}

int Cv(const char *str, int base, int *shift, int *flag, char mod)
{
    *flag = 1;
    if (base < 2 || base > 36)
        base = 10;
    long x;
    *shift = 0;
    int len_str = strlen(str), i;
    char *temp_str = (char *)malloc((len_str + 1) * sizeof(char));
    if (!temp_str)
    {
        *flag = 0;
        *shift = 0;
        return 0;
    }
    for (i = 0; i < len_str; ++i)
    {
        if (!isalnum(str[i]) || mod == 'v' && isupper(str[i]) || mod == 'V' && islower(str[i]))
            break;
        temp_str[i] = str[i];
    }
    temp_str[i] = '\0';

    char *end_ptr;
    x = strtol(temp_str, &end_ptr, base);
    str = temp_str;
    while (str != end_ptr)
    {
        (*shift)++;
        str++;
    }
    free(temp_str);
    return (int)x;
}

void del_space(const char *sourse, char *dest, int len)
{
    int idx = 0;
    for (int i = 0; i < len; ++i)
    {
        if (!isspace(sourse[i]))
            dest[idx++] = sourse[i];
    }
    dest[idx] = '\0';
}

int overfscanf(FILE *stream, const char *format, ...)
{
    if (!(stream && format))
        return -1;

    va_list args;
    va_start(args, format);

    int len_format = strlen(format), count = 0;

    char *new_format = (char *)malloc((len_format + 1) * sizeof(char));
    if (!new_format)
        return -1;

    del_space(format, new_format, len_format);
    len_format = strlen(new_format);
    char c = getc(stream);
    int idx_buff = 0;
    for (int i = 0; i < len_format; ++i)
    {
        if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'R' && new_format[i + 2] == 'o')
        {
            while (isspace(c))
            {
                c = getc(stream);
            }
            int *x = va_arg(args, int *);
            int shift;
            char *buffer = (char *)malloc(16 * sizeof(char));
            if (!buffer)
            {
                va_end(args);
                break;
            }
            idx_buff = 0;
            while (c == 'M' || c == 'C' || c == 'D' || c == 'X' || c == 'L' || c == 'I' || c == 'V')
            {
                buffer[idx_buff++] = c;
                c = getc(stream);
            }
            buffer[idx_buff] = '\0';
            ungetc(c, stream);
            *x = Ro(buffer, &shift);
            if (*x)
                count++;
            i += 2;
            free(buffer);
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'Z' && new_format[i + 2] == 'r')
        {
            while (isspace(c))
            {
                c = getc(stream);
            }
            unsigned int *x = va_arg(args, unsigned int *);
            int shift;
            char *buffer = (char *)malloc(50 * sizeof(char));
            if (!buffer)
            {
                va_end(args);
                break;
            }
            idx_buff = 0;
            while (c == '0' || c == '1')
            {
                buffer[idx_buff++] = c;
                c = getc(stream);
            }
            buffer[idx_buff] = '\0';
            ungetc(c, stream);
            *x = Zr(buffer, &shift);
            if (*x)
                count++;
            i += 2;
            free(buffer);
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'C' && (new_format[i + 2] == 'v' || new_format[i + 2] == 'V'))
        {
            while (isspace(c))
            {
                c = getc(stream);
            }
            int *x = va_arg(args, int *);
            int base = va_arg(args, int);
            base = (base < 2 || base > 36) ? 10 : base;
            int shift, flag;

            char *buffer = (char *)malloc(50 * sizeof(char));
            if (!buffer)
            {
                va_end(args);
                break;
            }
            idx_buff = 0;
            while (isdigit(c) || (islower(c) && new_format[i + 2] == 'v' || isupper(c) && new_format[i + 2] == 'V'))
            {
                buffer[idx_buff++] = c;
                c = getc(stream);
            }
            buffer[idx_buff] = '\0';
            ungetc(c, stream);
            *x = Cv(buffer, base, &shift, &flag, new_format[i + 2]);
            if (flag)
                count++;
            i += 2;
            free(buffer);
        }
        else if (new_format[i] == '%')
        {
            void *x = va_arg(args, void *);
            char *subformat = (char *)malloc((len_format - i + 4) * sizeof(char));
            if (!subformat)
            {
                va_end(args);
                free(new_format);
                return count;
            }
            int ind_format = 1;
            subformat[0] = '%';
            i++;
            while (new_format[i] != '%' && new_format[i] != '\0' && new_format[i - 1] != 's')
            {
                subformat[ind_format++] = new_format[i];
                i++;
            }
            i--;
            subformat[ind_format++] = '%';
            subformat[ind_format++] = 'n';
            subformat[ind_format++] = '\0';
            int n;
            count += fscanf(stream, subformat, x, &n);
            free(subformat);
        }
        else
        {
            while (isspace(c))
            {
                c = getc(stream);
            }
            if (c == new_format[i])
                c = getc(stream);
        }
    }
    free(new_format);
    va_end(args);
    return count;
}

int oversscanf(const char *buffer, const char *format, ...)
{
    if (!(buffer && format))
        return -1;

    va_list args;
    va_start(args, format);

    int len_format = strlen(format), count = 0;

    char *new_format = (char *)malloc((len_format + 1) * sizeof(char));
    if (!new_format)
        return -1;

    del_space(format, new_format, len_format);
    len_format = strlen(new_format);

    int idx_buff = 0;
    for (int i = 0; i < len_format; ++i)
    {
        if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'R' && new_format[i + 2] == 'o')
        {
            while (isspace(buffer[idx_buff]))
                idx_buff++;
            int *x = va_arg(args, int *);
            int shift;
            *x = Ro(buffer + idx_buff, &shift);
            if (*x)
                count++;
            i += 2;
            idx_buff += shift;
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'Z' && new_format[i + 2] == 'r')
        {
            while (isspace(buffer[idx_buff]))
                idx_buff++;
            unsigned int *x = va_arg(args, unsigned int *);
            int shift;
            *x = Zr(buffer + idx_buff, &shift);
            if (*x)
                count++;
            i += 2;
            idx_buff += shift;
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'C' && (new_format[i + 2] == 'v' || new_format[i + 2] == 'V'))
        {
            while (isspace(buffer[idx_buff]))
                idx_buff++;
            int *x = va_arg(args, int *);
            int base = va_arg(args, int);
            int shift, flag;
            *x = Cv(buffer + idx_buff, base, &shift, &flag, new_format[i + 2]);
            if (flag)
                count++;
            i += 2;
            idx_buff += shift;
        }
        else if (new_format[i] == '%')
        {
            void *x = va_arg(args, void *);
            char *subformat = (char *)malloc((len_format - i + 4) * sizeof(char));
            if (!subformat)
            {
                va_end(args);
                free(new_format);
                return count;
            }
            int ind_format = 1;
            subformat[0] = '%';
            i++;
            while (new_format[i] != '%' && new_format[i] != '\0' && new_format[i - 1] != 's')
            {
                subformat[ind_format++] = new_format[i];
                i++;
            }
            i--;
            subformat[ind_format++] = '%';
            subformat[ind_format++] = 'n';
            subformat[ind_format++] = '\0';
            int n;
            count += sscanf(buffer + idx_buff, subformat, x, &n);
            idx_buff += n;
            free(subformat);
        }
        else
        {
            while (isspace(buffer[idx_buff]))
                idx_buff++;
            if (buffer[idx_buff] == new_format[i])
                idx_buff++;
        }
    }
    free(new_format);
    va_end(args);
    return count;
}