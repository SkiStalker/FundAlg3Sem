#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void intToRoman(int num, char *buffer) {
    const char *roman_numerals[] = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };
    const int values[] = {
        1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
    };

    buffer[0] = '\0';

    for (int i = 0; i < 13 && num > 0; i++) {
        while (num >= values[i]) {
            strcat(buffer, roman_numerals[i]);
            num -= values[i];
        }
    }
}

void uintToCenkendorf(unsigned int num, char *buffer) {
    if (num == 0) {
        strcpy(buffer, "1");
        return;
    }

    unsigned int a = 1;
    unsigned int b = 2;
    unsigned int fib;
    char temp[100] = {0};
    int idx = 0;

    while (b <= num) {
        fib = a + b;
        a = b;
        b = fib;
    }

    while (b > 0) {
        if (num >= a) {
            num -= a;
            temp[idx++] = '1';
        } else {
            if (idx > 0) {
                temp[idx++] = '0';
            }
        }
        fib = a;
        a = b - a;
        b = fib;
    }

    temp[idx++] = '1'; 
    temp[idx] = '\0';

    for (int i = 0; i < idx / 2; i++) {
        char t = temp[i];
        temp[i] = temp[idx - i - 1];
        temp[idx - i - 1] = t;
    }
    
    strcpy(buffer, temp);
}

void intToBase(int num, int base, char *buffer, int upper) {
    if (base < 2 || base > 36) {
        base = 10;
    }

    char digits_lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char digits_upper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *digits = upper ? digits_upper : digits_lower;

    char temp[100];
    int idx = 0;

    if (num < 0) {
        num = -num;
        buffer[idx++] = '-';
    }

    do {
        temp[idx++] = digits[num % base];
        num /= base;
    } while (num > 0);

    temp[idx] = '\0';

    for (int i = 0; i < idx / 2; i++) {
        char t = temp[i];
        temp[i] = temp[idx - i - 1];
        temp[idx - i - 1] = t;
    }

    strcpy(buffer, temp);
}

int strToInt(const char *str, int base) {
    int len = strlen(str);
    int num = 0;

    for (int i = 0; i < len; i++) {
        char c = tolower(str[i]);
        int value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'a' && c <= 'z') {
            value = c - 'a' + 10;
        } else {
            return 0;
        }

        if (value >= base) {
            return 0;
        }

        num = num * base + value;
    }

    return num;
}

