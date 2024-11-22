#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char value;
    struct Node* children[20];  // допустим может быть до 20 детей
    int childCount;
} Node;

char* GetFileName(const char* filePath) {
    const char* fileName = strrchr(filePath, '\\');
    
    if (fileName == NULL) {
        return (char*)filePath;
    } else {
        return (char*)(fileName + 1);
    }
}

Node* CreateNode(char value) {
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        printf("malloc error\n");
        return NULL;
    }

    node->value = value;
    node->childCount = 0;
    for (int i = 0; i < 20; i++)
        node->children[i] = NULL;
    return node;
}

int ReadExpr(const char* str, int* pos, Node** root) {
    while (isspace(str[*pos])) {
        (*pos)++;
    }

    if (!isalpha(str[*pos])) {
        return -1;
    } 

    *root = CreateNode(str[*pos]);
    if (!*root) {
        return -1;
    }
    (*pos)++;

    while (isspace(str[*pos])) {
        (*pos)++;
    }

    if (str[*pos] == '(') {
        (*pos)++;
        while (1) {
            Node* child = NULL;
            if (ReadExpr(str, pos, &child) == -1) {
                return -1;
            }

            if ((*root)->childCount >= 20) {
                printf("too many children\n");
                return -1;
            }
            (*root)->children[(*root)->childCount++] = child;

            while (isspace(str[*pos])) {
                (*pos)++;
            }

            if (str[*pos] == ',') {
                (*pos)++;
                continue;
            } 
            else if (str[*pos] == ')') {
                (*pos)++;
                break;
            } else {
                return -1;
            }
        }
    }
    return 0; 
}

void PrintTree(Node* node, int depth, FILE* output) {
    if (!node) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        fprintf(output, "   ");
    }
    
    fprintf(output, "%c\n", node->value);

    for (int i = 0; i < node->childCount; i++) {
        PrintTree(node->children[i], depth + 1, output);
    }
}

void FreeTree(Node* node) {
    if (!node) {
        return;
    }

    for (int i = 0; i < node->childCount; i++) {
        FreeTree(node->children[i]);
    }
    
    free(node);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("enter file names\n");
        return 1;
    }

    char* fileName1 = GetFileName(argv[1]);
    char* fileName2 = GetFileName(argv[2]);

    if (!strcmp(fileName1, fileName2)) {
        printf("input and output files cannot be the same\n");
        return 2;
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        printf("cannot open input file\n");
        return 1;
    }

    FILE* output = fopen(argv[2], "w");
    if (!output) {
        printf("cannot open output file\n");
        return 1;
    }

    char exrp[BUFSIZ];
    
    while (fgets(exrp, sizeof(exrp), input)) {
        exrp[strcspn(exrp, "\n")] = 0; 

        Node* root = NULL;
        int pos = 0;

        if (ReadExpr(exrp, &pos, &root) == -1 || pos != (int)strlen(exrp)) {
            // по заданию все выражения корректные, но на всякий случай
            fprintf(output, "invalid expression: %s\n", exrp);
            continue;
        }

        fprintf(output, "tree for expression: %s\n", exrp);
        PrintTree(root, 0, output);
        fprintf(output, "\n");

        FreeTree(root);
    }

    fclose(input);
    fclose(output);
    
    return 0;
}