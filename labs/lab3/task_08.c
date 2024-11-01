#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task_08_lib.h"

int is_open_bracket(int c) {
    return c == '(';
}

int is_comma_or_bracket(int c) {
    return c == ')' || c == ',';
}

typedef int (*handle)(Poly *r, Poly a, Poly b);

int handle_add(Poly *r, Poly a, Poly b) {
    poly_free(*r);
    if (poly_add(r, a, b))
        return S_MALLOC_ERROR;
    poly_print(*r);
    printf("\n");
    return S_OK;
}
int handle_sub(Poly *r, Poly a, Poly b) {
    poly_free(*r);
    if (poly_sub(r, a, b))
        return S_MALLOC_ERROR;
    poly_print(*r);
    printf("\n");
    return S_OK;
}
int handle_mult(Poly *r, Poly a, Poly b) {
    poly_free(*r);
    if (poly_mult(r, a, b))
        return S_MALLOC_ERROR;
    poly_print(*r);
    printf("\n");
    return S_OK;
}
int handle_div(Poly *r, Poly a, Poly b) {
    poly_free(*r);
    if (poly_div(r, a, b))
        return S_MALLOC_ERROR;
    poly_print(*r);
    printf("\n");
    return S_OK;
}
int handle_mod(Poly *r, Poly a, Poly b) {
    poly_free(*r);
    if (poly_mod(r, a, b))
        return S_MALLOC_ERROR;
    poly_print(*r);
    printf("\n");
    return S_OK;
}
int handle_eval(Poly *r, Poly a, Poly b) {
    (void)r;
    if (b.n > 0) {
        printf("ERROR: expected double as second argument, got polynom\n");
        return S_PARSE_ERROR;
    }
    double res;
    poly_eval(&res, a, b.fs[0]);
    printf("%f\n", res);
    return S_OK;
}
int handle_diff(Poly *r, Poly a, Poly b) {
    poly_free(*r);
    (void)b;
    if (poly_diff(r, a))
        return 1;
    poly_print(*r);
    printf("\n");
    return S_OK;
}
int handle_cmps(Poly *r, Poly a, Poly b) {
    poly_free(*r);
    if (poly_comp(r, a, b))
        return S_MALLOC_ERROR;
    poly_print(*r);
    printf("\n");
    return S_OK;
}

#define FREE_ALL()                                                             \
    {                                                                          \
        free(op);                                                              \
        free(a);                                                               \
        free(b);                                                               \
        free(line);                                                            \
    }

int main(int argc, const char *argw[]) {
    if (argc <= 1) {
        printf("ERROR: no input file provided\n");
        return 1;
    }
    FILE *f = fopen(argw[1], "r");
    if (!f) {
        printf("ERROR: failed to open file [%s]\n", argw[1]);
        return 1;
    }
    Poly buf;
    poly_init(&buf, 0);
    buf.fs[0] = 0;
    const char *ops[] = {"Add", "Sub",  "Mult", "Div",
                         "Mod", "Eval", "Diff", "Cmps"};
    handle handles[] = {handle_add, handle_sub,  handle_mult, handle_div,
                        handle_mod, handle_eval, handle_diff, handle_cmps};
    char *line = NULL;
    size_t line_len = 0;
    bool comment = false;
    while (true) {
        int n = getline(&line, &line_len, f);
        if (n <= 0)
            break;
        n--;
        line[n] = 0;

        const char *s = line;
        if (comment) {
            s = strchr(s, ']');
            if (s == NULL)
                continue;
            comment = false;
            s++;
        }
        while (isspace(*s))
            s++;
        if (*s == 0 || *s == '%')
            continue;
        if (*s == '[') {
            comment = true;
            continue;
        }

        char *op, *a, *b;
        if (parse_field_str(&op, &s, is_open_bracket) ||
            parse_field_str(&a, &s, is_comma_or_bracket) ||
            parse_field_str(&b, &s, is_comma_or_bracket)) {
            printf("ERROR: Buy more ram\n");
            return 1;
        }

        if (*s != ';' && *b != ';') {
            printf("ERROR: line must end in ;\n");
            FREE_ALL();
            return 1;
        }
        Poly x, y;
        if (*b != ';') {
            if (poly_parse(&x, a) || poly_parse(&y, b)) {
                FREE_ALL();
                if (x.fs)
                    poly_free(x);
                return 1;
            }
        } else {
            if (poly_parse(&y, a) || poly_mul_by_double(&x, buf, 1)) {
                FREE_ALL();
                return 1;
            }
        }

        handle *h = NULL;
        for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
            if (strcmp(op, ops[i]) == 0) {
                h = &handles[i];
                break;
            }
        }
        if (h) {
            int r = (*h)(&buf, x, y);
            if (r)
                return r;
        } else {
            printf("ERROR: unknown operation [%s]\n", line);
            printf("supported operations: {");
            for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
                printf("%s, ", ops[i]);
            }
            printf("}\n");
        }
        if (strchr(s, '[') != NULL) {
            comment = true;
        }

        // printf("[%s] [", op);
        // poly_print(x);
        // printf("] [");
        // poly_print(y);
        // printf("] [%s]\n", s);
        free(op);
        free(a);
        free(b);
        poly_free(x);
        poly_free(y);
    }
    free(line);
    fclose(f);
    poly_free(buf);

    return 0;
}
