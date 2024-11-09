#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef int (*for_check_symbol)(int);

typedef struct Polynomial {
    double *next;
    int num;
} Polynomial;

int is_separator(int c) {
    return isspace(c) || c == '+' || c == '-' || c == ',' || c == ')' || c == '\0';
}

int func_for_string_parsing(char **result, const char **start, for_check_symbol m) {
    const char *end = *start;
    while (!m(*end) && (&end != NULL)) {
        end++;
    }
    int count_symbols = end - *start;
    *result = malloc(count_symbols + 1);
    if (!result) {
        return -1;
    }
        
    memcpy(*result, *start, count_symbols);
    (*result)[count_symbols] = 0;
    *start = end + 1;
    return 0;
}

/*
    void* memset(void *s, int c, size_t n) {
        char* p = (char *)s
        while ((int)n-- > 0) *p++ = c;
        return s;
    }
*/

int function_for_polynomial_parsing(Polynomial *result, const char *str) {
    result->num = strchr(str, 'x') != 0;
    const char *s = str;
    while (1) {
        char *end;
        s = strchr(s, '^');
        if (!s) {
            break;
        }

        s++;
        int pow = strtoul(s, &end, 10);
        if (*end != 0 && !is_separator(*end)) {
            int str_len = end - s;
            printf("I cant parse %*s as number\n", str_len, s);
            return -1;
        }
        if (pow > result->num)
            result->num = pow;
    }

    result->next = malloc((result->num + 1) * sizeof(double));
    if (!result->next)
        return -1;
    memset(result->next, 0, (result->num + 1) * sizeof(double));

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
            printf("I cant parse %*s as number\n", str_len, s);
            return -1;
        }
        if (s == end)
            coef = 1;
        if (neg)
            coef *= -1;

        s = end;
        if (*s != 'x') { // a
            result->next[0] = coef;
            continue;
        }
        s++;
        if (*s != '^') { // a * x
            result->next[1] = coef;
            continue;
        }
        s++;
        int pow = strtoul(s, &end, 10);
        result->next[pow] = coef;
        s = end;
    }

    return 0;
}

int function_for_print_polynomial(Polynomial a) {
    if (a.num == 0) {
        printf("%f", a.next[0]);
        return 0;
    }
    for (int n = a.num; n >= 0; n--) {
        if (a.next[n] == 0)
            continue;
        char op = a.next[n] < 0 ? '-' : '+';
        char *x;
        if (n == 0)
            x = "";
        else if (n == 1)
            x = "x";
        else
            x = "x^%d";
        if (n == a.num)
            printf("%.3f", a.next[n]);
        else
            printf("%c %.3f", op, fabs(a.next[n]));
        printf(x, n);
        if (n > 0)
            printf(" ");
    }
    return 0;
}

#define FOR_SWAP_POLYNOM(a, b)                                                        \
    {                                                                          \
        if (a.num < b.num) {                                                       \
            const Polynomial _temp_val_ = a;                                         \
            a = b;                                                             \
            b = _temp_val_;                                                    \
        }                                                                      \
    }

int create_polynomial(Polynomial *polynom, int n) {
    polynom->num = n;
    polynom->next = malloc((polynom->num + 1) * sizeof(double));
    if (polynom->next == NULL)
        return -1;
    memset(polynom->next, 0, (polynom->num + 1) * sizeof(double));
    return 0;
}

int function_for_double_mult(Polynomial *res, Polynomial a, double mul) {
    create_polynomial(res, a.num);
    for (int i = 0; i <= res->num; i++)
        res->next[i] = a.next[i] * mul;
    return 0;
}

int function_add_for_polynomial(Polynomial *res, Polynomial a, Polynomial b) {
    int r, i;
    FOR_SWAP_POLYNOM(a, b);
    if (r = create_polynomial(res, a.num)) {
        return r;
    }
    for (i = 0; i <= b.num; i++)
        res->next[i] = a.next[i] + b.next[i];
    for (i; i <= a.num; i++)
        res->next[i] = a.next[i];
    while (res->num > 0 && res->next[res->num] == 0)
        res->num--;
    return 0;
}

