#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

typedef enum return_code {
    OK,
    INVALID_INPUT,
    MEMORY_ALLOCATION_ERROR,
    OVERFLOW_ERROR,
    DEREFERENCING_NULL_POINTER_ATTEMPT,
    FILE_PATH_LENGTH_ERROR,
    FILE_OPENNING_ERROR
} return_code;

typedef struct Node {
    char data;
    struct Node* next_sibling;
    struct Node* first_child;
    struct Node* parent;
} Node;


Node* create_node(char data);
void initialize_tree(Node* root_node);
void add_child(Node* parent, Node* child);
return_code construct_tree(FILE* input_file, FILE* output_file);
void print_tree(Node* node, int depth, FILE* output_file);
void delete_tree(Node* node);

