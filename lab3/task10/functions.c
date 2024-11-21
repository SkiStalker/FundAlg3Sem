#include "functions.h"

Node* create_node(char data)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (node) {
        node->data = data;
        node->next_sibling = NULL;
        node->first_child = NULL;
        node->parent = NULL;
    }
    return node;
}

void add_child(Node* parent, Node* child)
{
    if (parent->first_child == NULL) {
        parent->first_child = child;
    } else {
        Node* sibling = parent->first_child;
        while (sibling->next_sibling) {
            sibling = sibling->next_sibling;
        }
        sibling->next_sibling = child;
    }
    child->parent = parent;
}

return_code construct_tree(FILE* input_file, FILE* output_file)
{
    char symbol;
    Node* root = create_node('\0');
    Node* current_node = root;

    if (!root) {
        return MEMORY_ALLOCATION_ERROR;
    }

    while ((symbol = getc(input_file)) != EOF)
    {
        switch (symbol) {
            case '(': {
                Node* child = create_node('\0');
                if (!child) {
                    delete_tree(root);
                    return MEMORY_ALLOCATION_ERROR;
                }
                add_child(current_node, child);
                current_node = child;
                break;
            }
            case ')': {
                if (current_node->parent) {
                    current_node = current_node->parent;
                }
                break;
            }
            case ',': {
                Node* sibling = create_node('\0');
                if (!sibling) {
                    delete_tree(root);
                    return MEMORY_ALLOCATION_ERROR;
                }
                current_node->next_sibling = sibling;
                sibling->parent = current_node->parent;
                current_node = sibling;
                break;
            }
            case '\n': {
                print_tree(root, 0, output_file);
                fprintf(output_file, "\n");
                delete_tree(root);

                root = create_node('\0');
                if (!root) {
                    return MEMORY_ALLOCATION_ERROR;
                }
                current_node = root;
                break;
            }
            default: {
                if (isalpha(symbol)) {
                    current_node->data = symbol;
                }
                break;
            }
        }
    }

    print_tree(root, 0, output_file);
    delete_tree(root);

    return OK;
}

void print_tree(Node* node, int depth, FILE* output_file)
{
    if (!node || node->data == '\0') {
        return;
    }
    for (int i = 0; i < depth; i++) {
        fprintf(output_file, "  ");
    }
    fprintf(output_file, "%c\n", node->data);

    if (node->first_child)
        print_tree(node->first_child, depth + 1, output_file);

    if (node->next_sibling)
        print_tree(node->next_sibling, depth, output_file);
}

void delete_tree(Node* node)
{
    if (!node) return;
    if (node->next_sibling)
        delete_tree(node->next_sibling);
    if (node->first_child)
        delete_tree(node->first_child);

    free(node);
}
