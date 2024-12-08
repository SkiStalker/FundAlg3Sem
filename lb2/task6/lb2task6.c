#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

typedef enum {
    SUCCESS,
    MEMORY_FAIL,
    INPUT_FAIL,
    FILE_FAIL
} ErrorCode;

int custom_sscanf(const char* buffer, const char* format, ...);
int custom_fprintf(FILE* file, const char* format, ...);
int custom_fscanf(FILE* stream, const char* format, ...);

int roman_to_int(const char* str, int* shift) {
    char symbols[13][3] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
    int values[] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };
    int index = 0, result = 0, limit;
    for (int i = 0; i < 13; ++i) {
        limit = 0;
        if (strlen(symbols[i]) == 1) {
            while (limit < 3 && str[index] == symbols[i][0]) {
                result += values[i];
                index++;
                limit++;
            }
        }
        if (strlen(symbols[i]) == 2) {
            while (limit < 3 && str[index] == symbols[i][0] && str[index + 1] == symbols[i][1]) {
                result += values[i];
                index += 2;
                limit++;
            }
        }
        *shift = index;
    }
    return result;
}

unsigned int fibonacci_to_int(const char* str, int* shift) {
    int fib_prev = 0, fib_cur = 1;
    int idx = 0;
    unsigned int result = 0, temp;
    while (str[idx] == '0' || str[idx] == '1') {
        if (str[idx] == '1') {
            result += fib_cur;
            if (str[idx + 1] == '1') {
                idx += 2;
                break;
            }
        }
        if (str[idx + 1] != '0' && str[idx + 1] != '1') {
            result = 0;
            idx++;
            break;
        }

        temp = fib_cur;
        fib_cur += fib_prev;
        fib_prev = temp;
        idx++;
    }
    *shift = idx;
    return result;
}

int custom_base_to_int(const char* str, int base, int* shift, int* flag, char mode) {
    *flag = 1;
    if (base < 2 || base > 36)
        base = 10;
    long result;
    *shift = 0;
    int len_str = strlen(str), i;
    char* temp_str = (char*)malloc((len_str + 1) * sizeof(char));
    if (!temp_str) {
        *flag = 0;
        *shift = 0;
        return 0;
    }
    for (i = 0; i < len_str; ++i) {
        if (!isalnum(str[i]) || (mode == 'v' && isupper(str[i])) || (mode == 'V' && islower(str[i])))
            break;
        temp_str[i] = str[i];
    }
    temp_str[i] = '\0';

    char* end_ptr;
    result = strtol(temp_str, &end_ptr, base);
    str = temp_str;
    while (str != end_ptr) {
        (*shift)++;
        str++;
    }
    free(temp_str);
    return (int)result;
}

void remove_spaces(const char* source, char* dest, int len) {
    int idx = 0;
    for (int i = 0; i < len; ++i) {
        if (!isspace(source[i]))
            dest[idx++] = source[i];
    }
    dest[idx] = '\0';
}

