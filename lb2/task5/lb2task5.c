#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

typedef enum ErrorCode{
    OK,
    MEM_ERR,
    INPUT_ERR,
    FILE_ERR
} ErrorCode;


char* roman_numeral(int num) {
    char* result, * temp;
    int capacity = 6, length;
    if (!(result = (char*)malloc(capacity * sizeof(char)))) {
        return NULL;
    }
    if (num == 0) {
        strcpy(result, "Nulla");
        return result;
    }
    if (num < 0) {
        num = abs(num);
        result[0] = '-';
        result[1] = '\0';
        length = 1;
    }
    else {
        result[0] = '\0';
        length = 0;
    }

    char symbols[13][3] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
    int values[] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };

    for (int i = 0; i < 13; ++i) {
        int symbol_len = strlen(symbols[i]);
        while (num >= values[i]) {
            if (capacity < length + symbol_len + 1) {
                capacity += (symbol_len * 4);
                temp = (char*)realloc(result, capacity * sizeof(char));
                if (!temp) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }

            strcat(result, symbols[i]);
            num -= values[i];
            length += symbol_len;
        }
    }
    return result;
}

char* fibonacci_encoding(unsigned int num) {
    if (!num)
        return NULL;
    unsigned int* fib_seq = (unsigned int*)malloc(50 * sizeof(unsigned int));
    if (!fib_seq)
        return NULL;
    char* result = (char*)malloc(50 * sizeof(char));
    if (!result) {
        free(fib_seq);
        return NULL;
    }

    fib_seq[0] = 0;
    fib_seq[1] = 1;
    int i = 1;
    while (fib_seq[i] < num) {
        i++;
        fib_seq[i] = fib_seq[i - 1] + fib_seq[i - 2];
    }
    int start = i;
    while (num) {
        for (int j = start; j > 0; --j) {
            if (fib_seq[j] <= num && fib_seq[j] != 0) {
                num -= fib_seq[j];
                fib_seq[j] = 0;
                start = j - 1;
                break;
            }
        }
    }
    int j;
    for (j = 1; j <= i; j++) {
        result[j - 1] = (fib_seq[j] == 0) ? '1' : '0';
    }
    result[j - 1] = '\0';
    char* last_one = strrchr(result, '1');
    *(++last_one) = '1';
    *(++last_one) = '\0';
    free(fib_seq);
    return result;
}

void convert_to_base(int num, int base, char* result) {
    int i, is_negative = 0;
    char temp;
    int result_len = 0;
    if (num < 0) {
        num = llabs(num);
        is_negative = 1;
    }
    while (num) {
        result[result_len++] = ((num % base > 9) ? (num % base - 10 + 'A') : (num % base + '0'));
        num /= base;
    }
    if (is_negative)
        result[result_len++] = '-';
    for (i = 0; i < result_len / 2; i++) {
        temp = result[i];
        result[i] = result[result_len - 1 - i];
        result[result_len - 1 - i] = temp;
    }
    result[result_len] = '\000';
}

char* custom_base_conversion(int num, int base) {
    if (base < 2 || base > 36)
        base = 10;

    char* result = (char*)malloc(33 * sizeof(char));
    if (!result)
        return NULL;
    convert_to_base(num, base, result);
    return result;
}

ErrorCode string_to_long_long(const char* str, long long* result, int base) {
    char* endptr;
    *result = strtoll(str, &endptr, base);
    if (*result == LLONG_MAX || *result == LLONG_MIN)
        return INPUT_ERR;
    if (*endptr != '\0')
        return INPUT_ERR;
    return OK;
}

char* memory_dump(void* data, int size) {
    char* result = (char*)malloc((9 * size) * sizeof(char));
    if (!result)
        return NULL;
    char* ptr = (char*)data;

    int idx = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 7; j >= 0; --j) {
            result[idx++] = (ptr[i] & (1 << j)) ? '1' : '0';
        }
        result[idx++] = ' ';
    }
    result[--idx] = '\0';
    return result;
}

