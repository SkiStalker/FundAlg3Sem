#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NODES 100
#define MAX_LINE_LENGTH 1024

typedef struct TreeNode {
    char data;
    struct TreeNode* children[MAX_NODES];
    int numChildren;
} TreeNode;

// Функция для инициализации нового узла
TreeNode* initializeNode(char data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (!newNode) {
        fprintf(stderr, "Ошибка: не удалось выделить память для узла '%c'.\n", data);
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->numChildren = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Функция для добавления дочернего узла
void appendChild(TreeNode* parent, TreeNode* child) {
    if (parent->numChildren < MAX_NODES) {
        parent->children[parent->numChildren++] = child;
    } else {
        fprintf(stderr, "Ошибка: превышено максимальное количество дочерних узлов для '%c'.\n", parent->data);
        exit(EXIT_FAILURE);
    }
}

void freeTree(TreeNode* node) {
    if (node == NULL) return;
    for (int i = 0; i < node->numChildren; i++) {
        freeTree(node->children[i]);
    }
    free(node);
}

void skipCharacters(const char* expression, int* index) {
    while (expression[*index] && (isspace(expression[*index]) || expression[*index] == ',')) {
        (*index)++;
    }
}

// Функция для построения дерева
TreeNode* buildTreeFromExpression(const char* expression, int* index, TreeNode* stack[], int* stackTop) {
    skipCharacters(expression, index);

    if (expression[*index] == '\0') return NULL;

    char currentChar = expression[*index];
    if (!isalnum(currentChar)) {
        printf("Ошибка: неверный символ '%c' в выражении.\n", currentChar);
        exit(EXIT_FAILURE);
    }
    TreeNode* newNode = initializeNode(currentChar);
    (*index)++;

    skipCharacters(expression, index);
    if (expression[*index] == '(') {
        (*index)++;
        stack[++(*stackTop)] = newNode;
        while (expression[*index] != ')' && expression[*index] != '\0') {
            TreeNode* child = buildTreeFromExpression(expression, index, stack, stackTop);
            if (child) {
                appendChild(stack[*stackTop], child);
            }
            skipCharacters(expression, index);
            if (expression[*index] == ',') {
                (*index)++;
                skipCharacters(expression, index);
            }
        }
        if (expression[*index] == ')') {
            (*index)++;
            (*stackTop)--;
        } else {
            printf("Ошибка: отсутствует закрывающая скобка в выражении.\n");
            exit(EXIT_FAILURE);
        }
    }

    return newNode;
}

// Функция для отображения дерева
void displayTree(TreeNode* node, const char* prefix, int is_last, FILE* output_file) {
    if (node == NULL) return;

    fprintf(output_file, "%s", prefix);
    if (is_last) {
        fprintf(output_file, "└── ");
    } else {
        fprintf(output_file, "├── ");
    }
    fprintf(output_file, "%c\n", node->data);

    char new_prefix[MAX_LINE_LENGTH];
    strcpy(new_prefix, prefix);
    if (is_last) {
        strcat(new_prefix, "    ");
    } else {
        strcat(new_prefix, "│   ");
    }

    for (int i = 0; i < node->numChildren; i++) {
        int last_child = (i == node->numChildren - 1) ? 1 : 0;
        displayTree(node->children[i], new_prefix, last_child, output_file);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: <исполняемый файл> <входной_файл> <выходной_файл>\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Ошибка: не удалось открыть входной файл %s\n", argv[1]);
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (!outputFile) {
        printf("Ошибка: не удалось открыть выходной файл %s\n", argv[2]);
        fclose(inputFile);
        return 1;
    }

    char buffer[MAX_LINE_LENGTH];
    int line_num = 1;

    while (fgets(buffer, sizeof(buffer), inputFile)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (strlen(buffer) == 0) continue;

        int index = 0;
        TreeNode* stack[MAX_NODES];
        int stackTop = -1;
        TreeNode* rootNode = buildTreeFromExpression(buffer, &index, stack, &stackTop);

        fprintf(outputFile, "Дерево для выражения %d: %s\n", line_num, buffer);
        if (rootNode != NULL) {
            fprintf(outputFile, "%c\n", rootNode->data);
            for (int i = 0; i < rootNode->numChildren; i++) {
                int last_child = (i == rootNode->numChildren - 1) ? 1 : 0;
                displayTree(rootNode->children[i], "", last_child, outputFile);
            }
        }
        fprintf(outputFile, "\n");

        freeTree(rootNode);
        line_num++;
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

