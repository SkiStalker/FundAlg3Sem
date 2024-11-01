#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Status {
    S_OK,
    S_MALLOC_ERROR,
    S_PARSE_ERROR,
} Status;

typedef int (*mask)(int);
int parse_field_str(char **res, const char **start, mask m);
int parse_field_float(float *res, const char **start, mask m);
int parse_field_uint(unsigned long *res, const char **start, mask m);
int parse_field_char(char *res, char const **start, mask m);

typedef struct Polynomial {
    int n;
    double *fs; // array of n + 1 len
} Poly;

int poly_init(Poly *res, int n);
int poly_parse(Poly *res, const char *s);
int poly_free(Poly a);
int poly_print(Poly a);
int poly_mul_by_double(Poly *res, Poly a, double mul);
int poly_add(Poly *res, Poly a, Poly b);
int poly_sub(Poly *res, Poly a, Poly b);
int poly_mult(Poly *res, Poly a,Poly b);
int poly_div(Poly *res, Poly a, Poly b);
int poly_mod(Poly *res, Poly a, Poly b);
int poly_comp(Poly *res, Poly a, Poly b);
int poly_diff(Poly *res, Poly a);
int poly_eval(double *res, Poly a, double x);

int powd(double *res, double x, unsigned n);
int poly_pow(Poly *res, Poly p, unsigned n);


int parse_field_str(char **res, const char **start, mask m) {
    const char *end = *start;
    while (*end && !m(*end)) {
        end++;
    }
    int n = end - *start;
    *res = malloc(n + 1);
    if (!res)
        return S_MALLOC_ERROR;
    memcpy(*res, *start, n);
    (*res)[n] = 0;
    *start = end + 1;
    return S_OK;
}

int parse_field_uint(unsigned long *res, const char **start, mask m) {
    char *ptr;
    *res = strtoul(*start, &ptr, 10);
    bool valid = *ptr == 0;
    valid = valid | m(*ptr);
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        fprintf(stderr, "ERROR: failed to parse [%s] as unsigned\n", *start);
        *ptr = tmp;
        return S_PARSE_ERROR;
    }

    *start = ptr + 1;
    return S_OK;
}

int parse_field_float(float *res, const char **start, mask m) {
    char *ptr;
    *res = strtof(*start, &ptr);
    bool valid = *ptr == 0;
    valid = valid | m(*ptr);
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        fprintf(stderr, "ERROR: failed to parse [%s] as float\n", *start);
        *ptr = tmp;
        return S_PARSE_ERROR;
    }

    *start = ptr + 1;
    return S_OK;
}

int parse_field_char(char *res, const char **start, mask m) {
    char *str;
    parse_field_str(&str, start, m);
    if (strlen(str) != 1) {
        fprintf(stderr, "ERROR: expected char, got string [%s]\n", str);
        free(str);
        return S_PARSE_ERROR;
    }
    *res = *str;
    free(str);
    return S_OK;
}

int is_separator(int c) {
    return isspace(c) || c == '+' || c == '-' || c == ',' || c == ')' ||
           c == '\0';
}

int poly_parse(Poly *res, const char *str) {
    res->n = strchr(str, 'x') != 0;
    const char *s = str;
    while (true) {
        char *end;
        s = strchr(s, '^');
        if (!s)
            break;
        s++;
        int pow = strtoul(s, &end, 10);
        if (*end != 0 && !is_separator(*end)) {
            int str_len = end - s;
            fprintf(stderr, "ERROR: failed to parse [%*s] as number\n", str_len,
                    s);
            return S_PARSE_ERROR;
        }
        if (pow > res->n)
            res->n = pow;
    }

    res->fs = malloc((res->n + 1) * sizeof(double));
    if (!res->fs)
        return S_MALLOC_ERROR;
    memset(res->fs, 0, (res->n + 1) * sizeof(double));

    s = str;
    while (*s) {
        while (isspace(*s))
            s++;
        bool neg = *s == '-';
        if (neg || *s == '+')
            s++;
        while (isspace(*s))
            s++;
        if (*s == 0)
            break;
        char *end;
        double coef = strtof(s, &end);
        if (*end != 'x' && !is_separator(*end)) {
            int str_len = end - s;
            fprintf(stderr, "ERROR: failed to parse [%*s] as float\n", str_len,
                    s);
            return S_PARSE_ERROR;
        }
        if (s == end)
            coef = 1;
        if (neg)
            coef *= -1;

        s = end;
        if (*s != 'x') { // a
            res->fs[0] = coef;
            continue;
        }
        s++;
        if (*s != '^') { // a * x
            res->fs[1] = coef;
            continue;
        }
        s++;
        int pow = strtoul(s, &end, 10);
        res->fs[pow] = coef;
        s = end;
    }

    return S_OK;
}

