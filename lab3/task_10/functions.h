#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "stdio.h"

enum return_code {
    OK,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
};

typedef struct Node {
    char data;
    struct Node *son, *brother, *prev;
} Node;

enum return_code build_tree(FILE *input, FILE *output);
void init_tree(Node *root);
void print_tree(Node *root, int level, FILE *output);
void delete_tree(Node *root);
enum return_code validate_input(int argc, char **argv);

#endif