int custom_fscanf(FILE* stream, const char* format, ...) {
    if (!(stream && format))
        return -1;

    va_list args;
    va_start(args, format);

    int len_format = strlen(format), count = 0;

    char* new_format = (char*)malloc((len_format + 1) * sizeof(char));
    if (!new_format)
        return -1;

    remove_spaces(format, new_format, len_format);
    len_format = strlen(new_format);
    char c = getc(stream);
    int idx_buff = 0;
    for (int i = 0; i < len_format; ++i) {
        if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'R' && new_format[i + 2] == 'o') {
            while (isspace(c)) {
                c = getc(stream);
            }
            int* x = va_arg(args, int*);
            int shift;
            char* buffer = (char*)malloc(16 * sizeof(char));
            if (!buffer) {
                va_end(args);
                break;
            }
            idx_buff = 0;
            while (c == 'M' || c == 'C' || c == 'D' || c == 'X' || c == 'L' || c == 'I' || c == 'V') {
                buffer[idx_buff++] = c;
                c = getc(stream);
            }
            buffer[idx_buff] = '\0';
            ungetc(c, stream);
            *x = roman_to_int(buffer, &shift);
            if (*x)
                count++;
            i += 2;
            free(buffer);
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'Z' && new_format[i + 2] == 'r') {
            while (isspace(c)) {
                c = getc(stream);
            }
            unsigned int* x = va_arg(args, unsigned int*);
            int shift;
            char* buffer = (char*)malloc(50 * sizeof(char));
            if (!buffer) {
                va_end(args);
                break;
            }
            idx_buff = 0;
            while (c == '0' || c == '1') {
                buffer[idx_buff++] = c;
                c = getc(stream);
            }
            buffer[idx_buff] = '\0';
            ungetc(c, stream);
            *x = fibonacci_to_int(buffer, &shift);
            if (*x)
                count++;
            i += 2;
            free(buffer);
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'C' && (new_format[i + 2] == 'v' || new_format[i + 2] == 'V')) {
            while (isspace(c)) {
                c = getc(stream);
            }
            int* x = va_arg(args, int*);
            int base = va_arg(args, int);
            base = (base < 2 || base > 36) ? 10 : base;
            int shift, flag;

            char* buffer = (char*)malloc(50 * sizeof(char));
            if (!buffer) {
                va_end(args);
                break;
            }
            idx_buff = 0;
            while (isdigit(c) || ((islower(c) && new_format[i + 2] == 'v') || (isupper(c) && new_format[i + 2] == 'V'))) {
                buffer[idx_buff++] = c;
                c = getc(stream);
            }
            buffer[idx_buff] = '\0';
            ungetc(c, stream);
            *x = custom_base_to_int(buffer, base, &shift, &flag, new_format[i + 2]);
            if (flag)
                count++;
            i += 2;
            free(buffer);
        }
        else if (new_format[i] == '%') {
            void* x = va_arg(args, void*);
            char* subformat = (char*)malloc((len_format - i + 4) * sizeof(char));
            if (!subformat) {
                va_end(args);
                free(new_format);
                return count;
            }
            int ind_format = 1;
            subformat[0] = '%';
            i++;
            while (new_format[i] != '%' && new_format[i] != '\0' && new_format[i - 1] != 's') {
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
        else {
            while (isspace(c)) {
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

int custom_sscanf(const char* buffer, const char* format, ...) {
    if (!(buffer && format))
        return -1;

    va_list args;
    va_start(args, format);

    int len_format = strlen(format), count = 0;

    char* new_format = (char*)malloc((len_format + 1) * sizeof(char));
    if (!new_format)
        return -1;

    remove_spaces(format, new_format, len_format);
    len_format = strlen(new_format);

    int idx_buff = 0;
    for (int i = 0; i < len_format; ++i) {
        if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'R' && new_format[i + 2] == 'o') {
            while (isspace(buffer[idx_buff]))
                idx_buff++;
            int* x = va_arg(args, int*);
            int shift;
            *x = roman_to_int(buffer + idx_buff, &shift);
            if (*x)
                count++;
            i += 2;
            idx_buff += shift;
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'Z' && new_format[i + 2] == 'r') {
            while (isspace(buffer[idx_buff]))
                idx_buff++;
            unsigned int* x = va_arg(args, unsigned int*);
            int shift;
            *x = fibonacci_to_int(buffer + idx_buff, &shift);
            if (*x)
                count++;
            i += 2;
            idx_buff += shift;
        }
        else if (new_format[i] == '%' && i < len_format - 2 && new_format[i + 1] == 'C' && (new_format[i + 2] == 'v' || new_format[i + 2] == 'V')) {
            while (isspace(buffer[idx_buff]))
                idx_buff++;
            int* x = va_arg(args, int*);
            int base = va_arg(args, int);
            int shift, flag;
            *x = custom_base_to_int(buffer + idx_buff, base, &shift, &flag, new_format[i + 2]);
            if (flag)
                count++;
            i += 2;
            idx_buff += shift;
        }
        else if (new_format[i] == '%') {
            void* x = va_arg(args, void*);
            char* subformat = (char*)malloc((len_format - i + 4) * sizeof(char));
            if (!subformat) {
                va_end(args);
                free(new_format);
                return count;
            }
            int ind_format = 1;
            subformat[0] = '%';
            i++;
            while (new_format[i] != '%' && new_format[i] != '\0' && new_format[i - 1] != 's') {
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
        else {
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


int main() {
    int num1, num2, count, num3, num4;
    float float1, float2;
    unsigned int unsigned_int;
    char str[20];

    puts("Test1");
    puts("String: 'This is string: ccc 123 1000 and 4555'");
    count = custom_sscanf("Thissvvv is string:ccc 123 1000 and 4555", "%5svvv is string :%Cv %d  %CV and%d", str, &num1, 16, &num2, &num3, 10, &num4);
    printf("Output: %d, %d, %d, %d, %s; Count scanned: %d\n\n\n", num1, num2, num3, num4, str, count);

    puts("Test2");
    puts("String: 'This is the number 10: 0010011'");
    count = custom_sscanf("This is the number 10: 0010011", "This is the number 10: %Zr", &unsigned_int);
    printf("Output: %u; Count scanned: %d\n\n\n", unsigned_int, count);

    puts("Test3");
    puts("String: 'This is the number 10 & 3954: X & MMMCMLIV'");
    count = custom_sscanf("This is the number 10 & 3954: X & MMMCMLIV", "This is the number 10 & 3954: %Ro & %Ro", &num1, &num2);
    printf("Output: %d %d; Count scanned: %d\n\n\n", num1, num2, count);

    puts("Test4");
    puts("String: 'This is float, float and Ro:  1.33, 9.23, XIX'");
    count = custom_sscanf("This is float, float and Ro:  1.33, 9.23, XIX", "This is float, float and Ro:%f, %f, %Ro", &float1, &float2, &num1);
    printf("Output: %f %f %d; Count scanned: %d\n\n\n", float1, float2, num1, count);

    puts("Test5");
    puts("String: '194394'");
    count = custom_sscanf("194394", "%Cv", &num1, 100);
    printf("Output: %d; Count scanned: %d\n\n\n", num1, count);
    FILE* stream = fopen("test1.txt", "r");
    if (!stream)
        return FILE_FAIL;

    puts("Test1");
    puts("String: 'This is string: ccc 123 1000 and 4555'");
    count = custom_fscanf(stream, "This is string :%Cv %d  %CV and%d", &num1, 16, &num2, &num3, 10, &num4);
    printf("Output: %d, %d, %d, %d; Count scanned: %d\n\n\n", num1, num2, num3, num4, count);
    fclose(stream);

    stream = fopen("test2.txt", "r");
    if (!stream)
        return FILE_FAIL;
    puts("Test2");
    puts("String: 'This is the number 10: 0010011'");
    count = custom_fscanf(stream, "This is the number 10: %Zr", &unsigned_int);
    printf("Output: %u; Count scanned: %d\n\n\n", unsigned_int, count);
    fclose(stream);

    stream = fopen("test3.txt", "r");
    if (!stream)
        return FILE_FAIL;
    puts("Test3");
    puts("String: 'This is the number 10 & 3954: X & MMMCMLIV'");
    count = custom_fscanf(stream, "This is the number 10 & 3954: %Ro & %Ro", &num1, &num2);
    printf("Output: %d %d; Count scanned: %d\n\n\n", num1, num2, count);
    fclose(stream);

    stream = fopen("test4.txt", "r");
    if (!stream)
        return FILE_FAIL;
    puts("Test4");
    puts("String: 'This is float, float and Ro:  1.33, 9.23, XIX'");
    count = custom_fscanf(stream, "This is float, float and Ro:%f, %f, %Ro", &float1, &float2, &num1);
    printf("Output: %f %f %d; Count scanned: %d\n\n\n", float1, float2, num1, count);
    fclose(stream);

    stream = fopen("test5.txt", "r");
    if (!stream)
        return FILE_FAIL;
    puts("Test5");
    puts("String: '194394'");
    count = custom_fscanf(stream, "%Cv", &num1, 100);
    printf("Output: %d; Count scanned: %d\n\n\n", num1, count);
    fclose(stream);

    return 0;
}
