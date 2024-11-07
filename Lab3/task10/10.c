#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10

typedef struct TreeNode {
    char data;
    struct TreeNode* children[MAX_NODES];
    int numChildren;
} TreeNode;

TreeNode* initializeNode(char data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->numChildren = 0;
    memset(newNode->children, 0, sizeof(newNode->children));
    return newNode;
}

void appendChild(TreeNode* parent, TreeNode* child) {
    if (parent->numChildren < MAX_NODES) {
        parent->children[parent->numChildren++] = child;
    }
}

TreeNode* buildTreeFromExpression(const char* expression) {
    TreeNode* nodeStack[100];
    int stackTop = -1; 
    TreeNode* current = NULL;
    TreeNode* rootNode = NULL;

    for (int i = 0; expression[i] != '\0'; i++) {
        char currentChar = expression[i];

        if (currentChar >= 'A' && currentChar <= 'Z') {
            TreeNode* newNode = initializeNode(currentChar);
            if (stackTop >= 0) {
                appendChild(nodeStack[stackTop], newNode);
            } else {
                rootNode = newNode;
            }
            current = newNode;
        } else if (currentChar == '(') {
            nodeStack[++stackTop] = current;
        } else if (currentChar == ')') {
            stackTop--; 
        }
    }
    return rootNode;
}

void displayTree(TreeNode* node, int depth, int lastChild[]) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) {
        if (lastChild[i])
            printf("    ");
        else
            printf("│   ");
    }

    if (depth > 0) {
        if (lastChild[depth - 1])
            printf("└── "); 
        else
            printf("├── ");
    }

    printf("%c\n", node->data); 

    for (int i = 0; i < node->numChildren; i++) {
        lastChild[depth] = (i == node->numChildren - 1);
        displayTree(node->children[i], depth + 1, lastChild);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: ./a.out <входной_файл> <выходной_файл>\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Ошибка при открытии входного файла\n");
        return 1;
    }

    FILE *outputFile = freopen(argv[2], "w", stdout);
    if (!outputFile) {
        printf("Ошибка при открытии выходного файла\n");
        fclose(inputFile);
        return 1;
    }

    char buffer[1000];
    int isLast[100];

    while (fgets(buffer, sizeof(buffer), inputFile)) {
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) continue;

        TreeNode* rootNode = buildTreeFromExpression(buffer);

        printf("Выражение: %s\n", buffer);
        displayTree(rootNode, 0, isLast);
        printf("\n");
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
