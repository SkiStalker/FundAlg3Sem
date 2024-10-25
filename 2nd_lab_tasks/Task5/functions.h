#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

typedef enum Status
{
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    FILE_OPEN_ERROR
} status;


char *Ro(int x)
{

    char *str, *for_realloc;
    int capacity = 6, len_str;
    if (!(str = (char *)malloc(capacity * sizeof(char))))
    {
        return NULL;
    }
    if (x == 0)
    {
        strcpy(str, "Nulla");
        return str;
    }
    if (x < 0)
    {
        x = abs(x);
        str[0] = '-';
        str[1] = '\0';
        len_str = 1;
    }
    else
    {
        str[0] = '\0';
        len_str = 0;
    }

    char symbols[13][3] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int value[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    for (int i = 0; i < 13; ++i)
    {
        int len_symbol = strlen(symbols[i]);
        while (x >= value[i])
        {
            if (capacity < len_str + len_symbol + 1)
            {
                capacity += (len_symbol * 4);
                for_realloc = (char *)realloc(str, capacity * sizeof(char));
                if (!for_realloc)
                {
                    free(str);
                    return NULL;
                }
                str = for_realloc;
            }

            strcat(str, symbols[i]);
            x -= value[i];
            len_str += len_symbol;
        }
    }
    return str;
}

char *Zr(unsigned int x)
{
    if (!x)
        return NULL;
    unsigned int *fib = (unsigned int *)malloc(50 * sizeof(unsigned int));
    if (!fib)
        return NULL;
    char *str = (char *)malloc(50 * sizeof(char));
    if (!str)
    {
        free(fib);
        return NULL;
    }

    fib[0] = 0;
    fib[1] = 1;
    int i = 1;
    while (fib[i] < x)
    {
        i++;
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    int start = i;
    while (x)
    {
        for (int j = start; j > 0; --j)
        {
            if (fib[j] <= x && fib[j] != 0)
            {
                x -= fib[j];
                fib[j] = 0;
                start = j - 1;
                break;
            }
        }
    }
    int j;
    for (j = 1; j <= i; j++)
    {
        str[j - 1] = (fib[j] == 0) ? '1' : '0';
    }
    str[j - 1] = '\0';
    char *r_one = strrchr(str, '1');
    *(++r_one) = '1';
    *(++r_one) = '\0';
    free(fib);
    return str;
}

void convert_to_xbase(int num, int base, char *result)
{
    int i, flag_minus = 0;
    char temp;
    int len_result = 0;
    if (num < 0)
    {
        num = llabs(num);
        flag_minus = 1;
    }
    while (num)
    {
        result[len_result++] = ((num % base > 9) ? (num % base - 10 + 'A') : (num % base + '0'));
        num /= base;
    }
    if (flag_minus)
        result[len_result++] = '-';
    for (i = 0; i < len_result / 2; i++)
    {
        temp = result[i];
        result[i] = result[len_result - 1 - i];
        result[len_result - 1 - i] = temp;
    }
    result[len_result] = '\000';
}

char *CV(int x, int base)
{
    if (base < 2 || base > 36)
        base = 10;

    char *str = (char *)malloc(33 * sizeof(char));
    if (!str)
        return NULL;
    convert_to_xbase(x, base, str);
    return str;
}

status string_to_ll(const char *str, long long *result, int base)
{
    char *endptr;
    *result = strtoll(str, &endptr, base);
    if (*result == LLONG_MAX || *result == LLONG_MIN)
        return INPUT_ERROR;
    if (*endptr != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

char *memory_dump(void *any, int size)
{
    char *res = (char *)malloc((9 * size) * sizeof(char));
    if (!res)
        return NULL;
    char *ptr = (char *)any;

    int idx = 0;
    for (int i = 0; i < size; ++i)
    {
        for (int j = 7; j >= 0; --j)
        {
            res[idx++] = (ptr[i] & (1 << j)) ? '1' : '0';
        }
        res[idx++] = ' ';
    }
    res[--idx] = '\0';
    return res;
}

int oversprintf(char *str, const char *format, ...)
{
    if (!(str && format))
        return -1;

    va_list args;
    va_start(args, format);

    int len_format = strlen(format), count = 0;

    for (int i = 0; i < len_format; ++i)
    {
        if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'R' && format[i + 2] == 'o')
        {
            int x = va_arg(args, int);
            char *res;
            res = Ro(x);
            if (res)
            {
                count += sprintf(str + count, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'Z' && format[i + 2] == 'r')
        {
            int x = va_arg(args, int);
            char *res;
            res = Zr(x);
            if (res)
            {
                count += sprintf(str + count, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'C' && format[i + 2] == 'v')
        {
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = CV(x, base);
            int len = strlen(res);
            for (int i = 0; i < len; ++i)
                res[i] = tolower(res[i]);
            count += sprintf(str + count, res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'C' && format[i + 2] == 'V')
        {
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = CV(x, base);
            count += sprintf(str + count, res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 't' && format[i + 2] == 'o')
        {
            char *str_to = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res_to;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (int j = 0; j < strlen(str_to); ++j)
            {
                if (isupper(str_to[j]))
                    base = -1;
            }
            if (string_to_ll(str_to, &res_to, base) != SUCCESS)
                continue;
            count += sprintf(str + count, "%lld", res_to);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'T' && format[i + 2] == 'O')
        {
            char *str_TO = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res_TO;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (int j = 0; j < strlen(str_TO); ++j)
            {
                if (islower(str_TO[j]))
                    base = -1;
            }
            if (string_to_ll(str_TO, &res_TO, base) != SUCCESS)
                continue;
            count += sprintf(str + count, "%lld", res_TO);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'i')
        {
            int ix = va_arg(args, int);
            char *res;
            if (res = memory_dump(&ix, sizeof(int)))
            {
                count += sprintf(str + count, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'u')
        {
            unsigned int ux = va_arg(args, unsigned int);
            char *res;
            if (res = memory_dump(&ux, sizeof(unsigned int)))
            {
                count += sprintf(str + count, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'd')
        {
            double dx = va_arg(args, double);
            char *res;
            if (res = memory_dump(&dx, sizeof(double)))
            {
                count += sprintf(str + count, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'f')
        {
            int fx = va_arg(args, double);
            char *res;
            if (res = memory_dump(&fx, sizeof(float)))
            {
                count += sprintf(str + count, res);
            }
            i += 2;
            free(res);
        }
        else if (format[i] == '%' && i < len_format - 1)
        {
            char cur_format[7];
            cur_format[0] = '%';
            int j;
            for (j = i + 1; j < i + 6 && format[j] != '%' && j < len_format; ++j)
            {
                cur_format[j - i] = format[j];
            }
            cur_format[j - i] = '\0';
            i = j - 1;
            count += vsprintf(str + count, cur_format, args);
        }

        else
        {
            str[count++] = format[i];
        }
    }
    str[count] = '\0';
    va_end(args);
    return count;
}

int overfprintf(FILE *file, const char *format, ...)
{
    if (!(file && format))
        return -1;

    va_list args;
    va_start(args, format);

    int len_format = strlen(format), count = 0;

    for (int i = 0; i < len_format; ++i)
    {
        if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'R' && format[i + 2] == 'o')
        {
            int x = va_arg(args, int);
            const char *res;
            res = Ro(x);
            if (res)
            {
                count += fprintf(file, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'Z' && format[i + 2] == 'r')
        {
            int x = va_arg(args, int);
            char *res;
            res = Zr(x);
            if (res)
            {
                count += fprintf(file, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'C' && format[i + 2] == 'v')
        {
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = CV(x, base);
            int len = strlen(res);
            for (int i = 0; i < len; ++i)
                res[i] = tolower(res[i]);
            count += fprintf(file, res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'C' && format[i + 2] == 'V')
        {
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = CV(x, base);
            count += fprintf(file, res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 't' && format[i + 2] == 'o')
        {
            char *str_to = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res_to;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (int j = 0; j < strlen(str_to); ++j)
            {
                if (isupper(str_to[j]))
                    base = -1;
            }
            if (string_to_ll(str_to, &res_to, base) != SUCCESS)
                continue;
            count += fprintf(file, "%lld", res_to);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'T' && format[i + 2] == 'O')
        {
            char *str_TO = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res_TO;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (int j = 0; j < strlen(str_TO); ++j)
            {
                if (islower(str_TO[j]))
                    base = -1;
            }
            if (string_to_ll(str_TO, &res_TO, base) != SUCCESS)
                continue;
            count += fprintf(file, "%lld", res_TO);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'i')
        {
            int ix = va_arg(args, int);
            char *res;
            if (res = memory_dump(&ix, sizeof(int)))
            {
                count += fprintf(file, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'u')
        {
            unsigned int ux = va_arg(args, unsigned int);
            char *res;
            if (res = memory_dump(&ux, sizeof(unsigned int)))
            {
                count += fprintf(file, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'd')
        {
            double dx = va_arg(args, double);
            char *res;
            if (res = memory_dump(&dx, sizeof(double)))
            {
                count += fprintf(file, res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < len_format - 2 && format[i + 1] == 'm' && format[i + 2] == 'f')
        {
            float fx = (float)va_arg(args, double);
            char *res;
            if (res = memory_dump(&fx, sizeof(float)))
            {
                count += fprintf(file, res);
            }
            i += 2;
            free(res);
        }
        else if (format[i] == '%' && i < len_format - 1)
        {
            char cur_format[7];
            cur_format[0] = '%';
            int j;
            for (j = i + 1; j < i + 6 && format[j] != '%' && j < len_format; ++j)
            {
                cur_format[j - i] = format[j];
            }
            cur_format[j - i] = '\0';
            i = j - 1;
            count += vfprintf(file, cur_format, args);
        }

        else
        {
            fputc(format[i], file);
            count++;
        }
    }
    va_end(args);
    return count;
}
