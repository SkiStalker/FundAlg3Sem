#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Status {
    S_OK,
    S_MALLOC_ERROR,
    S_PARSE_ERROR,
    S_INVALID_INPUT,
    S_FILE_READ_ERROR
} Status;

typedef int (*mask)(int);
size_t getword(char **res, size_t *cap, FILE *f, const char *sep);
int parse_field_str(char **res, char const **start, mask m);
int parse_field_uint(unsigned long *res, char const **start, mask m);

typedef struct Leaf {
    char *s;
    size_t cnt;
} Leaf;

typedef struct Tree Tree;
struct Tree {
    Leaf v;
    Tree *l;
    Tree *r;
};

int tree_parse_file(Tree *t, const char *sep, FILE *f);
int tree_add(Tree *t, const char *word);
int tree_get(const Tree *t, const char *word);
int tree_free(Tree *t);
int tree_print(const Tree *t);
int tree_size(const Tree *t);
int tree_dump_to_sorted_list(Leaf **r, const Tree *t);
int tree_longest(char **r, const Tree *t);
int tree_shortest(char **r, const Tree *t);
int tree_depth(const Tree *t);
int tree_write(FILE *f, const Tree *t);
int tree_read(FILE *f, Tree *t);

size_t getword(char **res, size_t *cap, FILE *f, const char *sep) {
    if (*res == NULL) {
        *cap = 32;
        *res = malloc(*cap);
        if (!*res)
            return -1;
    }
    size_t i = 0;
    bool found = false;
    while (!found) {
        char c = fgetc(f);
        if (c == 0 || c == EOF)
            break;

        for (const char *s = sep; *s && !found; s++)
            found = found || c == *s;

        if (i >= *cap - 1) {
            size_t new_cap = *cap * 2;
            char *t = realloc(*res, new_cap);
            if (!t)
                return -1;
            *cap = new_cap;
            *res = t;
        }
        (*res)[i] = c;
        i++;
    }
    (*res)[i] = 0;
    return i;
}

int parse_field_str(char **res, char const **start, mask m) {
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

int parse_field_uint(unsigned long *res, char const **start, mask m) {
    char *ptr;
    *res = strtoul(*start, &ptr, 10);
    bool valid = *ptr == 0;
    valid = valid | m(*ptr);
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        printf("ERROR: failed to parse [%s] as unsigned\n", *start);
        *ptr = tmp;
        return S_PARSE_ERROR;
    }

    *start = ptr + 1;
    return S_OK;
}

int _tree_inc_(Tree *t, const char *word, size_t i) {
    if (!t->v.s) {
        t->v.s = malloc(strlen(word) + 1);
        if (!t->v.s)
            return S_MALLOC_ERROR;
        strcpy(t->v.s, word);
    }
    t->v.cnt += i;
    return S_OK;
}

int _tree_add(Tree *t, const char *word, size_t i) {
    if (t->v.s == NULL)
        return _tree_inc_(t, word, i);

    int cmp;
    Tree **c = &t;
    do {
        cmp = strcmp(word, (*c)->v.s);
        if (cmp < 0)
            c = &(*c)->l;
        if (cmp > 0)
            c = &(*c)->r;
    } while (cmp && *c);

    if (!(*c)) {
        *c = malloc(sizeof(Tree));
        if (!*c)
            return S_MALLOC_ERROR;
        memset(*c, 0, sizeof(Tree));
    }

    return _tree_inc_(*c, word, i);
}

int tree_add(Tree *t, const char *word) {
    return _tree_add(t, word, 1);
}

int tree_get(const Tree *t, const char *word) {
    if (t->v.s == NULL)
        return 0;
    int cmp;
    do {
        cmp = strcmp(word, t->v.s);
        if (cmp < 0)
            t = t->l;
        if (cmp > 0)
            t = t->r;
    } while (cmp && t);
    if (!t)
        return 0;
    return t->v.cnt;
}

int _tree_print(const Tree *t, int level) { // can't implement non recursively
    if (!t)
        return 0;
    _tree_print(t->l, level + 1);
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("[%s]: %zu\n", t->v.s, t->v.cnt);
    _tree_print(t->r, level + 1);
    return 0;
}

int tree_print(const Tree *t) {
    return _tree_print(t, 0);
}

