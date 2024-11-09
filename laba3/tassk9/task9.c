#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    char *word;
    int count;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct WordCount {
    char *word;
    int count;
} WordCount;

TreeNode* createNode(const char *word) {
    TreeNode *newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (!newNode) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }
    newNode->word = strdup(word);
    if (!newNode->word) {
        fprintf(stderr, "Ошибка выделения памяти для слова\n");
        exit(EXIT_FAILURE);
    }
    newNode->count = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Функция вставки слова в дерево
TreeNode* insert(TreeNode *root, const char *word) {
    if (root == NULL) {
        return createNode(word);
    }
    int cmp = strcmp(word, root->word);
    if (cmp < 0) {
        root->left = insert(root->left, word);
    }
    else if (cmp > 0) {
        root->right = insert(root->right, word);
    }
    else {
        root->count++;
    }
    return root;
}

// Функция поиска слова в дереве
TreeNode* search(TreeNode *root, const char *word) {
    if (root == NULL) return NULL;
    int cmp = strcmp(word, root->word);
    if (cmp < 0) return search(root->left, word);
    else if (cmp > 0) return search(root->right, word);
    else return root;
}

// Функция для нахождения глубины дерева
int treeDepth(TreeNode *root) {
    if (root == NULL) return 0;
    int leftDepth = treeDepth(root->left);
    int rightDepth = treeDepth(root->right);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

// Функция для сбора всех слов из дерева
void collectWords(TreeNode *root, WordCount **array, int *size, int *capacity) {
    if (root == NULL) return;
    collectWords(root->left, array, size, capacity);
    if (*size >= *capacity) {
        *capacity *= 2;
        *array = realloc(*array, (*capacity) * sizeof(WordCount));
        if (!*array) {
            fprintf(stderr, "Ошибка выделения памяти при сборе слов\n");
            exit(EXIT_FAILURE);
        }
    }
    (*array)[*size].word = root->word;
    (*array)[*size].count = root->count;
    (*size)++;
    collectWords(root->right, array, size, capacity);
}

int compareCounts(const void *a, const void *b) {
    WordCount *wc1 = (WordCount*)a;
    WordCount *wc2 = (WordCount*)b;
    return wc2->count - wc1->count;
}

// Функция для нахождения самого длинного и самого короткого слова
void findLongestShortest(TreeNode *root, char **longest, char **shortest) {
    if (root == NULL) return;
    findLongestShortest(root->left, longest, shortest);
    if (*longest == NULL || strlen(root->word) > strlen(*longest)) {
        *longest = root->word;
    }
    if (*shortest == NULL || strlen(root->word) < strlen(*shortest)) {
        *shortest = root->word;
    }
    findLongestShortest(root->right, longest, shortest);
}


// Сохранение дерева в файл
void saveTree(TreeNode *root, FILE *fp) {
    if (root == NULL) {
        fprintf(fp, "#\n"); 
        return;
    }
    fprintf(fp, "%s %d\n", root->word, root->count);
    saveTree(root->left, fp);
    saveTree(root->right, fp);
}

// Восстановление дерева из файла
TreeNode* loadTree(FILE *fp) {
    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), fp)) return NULL;
    buffer[strcspn(buffer, "\n")] = 0;
    if (strcmp(buffer, "#") == 0) return NULL;
    char *space = strchr(buffer, ' ');
    if (!space) {
        fprintf(stderr, "Некорректный формат файла при загрузке дерева.\n");
        exit(EXIT_FAILURE);
    }
    *space = '\0';
    char *word = buffer;
    int count = atoi(space + 1);
    TreeNode *node = createNode(word);
    node->count = count;
    node->left = loadTree(fp);
    node->right = loadTree(fp);
    return node;
}

void freeTree(TreeNode *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->word);
    free(root);
}

