#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100

typedef struct Node {
    char name[MAX_NAME_LEN];
    struct Node** children;
    int child_count;
} Node;

Node* createNode(const char* name) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    strcpy(node->name, name);
    node->children = NULL;
    node->child_count = 0;
    return node;
}

int addChild(Node* parent, Node* child) {
    if (parent == NULL || child == NULL) {
        return -1;
    }
    parent->child_count++;
    Node** new_children = (Node**)realloc(parent->children, sizeof(Node*) * parent->child_count);
    if (new_children == NULL) {
        return -1;
    }
    parent->children = new_children;
    parent->children[parent->child_count - 1] = child;
    return 0;
}

Node* parseExpression(const char* expression, int* pos) {
    char name[MAX_NAME_LEN];
    int name_len = 0;

    while (expression[*pos] != '(' && expression[*pos] != ',' && expression[*pos] != ')' && expression[*pos] != '\0') {
        name[name_len++] = expression[*pos];
        (*pos)++;
    }
    name[name_len] = '\0';

    Node* node = createNode(name);
    if (node == NULL) {
        return NULL;
    }

    if (expression[*pos] == '(') {
        (*pos)++;
        while (expression[*pos] != ')' && expression[*pos] != '\0') {
            Node* child = parseExpression(expression, pos);
            if (child == NULL || addChild(node, child) != 0) {
                return NULL;
            }

            if (expression[*pos] == ',') {
                (*pos)++;
            }
        }
        (*pos)++;
    }

    return node;
}

int printTree(FILE* output, Node* node, int level, int is_last) {
    if (node == NULL || output == NULL) {
        return -1;
    }

    for (int i = 0; i < level - 1; i++) {
        fprintf(output, "│   ");
    }
    if (level > 0) {
        fprintf(output, is_last ? "└── " : "├── ");
    }

    fprintf(output, "%s\n", node->name);

    for (int i = 0; i < node->child_count; i++) {
        if (printTree(output, node->children[i], level + 1, i == node->child_count - 1) != 0) {
            return -1;
        }
    }
    return 0;
}

int freeTree(Node* node) {
    if (node == NULL) {
        return -1;
    }
    for (int i = 0; i < node->child_count; i++) {
        if (freeTree(node->children[i]) != 0) {
            return -1;
        }
    }
    free(node->children);
    free(node);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Использование: %s входной_файл выходной_файл\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    if (!input || !output) {
        printf("Ошибка открытия файлов.\n");
        return 1;
    }

    char expression[1000];
    if (fgets(expression, sizeof(expression), input) != NULL) {
        int pos = 0;
        Node* root = parseExpression(expression, &pos);
        if (root == NULL) {
            printf("Ошибка при построении дерева.\n");
            fclose(input);
            fclose(output);
            return 1;
        }

        if (printTree(output, root, 0, 1) != 0) {
            printf("Ошибка при выводе дерева.\n");
            fclose(input);
            fclose(output);
            freeTree(root);
            return 1;
        }
        freeTree(root);
    } else {
        printf("Ошибка чтения входного файла.\n");
    }

    fclose(input);
    fclose(output);

    return 0;
}