void dumpMemorySigned(int num, char *buffer) {
    unsigned char *bytes = (unsigned char *)&num;
    char temp[35] = {0};

    for (size_t i = 0; i < sizeof(num); i++) {
        for (int j = 7; j >= 0; j--) {
            temp[i * 9 + (7 - j)] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
        if (i < sizeof(num) - 1) {
            temp[i * 9 + 8] = ' ';
        }
    }

    strcpy(buffer, temp);
}

void dumpMemoryUnsigned(unsigned int num, char *buffer) {
    unsigned char *bytes = (unsigned char *)&num;
    char temp[35] = {0};

    for (size_t i = 0; i < sizeof(num); i++) {
        for (int j = 7; j >= 0; j--) {
            temp[i * 9 + (7 - j)] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
        if (i < sizeof(num) - 1) {
            temp[i * 9 + 8] = ' ';
        }
    }

    strcpy(buffer, temp);
}

void dumpMemoryFloat(float num, char *buffer) {
    unsigned char *bytes = (unsigned char *)&num;
    char temp[35] = {0};

    for (size_t i = 0; i < sizeof(num); i++) {
        for (int j = 7; j >= 0; j--) {
            temp[i * 9 + (7 - j)] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
        if (i < sizeof(num) - 1) {
            temp[i * 9 + 8] = ' ';
        }
    }

    strcpy(buffer, temp);
}

void dumpMemoryDouble(double num, char *buffer) {
    unsigned char *bytes = (unsigned char *)&num;
    char temp[75] = {0};

    for (size_t i = 0; i < sizeof(num); i++) {
        for (int j = 7; j >= 0; j--) {
            temp[i * 9 + (7 - j)] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
        if (i < sizeof(num) - 1) {
            temp[i * 9 + 8] = ' ';
        }
    }

    strcpy(buffer, temp);
}


int overfprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    char buffer[100];
    int result = 0;

    for (const char *p = format; *p; p++) {
        if (*p == '%') {
            p++;

            if (*p == 'R' && *(p + 1) == 'o') {
                int num = va_arg(args, int);
                intToRoman(num, buffer);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else if (*p == 'Z' && *(p + 1) == 'r') {
                unsigned int num = va_arg(args, unsigned int);
                uintToCenkendorf(num, buffer);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else if (*p == 'C' && *(p + 1) == 'v') {
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                intToBase(num, base, buffer, 0);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else if (*p == 'C' && *(p + 1) == 'V') {
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                intToBase(num, base, buffer, 1);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else if (*p == 't' && *(p + 1) == 'o') {
                const char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = strToInt(num_str, base);
                result += fprintf(stream, "%d", num);
                p++;
            } else if (*p == 'T' && *(p + 1) == 'O') {
                const char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = strToInt(num_str, base);
                result += fprintf(stream, "%d", num);
                p++;
            } else if (*p == 'm' && *(p + 1) == 'i') {
                int num = va_arg(args, int);
                dumpMemorySigned(num, buffer);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else if (*p == 'm' && *(p + 1) == 'u') {
                unsigned int num = va_arg(args, unsigned int);
                dumpMemoryUnsigned(num, buffer);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else if (*p == 'm' && *(p + 1) == 'd') {
                double num = va_arg(args, double);
                dumpMemoryDouble(num, buffer);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else if (*p == 'm' && *(p + 1) == 'f') {
                float num = va_arg(args, double);
                dumpMemoryFloat(num, buffer);
                result += fprintf(stream, "%s", buffer);
                p++;
            } else {
                va_list args_copy;
                va_copy(args_copy, args);
                result += vfprintf(stream, p - 1, args_copy);
                va_end(args_copy);

                while (*p != '\0' && !strchr("diufcsxX%", *p)) p++;
            }
        } else {
            result += fprintf(stream, "%c", *p);
        }
    }

    va_end(args);
    return result;
}

int oversprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    char buffer[100];
    int result = 0;
    int offset = 0;

    for (const char *p = format; *p; p++) {
        if (*p == '%') {
            p++;

            if (*p == 'R' && *(p + 1) == 'o') {
                int num = va_arg(args, int);
                intToRoman(num, buffer);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else if (*p == 'Z' && *(p + 1) == 'r') {
                unsigned int num = va_arg(args, unsigned int);
                uintToCenkendorf(num, buffer);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else if (*p == 'C' && *(p + 1) == 'v') {
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                intToBase(num, base, buffer, 0);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else if (*p == 'C' && *(p + 1) == 'V') {
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                intToBase(num, base, buffer, 1);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else if (*p == 't' && *(p + 1) == 'o') {
                const char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = strToInt(num_str, base);
                result += sprintf(str + offset, "%d", num);
                offset += snprintf(str + offset, 100, "%d", num);
                p++;
            } else if (*p == 'T' && *(p + 1) == 'O') {
                const char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = strToInt(num_str, base);
                result += sprintf(str + offset, "%d", num);
                offset += snprintf(str + offset, 100, "%d", num);
                p++;
            } else if (*p == 'm' && *(p + 1) == 'i') {
                int num = va_arg(args, int);
                dumpMemorySigned(num, buffer);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else if (*p == 'm' && *(p + 1) == 'u') {
                unsigned int num = va_arg(args, unsigned int);
                dumpMemoryUnsigned(num, buffer);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else if (*p == 'm' && *(p + 1) == 'd') {
                double num = va_arg(args, double);
                dumpMemoryDouble(num, buffer);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else if (*p == 'm' && *(p + 1) == 'f') {
                float num = va_arg(args, double);
                dumpMemoryFloat(num, buffer);
                int len = strlen(buffer);
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;
                p++;
            } else {
                va_list args_copy;
                va_copy(args_copy, args);
                int len = vsnprintf(buffer, sizeof(buffer), p - 1, args_copy);
                va_end(args_copy);
                
                strncpy(str + offset, buffer, len);
                offset += len;
                result += len;

                while (*p != '\0' && *p != '%' && !strchr("diufcsxX", *p)) p++;
            }
        } else {
            str[offset++] = *p;
            result++;
        }
    }
    
    str[offset] = '\0';
    va_end(args);
    return result;
}

int main() {
    printf("Demonstrating the overfprintf function:\n\n");

    overfprintf(stdout, "Year in Roman numerals: %Ro\n", 1987);
    overfprintf(stdout, "Zeckendorf number for 10: %Zr\n", 10);
    overfprintf(stdout, "Lowercase base-16 representation: %Cv\n", -255, 16);
    overfprintf(stdout, "Uppercase base-36 representation: %CV\n", 42, 36);
    overfprintf(stdout, "Default base (out of range example): %Cv\n", 42, 50);
    overfprintf(stdout, "Base-16 to decimal conversion: %to\n", "1a", 16);
    overfprintf(stdout, "Base-36 to decimal conversion: %TO\n", "z", 36);
    overfprintf(stdout, "Signed integer memory dump: %mi\n", -13);
    overfprintf(stdout, "Unsigned integer memory dump: %mu\n", 255);
    overfprintf(stdout, "Float memory dump example: %mf\n", 3.14f);
    overfprintf(stdout, "Double memory dump example: %md\n", 3.14159);

    printf("\n");
    printf("Testing oversprintf for various formats:\n\n");

    char buffer[1000];

    oversprintf(buffer, "Roman numeral conversion for 1987: %Ro\n", 1987);
    printf("%s", buffer);

    oversprintf(buffer, "Zeckendorf representation example: %Zr\n", 10);
    printf("%s", buffer);

    oversprintf(buffer, "Negative number in base 16 (lowercase): %Cv\n", -255, 16);
    printf("%s", buffer);

    oversprintf(buffer, "Positive number in base 36 (uppercase): %CV\n", 42, 36);
    printf("%s", buffer);

    oversprintf(buffer, "Base out of range (default): %Cv\n", 42, 50);
    printf("%s", buffer);

    oversprintf(buffer, "Hexadecimal to decimal conversion: %to\n", "1a", 16);
    printf("%s", buffer);

    oversprintf(buffer, "Alphabetic base-36 to decimal: %TO\n", "z", 36);
    printf("%s", buffer);

    oversprintf(buffer, "Signed memory representation: %mi\n", -255);
    printf("%s", buffer);

    oversprintf(buffer, "Unsigned integer dump: %mu\n", 255);
    printf("%s", buffer);

    oversprintf(buffer, "Floating point memory dump: %mf\n", 3.14f);
    printf("%s", buffer);

    oversprintf(buffer, "Double precision memory dump: %md\n", 3.14159);
    printf("%s", buffer);

    return 0;
}