int poly_print(Poly a) {
    if (a.n == 0) {
        printf("%f", a.fs[0]);
        return S_OK;
    }
    for (int n = a.n; n >= 0; n--) {
        if (a.fs[n] == 0)
            continue;
        char op = a.fs[n] < 0 ? '-' : '+';
        char *x;
        if (n == 0)
            x = "";
        else if (n == 1)
            x = "x";
        else
            x = "x^%d";
        if (n == a.n)
            printf("%f", a.fs[n]);
        else
            printf("%c %f", op, fabs(a.fs[n]));
        printf(x, n);
        if (n > 0)
            printf(" ");
    }
    return 0;
}

#define SWAP_DESC(a, b)                                                        \
    {                                                                          \
        if (a.n < b.n) {                                                       \
            const Poly _temp_val_ = a;                                         \
            a = b;                                                             \
            b = _temp_val_;                                                    \
        }                                                                      \
    }

int poly_init(Poly *res, int n) {
    res->n = n;
    res->fs = malloc((res->n + 1) * sizeof(double));
    if (!res->fs)
        return S_MALLOC_ERROR;
    memset(res->fs, 0, (res->n + 1) * sizeof(double));
    return S_OK;
}

int poly_free(Poly a) {
    free(a.fs);
    return S_OK;
}

int poly_mul_by_double(Poly *res, Poly a, double mul) {
    poly_init(res, a.n);
    for (int i = 0; i <= res->n; i++)
        res->fs[i] = a.fs[i] * mul;
    return S_OK;
}

int poly_add(Poly *res, Poly a, Poly b) {
    int r, i;
    SWAP_DESC(a, b);
    if ((r = poly_init(res, a.n)))
        return r;
    for (i = 0; i <= b.n; i++)
        res->fs[i] = a.fs[i] + b.fs[i];
    for (; i <= a.n; i++)
        res->fs[i] = a.fs[i];
    while (res->n > 0 && res->fs[res->n] == 0)
        res->n--;

    return S_OK;
}

int poly_sub(Poly *res, Poly a, Poly b) {
    Poly neg;
    int r;
    if ((r = poly_mul_by_double(&neg, b, -1)))
        return r;
    r = poly_add(res, a, neg);
    poly_free(neg);
    return r;
}

int poly_mult(Poly *res, Poly a, Poly b) {
    SWAP_DESC(a, b);
    int r;
    if ((r = poly_init(res, a.n + b.n)))
        return r;
    for (int i = 0; i <= a.n; i++) {
        for (int j = 0; j <= b.n; j++) {
            res->fs[i + j] += a.fs[i] * b.fs[j];
        }
    }
    return S_OK;
}

int powd(double *res, double x, unsigned exp) {
    *res = 1;
    while (exp) {
        if (exp & 1)
            (*res) *= x;
        exp >>= 1;
        x *= x;
    }
    return 0;
}

int poly_pow(Poly *res, Poly p, unsigned exp) {
    Poly result, base;
    if (poly_init(&result, 0) || poly_mul_by_double(&base, p, 1))
        return S_MALLOC_ERROR;
    result.fs[0] = 1;
    while (exp) {
        if (exp & 1) {
            Poly new_r;
            if (poly_mult(&new_r, result, base)) {
                poly_free(result);
                poly_free(base);
                return S_MALLOC_ERROR;
            }
            poly_free(result);
            result = new_r;
        }
        exp >>= 1;
        Poly new_base;
        if (poly_mult(&new_base, base, base)) {
            poly_free(base);
            poly_free(result);
            return S_MALLOC_ERROR;
        }
        poly_free(base);
        base = new_base;
    }
    poly_free(base);
    *res = result;
    return S_OK;
}

