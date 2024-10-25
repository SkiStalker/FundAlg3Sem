#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef int (*scanFunc)(void **f, const char *s, ...);
typedef int (*vscanFunc)(void **f, const char *s, va_list valist);
typedef int (*seekFunc)(void **f, int prev);

int countOccurrences(int *result, const char *s, char c) {
    for (; *s != 0; s++)
        (*result) += (*s == c);
    return 0;
}

int advanceString(void **f, const char *s) {
    int len = strlen(s);
    int count;
    countOccurrences(&count, s, '%');
    char *s2 = malloc(len + count + 3);
    if (!s2)
        return -1;
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] == '%') {
            s2[j] = '%';
            j++;
            s2[j] = '*';
            j++;
            continue;
        }
        s2[j] = s[i];
        j++;
    }

    j += 2;
    int n = 0;
    do {
        s2[j - 2] = '%';
        s2[j - 1] = 'n';
        s2[j] = 0;
        sscanf(*f, s2, &n);
        j--;
    } while (n == 0 && j >= 2);

    char *c = *f;
    c += n;
    *f = c;
    free(s2);
    return 0;
}

int fscanfFunc(void **f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = vfscanf(*f, s, valist);
    va_end(valist);
    return res;
}

int sscanfFunc(void **f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = vsscanf(*f, s, valist);
    advanceString(f, s);
    va_end(valist);
    return res;
}

int vfscanfFunc(void **f, const char *s, va_list valist) {
    return vfscanf(*f, s, valist);
}

int vsscanfFunc(void **f, const char *s, va_list valist) {
    int res = vsscanf(*f, s, valist);
    advanceString(f, s);
    return res;
}

int fseekFunc(void **f, int prev) {
    ungetc(prev, *f);
    return 0;
}

int sseekFunc(void **f, int prev) {
    (void)prev;
    char *c = *f;
    c--;
    *f = c;
    return 0;
}

int parseDigit(int *digit, int base, char c, char start) {
    if ('0' <= c && c <= '0' + base - 1) {
        *digit = c - '0';
        return 0;
    }
    if (start <= c && c <= start + base - 11) {
        *digit = c - start + 10;
        return 0;
    }
    return 1;
}

int parseRomanNumeral(void **f, scanFunc scan, seekFunc seek, int *n) {
    char c;
    if (scan(f, "%c", &c) <= 0)
        return 0;
    while (isspace(c)) {
        if (scan(f, "%c", &c) <= 0)
            return 0;
    }
    bool neg = c == '-';
    if (neg)
        if (scan(f, "%c", &c) <= 0)
            return 0;
    if (c == '0') {
        *n = 0;
        return 1;
    }

    int romanValues[] = {1, 5, 10, 50, 100, 500, 1000};
    char romanSymbols[] = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};

    int count = sizeof(romanSymbols) / sizeof(romanSymbols[0]);
    int num = 0;
    int prev = 2000;
    int i;
    while (c) {
        for (i = 0; i < count; i++)
            if (c == romanSymbols[i])
                break;
        if (c != romanSymbols[i]) {
            seek(f, c);
            break;
        }
        if (prev < romanValues[i])
            num -= 2 * prev;

        num += romanValues[i];
        prev = romanValues[i];

        if (scan(f, "%c", &c) <= 0)
            break;
    }

    num = neg ? -num : num;
    *n = num;
    return 1;
}

int parseZeckendorf(void **f, scanFunc scan, seekFunc seek, unsigned int *n) {
    char c;
    char prevChar = '0';
    if (scan(f, "%c", &c) <= 0)
        return 0;
    while (isspace(c)) {
        if (scan(f, "%c", &c) <= 0)
            return 0;
    }

    int b = 1, t = 1, a = 1, num = 0;

    while (c) {
        if (c != '0' && c != '1') {
            seek(f, c);
            return 0;
        }
        if (c == '1' && prevChar == '1') {
            *n = num;
            return 1;
        }

        if (c == '1') {
            num += a;
        }

        t = a;
        a += b;
        b = t;

        prevChar = c;
        if (scan(f, "%c", &c) <= 0)
            break;
    }
    return 0;
}

int parseInteger(void **f, scanFunc scan, seekFunc seek, int *n, int base, char start) {
    if (base < 2 || base > 36)
        base = 10;

    char c;
    if (scan(f, "%c", &c) <= 0)
        return 0;
    while (isspace(c)) {
        if (scan(f, "%c", &c) <= 0)
            return 0;
    }
    bool neg = c == '-';
    if (neg)
        if (scan(f, "%c", &c) <= 0)
            return 0;

    int digit = 0, num = 0;
    while (c) {
        if (parseDigit(&digit, base, c, start)) {
            seek(f, c);
            break;
        }
        num = num * base + digit;
        if (scan(f, "%c", &c) <= 0)
            break;
    }
    num = neg ? -num : num;
    *n = num;
    return 1;
}

