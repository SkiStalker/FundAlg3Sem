#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Node {
    char *word;
    int count;
    struct Node *left, *right;
} Node;

char *toLowerCase(char *word) {
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
    return word;
}

Node *createNode(const char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->word = strdup(word);
    newNode->count = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node *insert(Node *root, const char *word) {
    if (!root) return createNode(word);

    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        root->count++;
    } else if (cmp < 0) {
        root->left = insert(root->left, word);
    } else {
        root->right = insert(root->right, word);
    }
    return root;
}

Node *searchWord(Node *root, const char *word) {
    while (root) {
        int cmp = strcmp(word, root->word);
        if (cmp == 0) return root;
        root = (cmp < 0) ? root->left : root->right;
    }
    return NULL;
}

void saveToMassive(Node *root, Node **array, int *index) {
    if (root) {
        saveToMassive(root->left, array, index);
        array[(*index)++] = root;
        saveToMassive(root->right, array, index);
    }
}

int compareNodes(const void *a, const void *b) {
    Node *nodeA = *(Node **)a;
    Node *nodeB = *(Node **)b;
    return nodeB->count - nodeA->count;
}

int printTopNWords(Node *root, int n) {
    int index = 0;
    Node **array = (Node **)malloc(sizeof(Node *) * 1000);
    if (!array) {
        printf("Не удалось выделить память\n");
        return -1;
    }

    saveToMassive(root, array, &index);
    qsort(array, index, sizeof(Node *), compareNodes);

    printf("Топ %d слов:\n", n);
    for (int i = 0; i < n && i < index; i++) {
        printf("%s: %d\n", array[i]->word, array[i]->count);
    }
    free(array);
    return 0;
}

int processFile(const char *filename, Node **root, char *delimiters[], int delimiterCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла\n");
        return -1;
    }

    char buffer[4096], word[100];
    int wordIndex = 0;
    bool insideWord = false;

    while (fgets(buffer, sizeof(buffer), file)) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            bool isDelimiter = false;

            for (int j = 0; j < delimiterCount; j++) {
                if (buffer[i] == delimiters[j][0]) {
                    isDelimiter = true;
                    break;
                }
            }

            if (isDelimiter || isspace(buffer[i])) {
                if (insideWord) {
                    word[wordIndex] = '\0';
                    toLowerCase(word);
                    *root = insert(*root, word);
                    wordIndex = 0;
                    insideWord = false;
                }
            } else {
                word[wordIndex++] = buffer[i];
                insideWord = true;
            }
        }

        if (insideWord) {
            word[wordIndex] = '\0';
            toLowerCase(word);
            *root = insert(*root, word);
        }
    }

    fclose(file);
    return 0;
}

int findExtremes(Node *root, Node **minWord, Node **maxWord) {
    if (!root) return -1;

    *minWord = *maxWord = root;

    while (root) {
        if (strlen(root->word) < strlen((*minWord)->word)) *minWord = root;
        if (strlen(root->word) > strlen((*maxWord)->word)) *maxWord = root;

        root = root->right;
    }
    return 0;
}

int printAllWords(Node *root) {
    if (!root) return -1;
    printAllWords(root->left);
    printf("%s: %d\n", root->word, root->count);
    printAllWords(root->right);
    return 0;
}

int getTreeDepth(Node *root) {
    if (!root) return 0;

    int leftDepth = getTreeDepth(root->left);
    int rightDepth = getTreeDepth(root->right);

    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

int saveTreeToFile(Node *root, FILE *file) {
    if (!root) {
        fprintf(file, "null\n");
        return -1;
    }

    fprintf(file, "%s:%d\n", root->word, root->count);
    saveTreeToFile(root->left, file);
    saveTreeToFile(root->right, file);
    return 0;
}

Node *restoreTreeFromFile(FILE *file) {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    if (strcmp(buffer, "null\n") == 0) return NULL;

    char word[100];
    int count;
    sscanf(buffer, "%[^:]:%d", word, &count);

    Node *node = createNode(word);
    node->count = count;
    node->left = restoreTreeFromFile(file);
    node->right = restoreTreeFromFile(file);
    return node;
}

int freeTree(Node *root) {
    if (!root) return -1;
    
    freeTree(root->left);
    freeTree(root->right);

    free(root->word);      // Освобождаем память для строки
    free(root);            // Освобождаем память для узла
    return 0;
}

// Главная функция
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <имя файла> <делимитер1> [<делимитер2> ...]\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int delimiterCount = argc - 2;
    char **delimiters = argv + 2;
    Node *root = NULL;

    if (processFile(filename, &root, delimiters, delimiterCount) != 0) return 1;

    printf("Слова с частотой:\n");
    printAllWords(root);
    printf("\n");

    int depth = getTreeDepth(root);
    printf("Глубина дерева: %d\n", depth);

    char choice[10];
    while (1) {
        printf("\nОпции:\n");
        printf("1. Найти частоту слова\n");
        printf("2. Показать топ N наиболее частых слов\n");
        printf("3. Найти самое короткое и самое длинное слово\n");
        printf("0. Выход\n");
        printf("Введите выбор: ");
        
        if (fgets(choice, sizeof(choice), stdin) == NULL) break;

        if (choice[0] == '1') {
            char searchTerm[100];
            printf("Введите слово для поиска: ");
            scanf("%s", searchTerm);
            getchar();

            Node *result = searchWord(root, searchTerm);
            if (result) {
                printf("'%s' встречается %d раз\n", result->word, result->count);
            } else {
                printf("'%s' не найдено\n", searchTerm);
            }
        } else if (choice[0] == '2') {
            int n;
            printf("Введите количество топ слов для отображения: ");
            scanf("%d", &n);
            getchar();
            printTopNWords(root, n);
        } else if (choice[0] == '3') {
            Node *minWord = NULL, *maxWord = NULL;
            findExtremes(root, &minWord, &maxWord);
            if (minWord) printf("Самое короткое слово: %s\n", minWord->word);
            if (maxWord) printf("Самое длинное слово: %s\n", maxWord->word);
        } else if (choice[0] == '0') {
            break;
        } else {
            printf("Неверный выбор\n");
        }
    }

    // Сохранение дерева в файл
    FILE *saveFile = fopen("saveTree.txt", "w");
    if (saveFile) {
        saveTreeToFile(root, saveFile);
        fclose(saveFile);
        printf("Дерево успешно сохранено в файл saveTree.txt\n");
    } else {
        printf("Ошибка открытия файла для сохранения\n");
        return 1;
    }

    // Восстановление дерева из файла
    FILE *restoreFile = fopen("saveTree.txt", "r");
    if (restoreFile) {
        Node *restoredRoot = restoreTreeFromFile(restoreFile);
        fclose(restoreFile);
        printf("Дерево успешно восстановлено из файла saveTree.txt\n");

        printf("Слова восстановленного дерева:\n");
        printAllWords(restoredRoot);

        freeTree(restoredRoot);
    } else {
        printf("Ошибка открытия файла для восстановления\n");
    }

    freeTree(root);
    return 0;
}