int poly_diff(Poly *res, Poly a) {
    if (poly_init(res, a.n - 1))
        return S_MALLOC_ERROR;
    for (int i = 1; i <= a.n; i++) {
        res->fs[i - 1] = i * a.fs[i];
    }
    return S_OK;
}
int poly_eval(double *res, Poly a, double x) {
    *res = 0;
    double xp = 1;
    for (int i = 0; i <= a.n; i++) {
        *res += xp * a.fs[i];
        xp *= x;
    }
    return S_OK;
}

int poly_div_inner(Poly *div, Poly *mod, Poly a, Poly b) {
    // a / b
    if (b.n > a.n) {
        if (poly_init(div, 0))
            return S_MALLOC_ERROR;
        if (poly_mul_by_double(mod, a, 1)) {
            poly_free(*div);
            return S_MALLOC_ERROR;
        }
        return S_OK;
    }

    Poly r_mod;
    Poly r_div;
    if (poly_init(&r_div, 0))
        return S_MALLOC_ERROR;
    if (poly_mul_by_double(&r_mod, a, 1)) {
        poly_free(r_div);
        return S_MALLOC_ERROR;
    };
    while (r_mod.n >= b.n) {
        double power = r_mod.n - b.n;
        Poly r_cur;
        if (poly_init(&r_cur, power)) {
            poly_free(r_div);
            poly_free(r_mod);
            return S_MALLOC_ERROR;
        };
        r_cur.fs[r_cur.n] = r_mod.fs[r_mod.n] / b.fs[b.n];

        Poly r_add;
        int r = poly_add(&r_add, r_div, r_cur);
        poly_free(r_div);
        r_div = r_add;
        if (r) {
            poly_free(r_cur);
            poly_free(r_mod);
            poly_free(r_div);
            return r;
        }

        Poly r_mul;
        r = poly_mult(&r_mul, r_cur, b);
        poly_free(r_cur);
        if (r) {
            poly_free(r_mod);
            poly_free(r_div);
            return r;
        }

        Poly r_sub;
        r = poly_sub(&r_sub, r_mod, r_mul);
        poly_free(r_mod);
        poly_free(r_mul);
        r_mod = r_sub;
        if (r) {
            poly_free(r_mod);
            poly_free(r_div);
            return r;
        }
    }
    *mod = r_mod;
    *div = r_div;
    return S_OK;
}

int poly_div(Poly *res, Poly a, Poly b) {
    Poly mod;
    if (poly_div_inner(res, &mod, a, b))
        return S_MALLOC_ERROR;
    poly_free(mod);
    return S_OK;
}
int poly_mod(Poly *res, Poly a, Poly b) {
    Poly div;
    if (poly_div_inner(&div, res, a, b))
        return S_MALLOC_ERROR;
    poly_free(div);
    return S_OK;
}

int poly_comp(Poly *res, Poly a, Poly b) {
    Poly s;
    if (poly_init(&s, 0))
        return S_MALLOC_ERROR;
    for (int i = 0; i <= a.n; i++) {
        if (a.fs[i] == 0)
            continue;
        Poly powed;
        if (poly_pow(&powed, b, i)) {
            poly_free(s);
            return S_MALLOC_ERROR;
        }
        Poly sub_res;
        int r = poly_mul_by_double(&sub_res, powed, a.fs[i]);
        poly_free(powed);
        if (r) {
            poly_free(s);
            return r;
        }
        Poly sum;
        r = poly_add(&sum, s, sub_res);
        poly_free(sub_res);
        poly_free(s);
        s = sum;
        if (r) {
            poly_free(s);
            return r;
        }
    }
    *res = s;
    return S_OK;
}