int delete_polynomial(Polynomial a) {
    free(a.next);
    return 0;
}

int function_sub_for_polynomial(Polynomial *res, Polynomial a, Polynomial b) {
    Polynomial neg;
    int r;
    if ((r = function_for_double_mult(&neg, b, -1)))
        return r;
    r = function_add_for_polynomial(res, a, neg);
    delete_polynomial(neg);
    return r;
}

int function_mult_for_polynomial(Polynomial *res, Polynomial a, Polynomial b) {
    FOR_SWAP_POLYNOM(a, b);
    int r;
    if ((r = create_polynomial(res, a.num + b.num)))
        return r;
    for (int i = 0; i <= a.num; i++) {
        for (int j = 0; j <= b.num; j++) {
            res->next[i + j] += a.next[i] * b.next[j];
        }
    }
    return 0;
}



int function_pow_for_polynomial(Polynomial *result_polynomial, Polynomial original, unsigned e) {
    Polynomial result, base;
    if (create_polynomial(&result, 0) || function_for_double_mult(&base, original, 1))
        return -1;
    result.next[0] = 1;
    while (e != 0) {
        if (e & 1) {
            Polynomial new_r;
            if (function_mult_for_polynomial(&new_r, result, base)) {
                delete_polynomial(result);
                delete_polynomial(base);
                return -1;
            }
            delete_polynomial(result);
            result = new_r;
        }
        e >>= 1;
        Polynomial new_base;
        if (function_mult_for_polynomial(&new_base, base, base)) {
            delete_polynomial(base);
            delete_polynomial(result);
            return -1;
        }
        delete_polynomial(base);
        base = new_base;
    }
    delete_polynomial(base);
    *result_polynomial = result;
    return 0;
}

int poly_diff_for_polynomial(Polynomial *res, Polynomial a) {
    if (create_polynomial(res, a.num - 1))
        return -1;
    for (int i = 1; i <= a.num; i++) {
        res->next[i - 1] = i * a.next[i];
    }
    return 0;
}
int function_for_eval(double *res, Polynomial a, double x) {
    *res = 0;
    double xp = 1;
    for (int i = 0; i <= a.num; i++) {
        *res += xp * a.next[i];
        xp *= x;
    }
    return 0;
}

int function_for_div(Polynomial *div, Polynomial *mod, Polynomial a, Polynomial b) {
    if (b.num > a.num) {
        if (create_polynomial(div, 0))
        printf("I can't create polynomial(\n");
            return -1;
        if (function_for_double_mult(mod, a, 1)) {
            delete_polynomial(*div);
            printf("I can`t mult\n");
            return -1;
        }
        return 0;
    }

    Polynomial r_mod;
    Polynomial r_div;
    if (create_polynomial(&r_div, 0))
        return -1;
    if (function_for_double_mult(&r_mod, a, 1)) {
        delete_polynomial(r_div);
        return -1;
    };
    while (r_mod.num >= b.num) {
        double power = r_mod.num - b.num;
        Polynomial r_cur;
        if (create_polynomial(&r_cur, power)) {
            delete_polynomial(r_div);
            delete_polynomial(r_mod);
            return -1;
        };
        r_cur.next[r_cur.num] = r_mod.next[r_mod.num] / b.next[b.num];

        Polynomial r_add;
        int r = function_add_for_polynomial(&r_add, r_div, r_cur);
        delete_polynomial(r_div);
        r_div = r_add;
        if (r) {
            delete_polynomial(r_cur);
            delete_polynomial(r_mod);
            delete_polynomial(r_div);
            return r;
        }

        Polynomial r_mul;
        r = function_mult_for_polynomial(&r_mul, r_cur, b);
        delete_polynomial(r_cur);
        if (r) {
            delete_polynomial(r_mod);
            delete_polynomial(r_div);
            return r;
        }

        Polynomial r_sub;
        r = function_sub_for_polynomial(&r_sub, r_mod, r_mul);
        delete_polynomial(r_mod);
        delete_polynomial(r_mul);
        r_mod = r_sub;
        if (r) {
            delete_polynomial(r_mod);
            delete_polynomial(r_div);
            return r;
        }
    }
    *mod = r_mod;
    *div = r_div;
    return 0;
}

