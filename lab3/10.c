#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct Node {
    char value;
    struct Node** children;
    int childrenCount;
} Node;

Node* createNode(char value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        return NULL;
    }

    newNode->value = value;
    newNode->childrenCount = 0;
    newNode->children = NULL;
    return newNode;
}

void addChild(Node* parent, Node* child) {
    parent->childrenCount++;
    parent->children = (Node**)realloc(parent->children, parent->childrenCount * sizeof(Node*));
    parent->children[parent->childrenCount - 1] = child;
}

int printTree(Node* node, int level, FILE* outputFile) {
    if (!node) return -1;

    for (int i = 0; i < level; i++) {
        fprintf(outputFile, "  ");
    }
    fprintf(outputFile, "%c\n", node->value);

    for (int i = 0; i < node->childrenCount; i++) {
        printTree(node->children[i], level + 1, outputFile);
    }

    return 1;
}

Node* parseExpression(const char* expression, int* index) {
    if (expression[*index] == '\0') return NULL;

    Node* currentNode = createNode(expression[*index]);
    (*index)++;

    if (expression[*index] == '(') { // Пропускаем '('
        (*index)++;
        while (expression[*index] != ')' && expression[*index] != '\0') {
            if (expression[*index] == ',') {
                (*index)++; // Пропускаем ','
            }
            Node* child = parseExpression(expression, index);
            if (child) {
                addChild(currentNode, child);
            }
        }
        if (expression[*index] == ')') {
            (*index)++; // Пропускаем ')'
        }
    }

    return currentNode;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Использование: %s <входной файл> <выходной файл>\n", argv[0]);
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Ошибка: не удалось открыть входной файл %s\n", argv[1]);
        return 1;
    }

    FILE* outputFile = fopen(argv[2], "w");
    if (!outputFile) {
        printf("Ошибка: не удалось открыть выходной файл %s\n", argv[2]);
        fclose(inputFile);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int x = 1;
    while (fgets(line, sizeof(line), inputFile)) {
        
        // Удаляем символы новой строки
        line[strcspn(line, "\n")] = 0;

        int index = 0;
        Node* root = parseExpression(line, &index);
        fprintf(outputFile, "Дерево #%d:\n", x);
        x++;
        printTree(root, 0, outputFile);
        fprintf(outputFile, "\n");

        free(root->children);
        free(root);
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