// Функция разбивки строки на слова по заданным разделителям
char* getNextWord(char *str, char *delimiters) {
    if (str == NULL) return NULL;
    char *start = str;
    while (*start && strchr(delimiters, *start)) start++;
    if (*start == '\0') return NULL;
    char *end = start;
    while (*end && !strchr(delimiters, *end)) end++;
    int len = end - start;
    char *word = (char*)malloc(len + 1);
    if (!word) {
        fprintf(stderr, "Ошибка выделения памяти для слова\n");
        exit(EXIT_FAILURE);
    }
    strncpy(word, start, len);
    word[len] = '\0';
    return word;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <путь_к_файлу> <разделитель1> [<разделитель2> ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char delimiters[256] = {0};
    for (int i = 2; i < argc; i++) {
        strcat(delimiters, argv[i]);
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }

    TreeNode *root = NULL;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        char *ptr = buffer;
        char *word;
        while ((word = getNextWord(ptr, delimiters)) != NULL) {
            root = insert(root, word);
            ptr += strlen(word);
            while (*ptr && strchr(delimiters, *ptr)) ptr++;
            free(word);
        }
    }
    fclose(fp);

    int choice;
    do {
        printf("\nМеню:\n");
        printf("1. Узнать количество вхождений слова\n");
        printf("2. Вывести первые N наиболее часто встречающихся слов\n");
        printf("3. Найти самое длинное и самое короткое слово\n");
        printf("4. Найти глубину дерева\n");
        printf("5. Сохранить дерево в файл\n");
        printf("6. Загрузить дерево из файла\n");
        printf("0. Выход\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);
        getchar(); 

        if (choice == 1) {
            char query[256];
            printf("Введите слово для поиска: ");
            fgets(query, sizeof(query), stdin);
            query[strcspn(query, "\n")] = 0;
            TreeNode *result = search(root, query);
            if (result)
                printf("Слово \"%s\" встречается %d раз.\n", query, result->count);
            else
                printf("Слово \"%s\" не найдено.\n", query);
        }
        else if (choice == 2) {
            int n;
            printf("Введите значение N: ");
            scanf("%d", &n);
            getchar();
            int size = 0, capacity = 100;
            WordCount *words = malloc(capacity * sizeof(WordCount));
            if (!words) {
                fprintf(stderr, "Ошибка выделения памяти для списка слов\n");
                exit(EXIT_FAILURE);
            }
            collectWords(root, &words, &size, &capacity);
            qsort(words, size, sizeof(WordCount), compareCounts);
            printf("Топ %d слов:\n", n);
            for (int i = 0; i < n && i < size; i++) {
                printf("%s: %d\n", words[i].word, words[i].count);
            }
            free(words);
        }
        else if (choice == 3) {
            char *longest = NULL, *shortest = NULL;
            findLongestShortest(root, &longest, &shortest);
            if (longest) printf("Самое длинное слово: %s\n", longest);
            if (shortest) printf("Самое короткое слово: %s\n", shortest);
            if (!longest && !shortest)
                printf("Дерево пусто.\n");
        }
        else if (choice == 4) {
            int depth = treeDepth(root);
            printf("Глубина дерева: %d\n", depth);
        }
        else if (choice == 5) {
            char filename[256];
            printf("Введите имя файла для сохранения: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            FILE *save_fp = fopen(filename, "w");
            if (!save_fp) {
                perror("Ошибка открытия файла для сохранения");
                continue;
            }
            saveTree(root, save_fp);
            fclose(save_fp);
            printf("Дерево успешно сохранено в файл \"%s\".\n", filename);
        }
        else if (choice == 6) {
            char filename[256];
            printf("Введите имя файла для загрузки: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            FILE *load_fp = fopen(filename, "r");
            if (!load_fp) {
                perror("Ошибка открытия файла для загрузки");
                continue;
            }
            freeTree(root);
            root = loadTree(load_fp);
            fclose(load_fp);
            printf("Дерево успешно загружено из файла \"%s\".\n", filename);
        }
        else if (choice == 0) {
            printf("Выход из программы.\n");
        }
        else {
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 0);

    freeTree(root);
    return EXIT_SUCCESS;
}
