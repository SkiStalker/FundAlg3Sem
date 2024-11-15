#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 100

typedef enum return_code {
    OK,
    INVALID_INPUT,
    MEMORY_ALLOCATION_ERROR,
    OVERFLOW_ERROR,
    DEREFERENCING_NULL_POINTER_ATTEMPT,
    FILE_PATH_LENGTH_ERROR,
    FILE_OPENNING_ERROR,
    ACCSESS_ERROR
} return_code;

typedef int (*keys_comparer)(const char*, const char*);

typedef struct Node {
    int count;
    char value[100];
    struct Node* right;
    struct Node* left;
} Node;

typedef struct binary_tree {
    keys_comparer comp;
    Node* root;
} binary_tree, *p_binary_tree;

Node* create_node(const char* word);
Node* add_word(Node* root, const char* word);
void free_tree(Node* root);
return_code create_tree(p_binary_tree to_create, keys_comparer comp);

return_code print_top_n_words(Node* root, int n);
int find_tree_depth(Node* root);
return_code load_tree_from_file(Node** root, const char* filename);
return_code save_tree_to_file(Node* root, const char* filename);

int is_separator(char ch, char* separators[], int sep_count);
return_code process_file(const char* filename, char* separators[], int sep_count, Node** result);
void interactive_dialog(Node* root, char** separators);