int scanfChoice(void **f, scanFunc scan, vscanFunc vscan, seekFunc seek, const char *s, va_list valist) {
    int len = strlen(s) + 1;
    char *str = malloc(len);
    if (!str)
        return -1;
    memcpy(str, s, len);
    char *ptr = str;
    char flags[] = {'c', 's', '[', 'd', 'i', 'u', 'o', 'x', 'X', 'n',
                    'a', 'A', 'e', 'E', 'f', 'F', 'g', 'G', 'p'};
    int flagsCount = sizeof(flags) / sizeof(flags[0]);
    int count = 0;

    while (str) {
        char *prev = str;
        str = strchr(str + 1, '%');
        if (str)
            *str = 0;

        if (strncmp(prev, "%Ro", 3) == 0) {
            int *n = va_arg(valist, int *);
            if (!parseRomanNumeral(f, scan, seek, n))
                break;
            prev += 3;
            count++;
        }
        if (strncmp(prev, "%Zr", 3) == 0) {
            unsigned int *n = va_arg(valist, unsigned int *);
            if (!parseZeckendorf(f, scan, seek, n))
                break;
            prev += 3;
            count++;
        }
        if (strncmp(prev, "%Cv", 3) == 0) {
            int *n = va_arg(valist, int *);
            int base = va_arg(valist, int);
            if (!parseInteger(f, scan, seek, n, base, 'a'))
                break;
            prev += 3;
            count++;
        }
        if (strncmp(prev, "%CV", 3) == 0) {
            int *n = va_arg(valist, int *);
            int base = va_arg(valist, int);
            if (!parseInteger(f, scan, seek, n, base, 'A'))
                break;
            prev += 3;
            count++;
        }
        if (prev[0] != '%' || (prev[0] == '%' && prev[1] == '%')) {
            bool end = false;
            char c;

            for (; prev < str; prev++) {
                if (!scan(f, "%c", &c)) {
                    end = true;
                    break;
                }
                if (*prev != c) {
                    end = true;
                    break;
                }
            }
            if (str)
                str[0] = '%';
            if (end)
                break;
            continue;
        }

        va_list copy;
        va_copy(copy, valist);
        int res = vscan(f, prev, copy);
        va_end(copy);

        if (prev[0] == '%' && prev[1] != '%' && prev[1] != '*' && res < 1)
            break;

        count++;

        if (prev[0] == '%') {
            for (; *prev != 0; prev++) {
                if (*prev == '*') {
                    break;
                }
                for (int i = 0; i < flagsCount; i++) {
                    if (*prev == flags[i]) {
                        va_arg(valist, void *);
                        break;
                    }
                }
            }
        }
        if (str)
            str[0] = '%';
    }

    free(ptr);
    return count;
}

int overfscanfFunc(FILE *f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = scanfChoice((void *)&f, fscanfFunc, vfscanfFunc, fseekFunc, s, valist);
    va_end(valist);
    return res;
}

int oversscanfFunc(char *f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = scanfChoice((void *)&f, sscanfFunc, vsscanfFunc, sseekFunc, s, valist);
    va_end(valist);
    return res;
}

int main() {
    int a, b, c;
    float f;
    
    // Пример с римскими числами и плавающей точкой
    char *s = "VIII -IX XX : 3.14";
    printf("Исходная строка: %s\n", s);
    int result = oversscanfFunc(s, "%Ro %Ro %Ro : %f", &a, &b, &c, &f);
    printf("%d\t%d %d %d %f\n", result, a, b, c, f);
    printf("\n");

    // Пример с числовыми системами счисления
    s = "10101, 7B, 19 : 42.0";
    printf("Исходная строка: %s\n", s);
    result = oversscanfFunc(s, "%Cv, %CV, %Cv : %f", &a, 2, &b, 16, &c, 10, &f);
    printf("%d\t%d %d %d %f\n", result, a, b, c, f);
    printf("\n");

    // Пример с кодами Цекендорфа и целым числом
    s = "100101011 : -999";
    printf("Исходная строка: %s\n", s);
    result = oversscanfFunc(s, "%Zr : %d", &a, &b);
    printf("%d\t%d %d\n", result, a, b);

    return 0;
}
