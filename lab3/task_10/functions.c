#include "functions.h"

void init_tree(Node *root) {
    root->brother = NULL;
    root->prev = NULL;
    root->son = NULL;
}

enum return_code build_tree(FILE *input, FILE *output) {
    char c;
    Node *temp, *root;
    root = (Node *)malloc(sizeof(Node));

    if (!root)
        return MEMORY_ERROR;
    init_tree(root);
    temp = root;
    while ((c = getc(input))) {
        if (c == EOF) {
            print_tree(root, 0, output);
            break;
        }
        if (c == '\n') {
            print_tree(root, 0, output);
            fprintf(output, "\n");
            delete_tree(root);
            root = (Node *)malloc(sizeof(Node));
            if (!root)
                return MEMORY_ERROR;
            init_tree(root);
            temp = root;
        } else if (isspace(c))
            continue;
        else if (c == '(') {
            temp->son = (Node *)malloc(sizeof(Node));
            if (!temp->son) {
                delete_tree(root);
                return MEMORY_ERROR;
            }
            temp->son->prev = temp;
            temp = temp->son;
            temp->brother = NULL;
            temp->son = NULL;
        } else if (c == ')') {
            temp = temp->prev;
        } else if (c == ',') {
            temp->brother = (Node *)malloc(sizeof(Node));
            if (!temp->brother) {
                delete_tree(root);
                return MEMORY_ERROR;
            }
            temp->brother->prev = temp->prev;
            temp = temp->brother;
            temp->son = NULL;
            temp->brother = NULL;
        } else {
            temp->data = c;
        }
    }
    delete_tree(root);
    return OK;
}

void print_tree(Node *root, int level, FILE *output) {
    if (!root)
        return;
    for (int i = 0; i < level; i++) {
        fprintf(output, "  ");
    }
    fprintf(output, "%c\n", root->data);
    if (root->son)
        print_tree(root->son, level + 1, output);
    if (root->brother)
        print_tree(root->brother, level, output);
}

void delete_tree(Node *root) {
    if (root->brother)
        delete_tree(root->brother);
    if (root->son)
        delete_tree(root->son);

    free(root);
}