int custom_sprintf(char* str, const char* format, ...) {
    if (!(str && format))
        return -1;

    va_list args;
    va_start(args, format);

    int format_len = strlen(format), count = 0;

    for (int i = 0; i < format_len; ++i) {
        if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'R' && format[i + 2] == 'o') {
            int num = va_arg(args, int);
            char* res;
            res = roman_numeral(num);
            if (res) {
                strcat(str + count, res);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'Z' && format[i + 2] == 'r') {
            int num = va_arg(args, int);
            char* res;
            res = fibonacci_encoding(num);
            if (res) {
                strcat(str + count, res);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'C' && format[i + 2] == 'v') {
            int num = va_arg(args, int);
            int base = va_arg(args, int);
            char* res;
            res = custom_base_conversion(num, base);
            int len = strlen(res);
            for (int i = 0; i < len; ++i)
                res[i] = tolower(res[i]);
            strcat(str + count, res);
            count += strlen(res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'C' && format[i + 2] == 'V') {
            int num = va_arg(args, int);
            int base = va_arg(args, int);
            char* res;
            res = custom_base_conversion(num, base);
            strcat(str + count, res);
            count += strlen(res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 't' && format[i + 2] == 'o') {
            char* str_to = va_arg(args, char*);
            int base = va_arg(args, int);
            long long res_to;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (size_t j = 0; j < strlen(str_to); ++j) {
                if (isupper(str_to[j]))
                    base = -1;
            }
            if (string_to_long_long(str_to, &res_to, base) != OK)
                continue;
            count += sprintf(str + count, "%lld", res_to);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'T' && format[i + 2] == 'O') {
            char* str_TO = va_arg(args, char*);
            int base = va_arg(args, int);
            long long res_TO;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (size_t j = 0; j < strlen(str_TO); ++j) {
                if (islower(str_TO[j]))
                    base = -1;
            }
            if (string_to_long_long(str_TO, &res_TO, base) != OK)
                continue;
            count += sprintf(str + count, "%lld", res_TO);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'i') {
            int num = va_arg(args, int);
            char* res;
            if ((res = memory_dump(&num, sizeof(int)))) {
                strcat(str + count, res);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'u') {
            unsigned int num = va_arg(args, unsigned int);
            char* res;
            if ((res = memory_dump(&num, sizeof(unsigned int)))) {
                strcat(str + count, res);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'd') {
            double num = va_arg(args, double);
            char* res;
            if ((res = memory_dump(&num, sizeof(double)))) {
                strcat(str + count, res);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'f') {
            float num = (float)va_arg(args, double);
            char* res;
            if ((res = memory_dump(&num, sizeof(float)))) {
                strcat(str + count, res);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }
        else if (format[i] == '%' && i < format_len - 1) {
            char cur_format[7];
            cur_format[0] = '%';
            int j;
            for (j = i + 1; j < i + 6 && format[j] != '%' && j < format_len; ++j) {
                cur_format[j - i] = format[j];
            }
            cur_format[j - i] = '\0';
            i = j - 1;
            count += vsprintf(str + count, cur_format, args);
        }

        else {
            str[count++] = format[i];
        }
    }
    str[count] = '\0';
    va_end(args);
    return count;
}

int custom_fprintf(FILE* file, const char* format, ...) {
    if (!(file && format))
        return -1;

    va_list args;
    va_start(args, format);

    int format_len = strlen(format), count = 0;

    for (int i = 0; i < format_len; ++i) {
        if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'R' && format[i + 2] == 'o') {
            int num = va_arg(args, int);
            char* res;
            res = roman_numeral(num);
            if (res) {
                fputs(res, file);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'Z' && format[i + 2] == 'r') {
            int num = va_arg(args, int);
            char* res;
            res = fibonacci_encoding(num);
            if (res) {
                fputs(res, file);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'C' && format[i + 2] == 'v') {
            int num = va_arg(args, int);
            int base = va_arg(args, int);
            char* res;
            res = custom_base_conversion(num, base);
            int len = strlen(res);
            for (int i = 0; i < len; ++i)
                res[i] = tolower(res[i]);
            fputs(res, file);
            count += strlen(res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'C' && format[i + 2] == 'V') {
            int num = va_arg(args, int);
            int base = va_arg(args, int);
            char* res;
            res = custom_base_conversion(num, base);
            fputs(res, file);
            count += strlen(res);
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 't' && format[i + 2] == 'o') {
            char* str_to = va_arg(args, char*);
            int base = va_arg(args, int);
            long long res_to;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (size_t j = 0; j < strlen(str_to); ++j) {
                if (isupper(str_to[j]))
                    base = -1;
            }
            if (string_to_long_long(str_to, &res_to, base) != OK)
                continue;
            count += fprintf(file, "%lld", res_to);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'T' && format[i + 2] == 'O') {
            char* str_TO = va_arg(args, char*);
            int base = va_arg(args, int);
            long long res_TO;
            i += 2;
            if (base < 2 || base > 36)
                base = 10;
            for (size_t j = 0; j < strlen(str_TO); ++j) {
                if (islower(str_TO[j]))
                    base = -1;
            }
            if (string_to_long_long(str_TO, &res_TO, base) != OK)
                continue;
            count += fprintf(file, "%lld", res_TO);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'i') {
            int num = va_arg(args, int);
            char* res;
            if ((res = memory_dump(&num, sizeof(int)))) {
                fputs(res, file);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'u') {
            unsigned int num = va_arg(args, unsigned int);
            char* res;
            if ((res = memory_dump(&num, sizeof(unsigned int)))) {
                fputs(res, file);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'd') {
            double num = va_arg(args, double);
            char* res;
            if ((res = memory_dump(&num, sizeof(double)))) {
                fputs(res, file);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i < format_len - 2 && format[i + 1] == 'm' && format[i + 2] == 'f') {
            float num = (float)va_arg(args, double);
            char* res;
            if ((res = memory_dump(&num, sizeof(float)))) {
                fputs(res, file);
                count += strlen(res);
            }
            i += 2;
            free(res);
        }
        else if (format[i] == '%' && i < format_len - 1) {
            char cur_format[7];
            cur_format[0] = '%';
            int j;
            for (j = i + 1; j < i + 6 && format[j] != '%' && j < format_len; ++j) {
                cur_format[j - i] = format[j];
            }
            cur_format[j - i] = '\0';
            i = j - 1;
            count += vfprintf(file, cur_format, args);
        }

        else {
            fputc(format[i], file);
            count++;
        }
    }
    va_end(args);
    return count;
}

int main() {
    char buffer[BUFSIZ];
    int count;
    printf("Test 1\n");
    count = custom_sprintf(buffer, "%d %Ro %Ro %Ro %.2f", 23, -120, 20000, 0, 3.22);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 2\n");
    count = custom_sprintf(buffer, "stroka %Ro", 6);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 3\n");
    count = custom_sprintf(buffer, "stroka %Roover", 563);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 4\n");
    count = custom_sprintf(buffer, "stroka %Roover", 10000);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 5\n");
    count = custom_sprintf(buffer, "stroka trtedd %Ro", 5);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 6\n");
    count = custom_sprintf(buffer, "stroka %", 1000);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 7\n");
    count = custom_sprintf(buffer, "stroka %Cv", 123, 2);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 8\n");
    count = custom_sprintf(buffer, "stroka %Cv", -123, 16);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 9\n");
    count = custom_sprintf(buffer, "stroka %CV", 123, 2);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 10\n");
    count = custom_sprintf(buffer, "stroka %CV", -123, 16);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 11\n");
    count = custom_sprintf(buffer, "stroka %to", "1111011", 2);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 12\n");
    count = custom_sprintf(buffer, "stroka %to", "-7b4", 16);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 13\n");
    count = custom_sprintf(buffer, "stroka %TO", "-7B3", 10);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 14\n");
    count = custom_sprintf(buffer, "%md", 0.1);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 15\n");
    count = custom_sprintf(buffer, "%mf", (float)12334.34);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    printf("Test 16\n");
    count = custom_sprintf(buffer, "%Zr", 10);
    printf("string: '%s'; count symbols: %d\n\n", buffer, count);

    FILE* output = fopen("output.txt", "w");
    if (output == NULL)
        return FILE_ERR;

    custom_fprintf(output, "%d %d %Ro %.2f\n", 3888, 3, 4, 3.22);

    custom_fprintf(output, "rororoRo%% %Ro\n", 6);

    custom_fprintf(output, "%Ro\n", 322222222);

    custom_fprintf(output, " %Roooon\n", 100000000);

    custom_fprintf(output, "%Ro %.2f bubedi\n", 5, 234567.89);

    custom_fprintf(output, " %\n", 100000000);

    custom_fprintf(output, " %Cv\n", 123, 2);

    custom_fprintf(output, "%Cv\n", -123, 16);

    custom_fprintf(output, "%CV\n", 123, 2);

    custom_fprintf(output, "stroka %CV\n", -123, 16);

    custom_fprintf(output, "aboba %to\n", "1111011", 2);

    custom_fprintf(output, "rrr %to\n", "-7b", 16);

    custom_fprintf(output, "to%TOto\n", "-7B", 10);

    custom_fprintf(output, "%mi\n", -123);

    custom_fprintf(output, "%mu\n", 123);

    custom_fprintf(output, "%md\n", 0.1);

    custom_fprintf(output, "%mf\n", -1.2);

    custom_fprintf(output, "%Zr\n", 10);

    fclose(output);
    return 0;
}