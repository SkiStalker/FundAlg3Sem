#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char *word;
    int count;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(char *word) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->word = strdup(word);
    node->count = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* addWord(Node *root, char *word) {
    if (root == NULL) {
        return createNode(word);
    }
    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        root->count++;
    } else if (cmp < 0) {
        root->left = addWord(root->left, word);
    } else {
        root->right = addWord(root->right, word);
    }
    return root;
}

Node* findWord(Node *root, char *word) {
    if (root == NULL) {
        return NULL;
    }
    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return findWord(root->left, word);
    } else {
        return findWord(root->right, word);
    }
}

int getTreeDepth(Node *root) {
    if (root == NULL) {
        return 0;
    }
    int leftDepth = getTreeDepth(root->left);
    int rightDepth = getTreeDepth(root->right);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

// Функция для поиска самого короткого и длинного слова
int findMinMax(Node *root, Node **minNode, Node **maxNode) {
    if (root == NULL) return -1;
    else if (*minNode == NULL || strlen(root->word) < strlen((*minNode)->word)) {
        *minNode = root;
    }
    else if (*maxNode == NULL || strlen(root->word) > strlen((*maxNode)->word)) {
        *maxNode = root;
    }
    else if (findMinMax(root->left, minNode, maxNode) == -1 || findMinMax(root->right, minNode, maxNode) == -1) {
        return -1;
    }
    return 0;
}

// Функция для поиска и добавления наиболее частых слов в массив
int printTopN(Node *root, Node **arr, int *index, int maxIndex) {
    if (root == NULL) return -1;
    if (printTopN(root->right, arr, index, maxIndex) == -1) return -1;
    if (*index < maxIndex) {
        arr[*index] = root;
        (*index)++;
    }
    if (printTopN(root->left, arr, index, maxIndex) == -1) return -1;
    return 0;
}

// Функция для считывания файла и построения дерева
Node* buildTreeFromFile(const char *filename, const char *separators) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return NULL;
    }
    Node *root = NULL;
    char word[100];
    while (fscanf(file, "%99s", word) == 1) {
        char *token = strtok(word, separators);
        while (token) {
            root = addWord(root, token);
            token = strtok(NULL, separators);
        }
    }
    fclose(file);
    return root;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <файл> <сепараторы>\n", argv[0]);
        return 1;
    }

    Node *root = buildTreeFromFile(argv[1], argv[2]);
    if (!root) return 1;

    int choice;
    char word[100];
    int n;
    Node *minNode = NULL, *maxNode = NULL;

    while (1) {
        printf("\nВыберите опцию:\n");
        printf("1. Узнать, сколько раз слово встречалось в файле\n");
        printf("2. Вывести n наиболее часто встречающихся слов\n");
        printf("3. Найти самое длинное и самое короткое слово\n");
        printf("4. Найти глубину дерева\n");
        printf("5. Выйти\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите слово: ");
                scanf("%s", word);
                Node *found = findWord(root, word);
                if (found) {
                    printf("Слово \"%s\" встречается %d раз(а).\n", word, found->count);
                } else {
                    printf("Слово \"%s\" не найдено.\n", word);
                }
                break;
            case 2:
                printf("Введите значение n: ");
                scanf("%d", &n);
                Node *arr[10] = {NULL}; // Буфер для n самых частых слов
                int index = 0;
                if (printTopN(root, arr, &index, n) == 0) {
                    for (int i = 0; i < n && arr[i]; i++) {
                        printf("%s: %d\n", arr[i]->word, arr[i]->count);
                    }
                }
                break;
            case 3:
                if (findMinMax(root, &minNode, &maxNode) == 0 && minNode && maxNode) {
                    printf("Самое короткое слово: %s\n", minNode->word);
                    printf("Самое длинное слово: %s\n", maxNode->word);
                }
                break;
            case 4:
                printf("Глубина дерева: %d\n", getTreeDepth(root));
                break;
            case 5:
                printf("Выход из программы.\n");
                return 0;
            default:
                printf("Некорректный выбор.\n");
        }
    }
    return 0;
}
