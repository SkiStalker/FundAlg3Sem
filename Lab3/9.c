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
    if (root == NULL) return 0;

    if (*minNode == NULL || strlen(root->word) < strlen((*minNode)->word)) {
        *minNode = root;
    }
    if (*maxNode == NULL || strlen(root->word) > strlen((*maxNode)->word)) {
        *maxNode = root;
    }
    findMinMax(root->left, minNode, maxNode);
    findMinMax(root->right, minNode, maxNode);
    return 1;
}

// Функция для поиска и добавления наиболее частых слов в массив
void printTopN(Node *root, Node **arr, int *index, int maxIndex) {
    if (root == NULL) return;
    printTopN(root->right, arr, index, maxIndex);
    if (*index < maxIndex) {
        arr[*index] = root;
        (*index)++;
    }
    printTopN(root->left, arr, index, maxIndex);
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

// Функция для сохранения дерева в файл
void saveTree(Node* root, FILE* file) {
    if (root != NULL) {
        fprintf(file, "(%s %d ", root->word, root->count);
        saveTree(root->left, file);
        saveTree(root->right, file);
        fprintf(file, ") ");
    }
}

// Функция для загрузки дерева из файла
Node* loadTree(FILE* file) {
    char word[256];
    int count;

    if (fscanf(file, " ( %255s ", word) != 1) {
        return NULL; // Если не удалось прочитать узел, возвращаем NULL
    }

    if (strcmp(word, ")") == 0) {
        return NULL; // Закрывающая скобка указывает на конец узла
    }

    if (fscanf(file, "%d ", &count) != 1) {
        printf("Ошибка чтения дерева из файла.\n");
        return NULL;
    }

    Node* root = createNode(word);
    root->count = count;
    root->left = loadTree(file);
    root->right = loadTree(file);

    fscanf(file, " ) "); // Считываем закрывающую скобку
    return root;
}

// Функция для освобождения памяти дерева
void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root->word);
    free(root);
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

    // Переменные для работы с файлами
    FILE *file = NULL;
    FILE *fileRead = NULL;

    while (1) {
        printf("\nВыберите опцию:\n");
        printf("1. Узнать, сколько раз слово встречалось в файле\n");
        printf("2. Вывести n наиболее часто встречающихся слов\n");
        printf("3. Найти самое длинное и самое короткое слово\n");
        printf("4. Найти глубину дерева\n");
        printf("5. Сохранить дерево\n");
        printf("6. Загрузить дерево\n");
        printf("7. Выйти\n");
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
                printf("Введите значение n (максимум 10): ");
                scanf("%d", &n);
                if (n > 10) n = 10;
                Node *arr[10] = {NULL};
                int index = 0;
                printTopN(root, arr, &index, n);
                for (int i = 0; i < index; i++) {
                    printf("%s: %d\n", arr[i]->word, arr[i]->count);
                }
                break;
            case 3:
                if (findMinMax(root, &minNode, &maxNode) && minNode && maxNode) {
                    printf("Самое короткое слово: %s\n", minNode->word);
                    printf("Самое длинное слово: %s\n", maxNode->word);
                } else {
                    printf("Дерево пустое.\n");
                }
                break;
            case 4:
                printf("Глубина дерева: %d\n", getTreeDepth(root));
                break;
            case 5:
                file = fopen("tree.txt", "w");
                if (file) {
                    saveTree(root, file);
                    fclose(file);
                    printf("Дерево успешно сохранено.\n");
                } else {
                    printf("Не удалось открыть файл для записи.\n");
                }
                break;
            case 6:
                fileRead = fopen("tree.txt", "r");
                if (fileRead) {
                    freeTree(root);
                    root = loadTree(fileRead);
                    fclose(fileRead);
                    printf("Дерево успешно загружено.\n");
                } else {
                    printf("Не удалось открыть файл для чтения.\n");
                }
                break;
            case 7:
                printf("Выход из программы.\n");
                freeTree(root);
                return 0;
            default:
                printf("Некорректный выбор.\n");
        }
    }
    return 0;
}