int function_polynomial_div(Polynomial *res, Polynomial a, Polynomial b) {
    Polynomial mod;
    if (function_for_div(res, &mod, a, b))
        return -1;
    delete_polynomial(mod);
    return 0;
}
int poly_mod(Polynomial *res, Polynomial a, Polynomial b) {
    Polynomial div;
    if (function_for_div(&div, res, a, b))
        return -1;
    delete_polynomial(div);
    return 0;
}

int my_pow_for_double(double *res, double x, unsigned exp) {
    *res = 1;
    while (exp) {
        if (exp & 1)
            (*res) *= x;
        exp >>= 1;
        x *= x;
    }
    return 0;
}

int function_for_comparable(Polynomial *res, Polynomial a, Polynomial b) {
    Polynomial s;
    if (create_polynomial(&s, 0))
        return -1;
    for (int i = 0; i <= a.num; i++) {
        if (a.next[i] == 0)
            continue;
        Polynomial powed;
        if (function_pow_for_polynomial(&powed, b, i)) {
            delete_polynomial(s);
            return -1;
        }
        Polynomial sub_res;
        int r = function_for_double_mult(&sub_res, powed, a.next[i]);
        delete_polynomial(powed);
        if (r) {
            delete_polynomial(s);
            return r;
        }
        Polynomial sum;
        r = function_add_for_polynomial(&sum, s, sub_res);
        delete_polynomial(sub_res);
        delete_polynomial(s);
        s = sum;
        if (r) {
            delete_polynomial(s);
            return r;
        }
    }
    *res = s;
    return 0;
}


int check_if_skobka(int c) {
    return c == '(';
}


typedef int (*function)(Polynomial *r, Polynomial a, Polynomial b);

int for_mult(Polynomial *r, Polynomial a, Polynomial b) {
    delete_polynomial(*r);
    if (function_mult_for_polynomial(r, a, b))
        return -1;
    function_for_print_polynomial(*r);
    printf("\n");
    return 0;
}

int for_sub(Polynomial *r, Polynomial a, Polynomial b) {
    delete_polynomial(*r);
    int check_error = function_sub_for_polynomial(r, a, b);
    if (check_error != 0) {
        printf("I can't sub(\n");
        return -1;
    }
        
    function_for_print_polynomial(*r);
    printf("\n");
    return 0;
}


int for_add(Polynomial *r, Polynomial a, Polynomial b) {
    delete_polynomial(*r);
    int check_error = function_add_for_polynomial(r, a, b);
    if (check_error != 0) {
        printf("I can't add(\n");
        return -1;
    }

    function_for_print_polynomial(*r);
    printf("\n");
    return 0;
}

int for_div(Polynomial *r, Polynomial a, Polynomial b) {
    delete_polynomial(*r);
    if (function_polynomial_div(r, a, b))
        return -1;
    function_for_print_polynomial(*r);
    printf("\n");
    return 0;
}
int for_mod(Polynomial *r, Polynomial a, Polynomial b) {
    delete_polynomial(*r);
    if (poly_mod(r, a, b))
        return -1;
    function_for_print_polynomial(*r);
    printf("\n");
    return 0;
}
int for_eval(Polynomial *r, Polynomial a, Polynomial b) {
    (void)r;
    if (b.num > 0) {
        fprintf(stderr,
                "ERROR: expected double as second argument, got polynom\n");
        return -1;
    }
    double res;
    function_for_eval(&res, a, b.next[0]);
    printf("%f\n", res);
    return 0;
}
int for_diff(Polynomial *r, Polynomial a, Polynomial b) {
    delete_polynomial(*r);
    (void)b;
    if (poly_diff_for_polynomial(r, a))
        return 1;
    function_for_print_polynomial(*r);
    printf("\n");
    return 0;
}
int for_compare(Polynomial *r, Polynomial a, Polynomial b) {
    delete_polynomial(*r);
    if (function_for_comparable(r, a, b))
        return -1;
    function_for_print_polynomial(*r);
    printf("\n");
    return 0;
}