int _tree_free(Tree *t) { // can't implement non recursively
    if (!t || !t->v.s)
        return 0;
    _tree_free(t->l);
    _tree_free(t->r);
    if (t->l)
        free(t->l);
    if (t->r)
        free(t->r);
    if (t->v.s)
        free(t->v.s);
    return 0;
}

int tree_free(Tree *t) {
    if (!t || !t->v.s)
        return 0;
    _tree_free(t);
    t->r = NULL;
    t->l = NULL;
    t->v.s = NULL;
    t->v.cnt = 0;
    return 0;
}

int tree_parse_file(Tree *t, const char *sep, FILE *f) {
    char *word = NULL;
    size_t word_cap = 0;
    Status s = S_OK;
    while (s == S_OK) {
        int n = getword(&word, &word_cap, f, sep);
        if (n <= 0)
            break;
        if (n == 1)
            continue;
        word[n - 1] = 0;
        s = tree_add(t, word);
    }
    free(word);
    return s;
}

int tree_size(const Tree *t) {
    if (!t || !t->v.s)
        return 0;
    return tree_size(t->l) + tree_size(t->r) + 1;
}

int _push_to_list(Leaf *r, const Tree *t, size_t *i) {
    if (!t)
        return 0;
    r[*i] = t->v;
    (*i)++;
    _push_to_list(r, t->l, i);
    _push_to_list(r, t->r, i);
    return 0;
}

int leaf_cmp(const void *p_a, const void *p_b) {
    const Leaf *a = p_a;
    const Leaf *b = p_b;
    return a->cnt > b->cnt ? -1 : a->cnt != b->cnt;
}

int tree_dump_to_sorted_list(Leaf **r, const Tree *t) {
    if (!t || !t->v.s)
        return 0;
    int n = tree_size(t);
    *r = malloc(n * sizeof(Leaf));
    if (*r == NULL)
        return -1;
    size_t i = 0;
    _push_to_list(*r, t, &i);
    qsort(*r, n, sizeof(Leaf), leaf_cmp);
    return i;
}

int tree_depth(const Tree *t) {
    if (!t || !t->v.s)
        return 0;
    int left = tree_depth(t->l);
    int right = tree_depth(t->r);
    return left > right ? left + 1 : right + 1;
}

int _tree_write(FILE *f, const Tree *t) {
    if (!t || !t->v.s)
        return 0;
    fwrite(&t->v.cnt, sizeof(size_t), 1, f);
    size_t len = strlen(t->v.s);
    fwrite(&len, sizeof(size_t), 1, f);
    fwrite(t->v.s, len, 1, f);
    _tree_write(f, t->l);
    _tree_write(f, t->r);
    return 0;
}

int tree_write(FILE *f, const Tree *t) {
    if (!t)
        return 0;
    size_t n = tree_size(t);
    fwrite(&n, sizeof(size_t), 1, f);
    return _tree_write(f, t);
}

int tree_read(FILE *f, Tree *t) {
    size_t n = 0;
    if (!fread(&n, sizeof(size_t), 1, f))
        return S_FILE_READ_ERROR;
    for (size_t i = 0; i < n; i++) {
        size_t cnt, len;
        if (!fread(&cnt, sizeof(size_t), 1, f) ||
            !fread(&len, sizeof(size_t), 1, f))
            return S_FILE_READ_ERROR;

        char *s = malloc(len + 1);
        if (!s)
            return S_MALLOC_ERROR;

        if (fread(s, 1, len, f) != len)
            return S_FILE_READ_ERROR;

        s[len] = 0;
        int r = _tree_add(t, s, cnt);
        free(s);
        if (r)
            return r;
    }
    return S_OK;
}

int _tree_longest(char **r, const Tree *t, bool shortest) {
    if (!t)
        return 0;
    if ((strlen(t->v.s) > strlen(*r)) ^ shortest)
        *r = t->v.s;
    _tree_longest(r, t->l, shortest);
    _tree_longest(r, t->r, shortest);
    return S_OK;
}
int tree_longest(char **r, const Tree *t) {
    if (!t || !t->v.s)
        return S_INVALID_INPUT;
    *r = t->v.s;
    return _tree_longest(r, t, false);
}

int tree_shortest(char **r, const Tree *t) {
    if (!t || !t->v.s)
        return S_INVALID_INPUT;
    *r = t->v.s;
    return _tree_longest(r, t, true);
}
