#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task_09_lib.h"

typedef int (*handle)(Tree *t, const char *s);

int handle_get(Tree *t, const char *s) {
    char *word;
    parse_field_str(&word, &s, isspace);
    int r = tree_get(t, word);
    printf("%s is found %d times\n", word, r);
    free(word);
    return S_OK;
}

int handle_first(Tree *t, const char *s) {
    size_t n;
    parse_field_uint(&n, &s, isspace);
    Leaf *ls;
    int act = tree_dump_to_sorted_list(&ls, t);
    if (act < (int)n)
        n = act;
    printf("%zu most common words:\n", n);
    for (size_t i = 0; i < n; i++) {
        Leaf l = ls[i];
        printf("\"%s\": %zu\n", l.s, l.cnt);
    }
    free(ls);
    return S_OK;
}

int handle_longest(Tree *t, const char *s) {
    (void)s;
    char *str = "";
    tree_longest(&str, t);
    printf("longest word: \"%s\"\n", str);
    return S_OK;
}

int handle_shortest(Tree *t, const char *s) {
    (void)s;
    char *str = "";
    tree_shortest(&str, t);
    printf("shortest word: \"%s\"\n", str);
    return S_OK;
}
int handle_depth(Tree *t, const char *s) {
    (void)s;
    int depth = tree_depth(t);
    printf("tree depth: %d\n", depth);
    return S_OK;
}

int handle_write(Tree *t, const char *s) {
    char *fname;
    parse_field_str(&fname, &s, isspace);
    FILE *f = fopen(fname, "w");
    if (!f) {
        printf("ERROR: failed to open file [%s]\n", fname);
        free(fname);
        return 1;
    }
    free(fname);
    return tree_write(f, t);
}

int handle_read(Tree *t, const char *s) {
    char *fname;
    parse_field_str(&fname, &s, isspace);
    FILE *f = fopen(fname, "r");
    if (!f) {
        printf("ERROR: failed to open file [%s]\n", fname);
        free(fname);
        return 1;
    }
    free(fname);
    tree_free(t);
    return tree_read(f, t);
}

int main(int argc, const char *argw[]) {
    if (argc < 2) {
        printf("ERROR: no input file provided\n");
        return 1;
    }
    if (argc < 3) {
        printf("ERROR: at least one separator must be provided\n");
        return 1;
    }
    char *sep = malloc(argc - 1);
    for (int i = 2; i < argc; i++) {
        if (argw[i][0] == '\\' && strlen(argw[i]) == 2) {
            switch (argw[i][1]) {
            case 't':
                sep[i - 2] = '\t';
                break;
            case 'n':
                sep[i - 2] = '\n';
                break;
            case '\\':
                sep[i - 2] = '\\';
                break;
            case '\'':
                sep[i - 2] = '\'';
                break;
            case '"':
                sep[i - 2] = '"';
                break;
            default:
                printf("ERROR: unsupported escape sequence: [%s]\n", argw[i]);
                return 1;
            }
            continue;
        }
        if (strlen(argw[i]) > 1) {
            printf("ERROR: each separator must be a signle symbol\n");
            return 1;
        }
        sep[i - 2] = argw[i][0];
    }

    FILE *f = fopen(argw[1], "r");
    if (!f) {
        printf("ERROR: failed to open file [%s]\n", argw[1]);
        free(sep);
        return 1;
    }

    Tree t = {0};
    Status s = tree_parse_file(&t, sep, f);
    fclose(f);
    free(sep);

    const char *ops[] = {"Get",   "First", "Longest", "Shortest",
                         "Depth", "Write", "Read"};
    handle handles[] = {handle_get,      handle_first, handle_longest,
                        handle_shortest, handle_depth, handle_write,
                        handle_read};

    char *line = NULL;
    size_t line_len = 0;
    while (true) {
        int n = getline(&line, &line_len, stdin);
        if (n <= 1)
            break;
        n--;
        line[n] = 0;

        const char *s = line;
        char *op;
        parse_field_str(&op, &s, isspace);
        handle *h = NULL;
        for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]) && !h; i++)
            if (strcmp(op, ops[i]) == 0)
                h = &handles[i];

        if (!h) {
            printf("ERROR: unknown operation \"%s\"\n", line);
            printf("supported operations: {");
            for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
                printf("%s, ", ops[i]);
            }
            printf("}\n");
        } else {
            (*h)(&t, s);
        }
        free(op);
    }
    free(line);

    tree_free(&t);
    return s;
}
