#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum StatusCode {
    SUCCESS = 0,
    MEMORY_ALLOCATION_ERROR,
    INVALID_NODE,
    INVALID_EXPRESSION,
    FILE_OPEN_ERROR,
    FILE_WRITE_ERROR,
    INVALID_ARGUMENTS
} StatusCode;

typedef struct Node {
    char value;
    struct Node** children;
    int childCount;
} Node;

StatusCode createNode(char value, Node** newNode) {
    *newNode = (Node*)malloc(sizeof(Node));
    if (!(*newNode)) {
        return MEMORY_ALLOCATION_ERROR;
    }
    (*newNode)->value = value;
    (*newNode)->children = NULL;
    (*newNode)->childCount = 0;
    return SUCCESS;
}

StatusCode addChild(Node* parent, Node* child) {
    if (!parent || !child) {
        return INVALID_NODE;
    }
    Node** temp = realloc(parent->children, (parent->childCount + 1) * sizeof(Node*));
    if (!temp) {
        return MEMORY_ALLOCATION_ERROR;
    }
    parent->children = temp;
    parent->children[parent->childCount] = child;
    parent->childCount++;
    return SUCCESS;
}

StatusCode freeTree(Node* root) {
    if (root != NULL) {
        for (int i = 0; i < root->childCount; i++) {
            freeTree(root->children[i]);
        }
        free(root->children);
        free(root);
    }
    return SUCCESS;
}

StatusCode buildTree(const char* expression, int* index, Node** node) {
    while (isspace(expression[*index])) {
        (*index)++;
    }

    if (expression[*index] == '\0') {
        return INVALID_EXPRESSION;
    }

    char current = expression[*index];
    StatusCode status = createNode(current, node);
    if (status != SUCCESS) {
        return status;
    }
    (*index)++;

    while (isspace(expression[*index])) {
        (*index)++;
    }

    if (expression[*index] == '(') {
        (*index)++;
        while (1) {
            Node* child = NULL;
            status = buildTree(expression, index, &child);
            if (status == SUCCESS) {
                status = addChild(*node, child);
                if (status != SUCCESS) {
                    freeTree(*node);
                    return status;
                }
            } else if (status == INVALID_EXPRESSION) {
                break;
            }

            while (isspace(expression[*index])) {
                (*index)++;
            }

            if (expression[*index] == ',') {
                (*index)++;
                continue;
            } else if (expression[*index] == ')') {
                (*index)++;
                break;
            }
        }
    }

    return SUCCESS;
}

StatusCode printTree(const Node* root, FILE* file, int depth) {
    if (root == NULL) {
        return INVALID_NODE; 
    }

    if (fprintf(file, "%*s%c\n", (depth * 4), "", root->value) < 0) {
        return FILE_WRITE_ERROR;
    }

    for (int i = 0; i < root->childCount; i++) {
        StatusCode status = printTree(root->children[i], file, depth + 1);
        if (status != SUCCESS) {
            return status;
        }
    }
    return SUCCESS;
}

StatusCode validateExpression(const char* expression) {
    int balance = 0;
    for (int i = 0; expression[i] != '\0'; i++) {
        if (expression[i] == '(') {
            balance++;
        } else if (expression[i] == ')') {
            balance--;
        }
        if (balance < 0) {
            return INVALID_EXPRESSION;
        }
    }
    return (balance == 0) ? SUCCESS : INVALID_EXPRESSION;
}

StatusCode processFile(const char* inputFilePath, const char* outputFilePath) {
    FILE* inputFile = fopen(inputFilePath, "r");
    if (!inputFile) {
        return FILE_OPEN_ERROR; 
    }

    FILE* outputFile = fopen(outputFilePath, "w");
    if (!outputFile) {
        fclose(inputFile);
        return FILE_OPEN_ERROR; 
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        int index = 0;

        line[strcspn(line, "\n")] = 0;

        StatusCode status = validateExpression(line);
        if (status != SUCCESS) {
            fprintf(stderr, "Invalid expression: %s\n", line);
            continue;
        }

        Node* root = NULL;
        status = buildTree(line, &index, &root);
        if (status == SUCCESS) {
            fprintf(outputFile, "Tree for expression '%s':\n", line);
            status = printTree(root, outputFile, 0);
            if (status != SUCCESS) {
                fprintf(stderr, "Error printing tree for expression: %s\n", line);
            }
            fprintf(outputFile, "\n");
            freeTree(root);
        } else {
            fprintf(stderr, "Error building tree for expression: %s\n", line);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    return SUCCESS;
}

StatusCode validateCommandLineArgs(int argc, const char* argv[]) {
    (void)argv;
    if (argc != 3) {
        return INVALID_ARGUMENTS;
    }
    return SUCCESS;
}

int main(int argc, const char* argv[]) {
    StatusCode status = validateCommandLineArgs(argc, argv);
    if (status != SUCCESS) {
        fprintf(stderr, "Usage: %s <input_file_path> <output_file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    status = processFile(argv[1], argv[2]);
    if (status != SUCCESS) {
        switch (status) {
            case FILE_OPEN_ERROR:
                fprintf(stderr, "Error opening file.\n");
                break;
            case MEMORY_ALLOCATION_ERROR:
                fprintf(stderr, "Memory allocation error.\n");
                break;
            case INVALID_ARGUMENTS:
                fprintf(stderr, "Invalid command line arguments.\n");
                break;
            case INVALID_EXPRESSION:
                fprintf(stderr, "Invalid expression encountered during processing.\n");
                break;
            case FILE_WRITE_ERROR:
                fprintf(stderr, "Error writing to output file.\n");
                break;
            default:
                fprintf(stderr, "Unknown error occurred.\n");
                break;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS; 
}