int check_if_skobka_or_enough_skobka(int c) {
    return c == ')' || c == ',';
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Usage : %s <input file>\n", argv[0]);
        return 1;
    }

    const char *flags[] = {"Add", "Sub",  "Mult", "Div", "Mod", "Eval", "Diff", "Compare"};
    function functions[] = {for_add, for_sub,  for_mult, for_div, 
    for_mod, for_eval, for_diff, for_compare};

    FILE *file_input = fopen(argv[1], "r");
    if (!file_input) {
        printf("I can`t open file: %s\n", argv[1]);
        return 1;
    }

    Polynomial polynom;
    int check_error = create_polynomial(&polynom, 0);
    if (check_error) {
        printf("Can`t create polynomial\n");
        fclose(file_input);
        return 1;
    }

    polynom.next[0] = 0;
    printf("Hello, in your file you can use only following commands:\n");
    for (int i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
        printf("%s ", flags[i]);
    }
    printf("\n\n");

    size_t len_of_input = 0;
    char *input = NULL;
    bool comment_in_file = false;
    while (1) {
        int n = getline(&input, &len_of_input, file_input);
        if (n <= 0) {
            break;
        }
        n--;
        input[n] = 0;

        const char *s = input;
        if (comment_in_file) {
            s = strchr(s, ']');
            if (s == NULL)
                continue;
            comment_in_file = false;
            s++;
        }

        while (isspace(*s)) {
            s++;
        }

        if (*s == 0 || *s == '%')
            continue;
        if (*s == '[') {
            comment_in_file = true;
            continue;
        }

        char *operation, *symbol1, *symbol2;
        check_error = func_for_string_parsing(&operation, &s, check_if_skobka);
        if (check_error) {
            printf("I can`t parse line: %s\n", input);
            return 1;
        }
        check_error = func_for_string_parsing(&symbol1, &s, check_if_skobka_or_enough_skobka);
        if (check_error) {
            printf("I can`t parse line: %s\n", input);
            return 1;
        }
        check_error = func_for_string_parsing(&symbol2, &s, check_if_skobka_or_enough_skobka);
        if (check_error) {
            printf("I can`t parse line: %s\n", input);
            return 1;
        }
        
        check_error = 0;
        if (*symbol2 != ';') {
            check_error++;
        }
        if (*s != ';') {
            check_error++;
        }

        if (check_error == 2) {
            printf("Line must end in ;\n");
            return 1;
        }

        Polynomial one, two;
        if (*symbol2 != ';') {
            if (function_for_polynomial_parsing(&one, symbol1) || function_for_polynomial_parsing(&two, symbol2)) {
                if (one.next) {
                    printf("I can`t parse polynomial");
                    delete_polynomial(one);
                }
                    
                return 1;
            }
        } else {
            if (function_for_polynomial_parsing(&two, symbol1) || function_for_double_mult(&one, polynom, 1)) {
                return 1;
            }
        }

        function *choose = NULL;
        for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
            if (strcmp(operation, flags[i]) == 0) {
                choose = &functions[i];
                break;
            }
        }
        if (choose != NULL) {
            int r = (*choose)(&polynom, one, two);
            if (r)
                return r;
        } else {
            printf("I don`t know operation: %s\n", operation);
        }
        if (strchr(s, '[') != NULL) {
            comment_in_file = true;
        }

        free(symbol1);
        free(symbol2);
        free(operation);
        
        delete_polynomial(two);
        delete_polynomial(one);
    }
    fclose(file_input);
    
    free(input);
    
    delete_polynomial(polynom);

    return 0;
}