#include <ctype.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum Status {
    S_OK,
    S_MALLOC_ERROR,
    S_PARSE_ERROR,
} Status;

typedef struct Tree Tree;
struct Tree {
    char *val;
    Tree *up;
    Tree *down;
    Tree *side;
};

int tree_free(Tree *t) {
    if (!t)
        return 0;
    tree_free(t->side);
    tree_free(t->down);
    if (t->val)
        free(t->val);
    free(t);
    return 0;
}

int tree_set_v(Tree *t, const char *s) {
    int n = strlen(s);
    t->val = malloc(n + 1);
    if (!t->val) {
        return S_MALLOC_ERROR;
    }
    strcpy(t->val, s);
    return S_OK;
}

int tree_init(Tree **t) {
    *t = malloc(sizeof(Tree));
    if (*t == NULL)
        return S_MALLOC_ERROR;
    memset(*t, 0, sizeof(Tree));
    return S_OK;
}

Tree *tree_add_side(Tree *t, const char *s) {
    if (*s == 0)
        return t;
    if (t->val == NULL) {
        if (tree_set_v(t, s))
            return NULL;
        return t;
    }

    Tree **tar = &t->side;
    while (*tar != NULL)
        tar = &(*tar)->side;
    if (tree_init(tar) || tree_set_v(*tar, s))
        return NULL;
    (*tar)->up = t->up;
    return *tar;
}

int tree_parse(Tree *t, const char *str_const) {
    char *str = malloc(strlen(str_const) + 1);
    if (!str)
        return S_MALLOC_ERROR;
    strcpy(str, str_const);
    Tree *cur = t;
    char *s = str;
    while (isspace(*s))
        s++;
    char *end = s;
    while (*end) {
        if (*end != '(' && *end != ')' && *end != ',') {
            end++;
            continue;
        }
        char c = *end;
        *end = 0;
        char *back = end - 1;
        while (isspace(*back) && back > s) {
            *back = 0;
            back--;
        }
        cur = tree_add_side(cur, s);
        if (cur == NULL) {
            free(str);
            return S_MALLOC_ERROR;
        }
        if (c == '(') {
            if (tree_init(&cur->down)) {
                free(str);
                return S_MALLOC_ERROR;
            }
            cur->down->up = cur;
            cur = cur->down;
        } else if (c == ')') {
            cur = cur->up;
        }
        end++;
        while (isspace(*end))
            end++;
        s = end;
    }
    if (*s && tree_add_side(cur, s) == NULL) {
        free(str);
        return S_MALLOC_ERROR;
    }
    free(str);
    return S_OK;
}

int tree_print(FILE *f, const Tree *t, int offset) {
    if (!t)
        return S_OK;
    for (int i = 0; i < offset; i++)
        fprintf(f, "  ");
    fprintf(f, "\"%s\"\n", t->val);
    tree_print(f, t->down, offset + 1);
    tree_print(f, t->side, offset);
    return S_OK;
}

int main(int argc, const char *argw[]) {
    if (argc <= 1) {
        printf("ERROR: no input file provided\n");
        return 1;
    }
    if (argc <= 2) {
        printf("ERROR: no output file provided\n");
        return 1;
    }
    if (strcmp(argw[1], argw[2]) == 0) {
        printf("ERROR: input and output files are the same\n");
        return 1;
    }
    FILE *in = fopen(argw[1], "r");
    if (!in) {
        printf("ERROR: failed to open file [%s]\n", argw[1]);
        return 1;
    }
    FILE *out = fopen(argw[2], "w");
    if (!out) {
        printf("ERROR: failed to open file [%s]\n", argw[2]);
        return 1;
    }

    char *line = NULL;
    size_t line_len = 0;
    while (true) {
        int n = getline(&line, &line_len, in);
        if (n <= 1)
            break;
        n--;
        line[n] = 0;

        const char *s = line;
        Tree *t = NULL;
        tree_init(&t);
        if (tree_parse(t, s) != S_OK) {
            printf("ERROR: buy more ram\n");
            return 1;
        }
        tree_print(out, t, 0);
        fprintf(out, "\n");
        tree_free(t);
    }

    free(line);
    fclose(in);
    fclose(out);
    return 0;
}
