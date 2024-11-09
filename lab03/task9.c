#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Node {
    char *word;
    int count;
    struct Node *left;
    struct Node  *right;

} Node;

Node *create_node(const char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) return NULL;

    newNode->word = strdup(word);
    newNode->count = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node *insert(Node *root, const char *word) {
    if (root == NULL) return create_node(word);

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

Node *find_word(Node *root, const char *word) {
    while (root != NULL) {
        int cmp = strcmp(word, root->word);
        if (cmp == 0) return root;
        else if (cmp < 0) root = root->left;
        else root = root->right;
    }
    return NULL;
}

int save_to_array(Node *root, Node **array, int *index) {
    if (root == NULL) {
        return 0;
    }
    save_to_array(root->left, array, index);
    array[*index] = root;
    (*index)++;
    save_to_array(root->right, array, index);

    return 0;
}

int compare_nodes(const void *a, const void *b) {
    Node *nodeA = *(Node **)a;
    Node *nodeB = *(Node **)b;
    return nodeB->count - nodeA->count;
}

int free_tree(Node *root) {
    if (root == NULL) {
        return 0;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root->word);
    free(root);

    return 0;
}

int print_top_n_words(Node *root, int n) {
    int index = 0;
    Node **array = (Node **)malloc(sizeof(Node *) * 1000);  // assuming max 1000 unique words
    if (!array) {
        printf("Memory allocation failed\n");
        return -1;
    }
    save_to_array(root, array, &index);

    qsort(array, index, sizeof(Node *), compare_nodes);

    printf("Top %d words:\n", n);
    for (int i = 0; i < n && i < index; i++) {
        printf("%s: %d\n", array[i]->word, array[i]->count);
    }

    free(array);

    return 0;
}

int process_file(const char *filename, Node **root, char *delimiters[], int delimiterCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла");
        return -1;
    }

    char buffer[4096];
    char word[4096];
    int word_index = 0;
    bool inside_word = false;

    while (fgets(buffer, sizeof(buffer), file)) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            bool isDelimiter = false;

            for (int j = 0; j < delimiterCount; j++) {
                if (buffer[i] == delimiters[j][0]) {
                    isDelimiter = true;
                    break;
                }
            }

            if (isDelimiter) {
                if (inside_word) {
                    word[word_index] = '\0';  
                    *root = insert(*root, word);
                    word_index = 0;
                    inside_word = false;
                }
            } else {
                if (isspace(buffer[i]) && inside_word) {
                    word[word_index++] = ' ';
                } else if (!isspace(buffer[i])) {
                    word[word_index++] = buffer[i];
                    inside_word = true;
                }
            }
        }
        
        if (inside_word) {
            word[word_index] = '\0';
            *root = insert(*root, word);
        }
    }

    fclose(file);
    return 0;
}

int save_words_to_array(Node *root, Node **array, int *index) {
    if (root == NULL) {
        return 0;
    }
    save_words_to_array(root->left, array, index);
    array[*index] = root;
    (*index)++;
    save_words_to_array(root->right, array, index);
    return 0;
}

int find_min_max_word(Node *root, Node **minWord, Node **maxWord) {
    if (root == NULL) {
        *minWord = NULL;
        *maxWord = NULL;
        return 0;
    }

    int index = 0;
    Node **array = (Node **)malloc(sizeof(Node *) * 1000); 
    if (!array) {
        printf("Ошибка выделения памяти\n");
        return 0;
    }

    save_words_to_array(root, array, &index);
    *minWord = array[0];
    *maxWord = array[0];


    for (int i = 1; i < index; i++) {
        if (strlen(array[i]->word) < strlen((*minWord)->word)) {
            *minWord = array[i];
        }
        if (strlen(array[i]->word) > strlen((*maxWord)->word)) {
            *maxWord = array[i];
        }
    }

    free(array);
    return 0;
}



int print_all_words(Node *root) {
    if (root == NULL) return 0;
    print_all_words(root->left);
    printf("%s: %d\n", root->word, root->count);
    print_all_words(root->right);

    return -1;
}

int get_tree_depth(Node *root) {
    if (root == NULL) {
        return 0;  
    }
    
    
    int leftDepth = get_tree_depth(root->left);
    int rightDepth = get_tree_depth(root->right);
    
    
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

int save_tree_to_file(Node* root, FILE* file) {
    if (root == NULL) {
        fprintf(file, "null\n");
        return 0;
    }


    fprintf(file, "%s   %d\n", root->word, root->count);
    save_tree_to_file(root->left, file);
    save_tree_to_file(root->right, file);

    return 0;
}

Node* restore_tree_from_file(FILE* file) {
    char buffer[256];
    
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return NULL; 
    }
    
    
    if (strcmp(buffer, "null\n") == 0) {
        return NULL;
    }
    
    
    char word[100];
    int count;
    sscanf(buffer, "%s %d", word, &count);
    
    
    Node* node = create_node(word);
    node->count = count;

    
    node->left = restore_tree_from_file(file);
    node->right = restore_tree_from_file(file);
    
    return node;
}



int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <delimiter1> [<delimiter2> ...]\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int delimiterCount = argc - 2;
    char **delimiters = argv + 2;
    Node *root = NULL;

    int check_error = process_file(filename, &root, delimiters, delimiterCount);
    if (check_error != 0) {
        return 1;
    }

    printf("Слова с частотой:\n");
    print_all_words(root);
    printf("\n");

    int depth = get_tree_depth(root);
    printf("Глубина дерева: %d\n", depth);

    char choice[10];
    while (1) {
        printf("\nOptions:\n");
        printf("1. Find frequency of a word\n");
        printf("2. Show top N most frequent words\n");
        printf("3. Find shortest and longest word\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            break;
        }

        if (choice[0] == '1') {
            char searchWord[100];
            printf("Enter word to search: ");
            scanf("%s", searchWord);
            getchar();

            Node *result = find_word(root, searchWord);
            if (result) {
                printf("'%s' appears %d times\n", result->word, result->count);
            } else {
                printf("'%s' not found\n", searchWord);
            }
        } else if (choice[0] == '2') {
            int n;
            printf("Enter number of top words to display: ");
            scanf("%d", &n);
            getchar();
            print_top_n_words(root, n);
        } else if (choice[0] == '3') {
            Node *minWord = NULL;
            Node *maxWord = NULL;
            find_min_max_word(root, &minWord, &maxWord);
            if (minWord) printf("Shortest word: %s\n", minWord->word);
            if (maxWord) printf("Longest word: %s\n", maxWord->word);
        } else if (choice[0] == '4') {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    printf("\n");

    FILE* saveFile = fopen("tree_data.txt", "w");
    if (saveFile) {
        save_tree_to_file(root, saveFile);
        fclose(saveFile);
        printf("Дерево успешно сохранено в файл tree_data.txt\n");
    } else {
        printf("Ошибка открытия файла для сохранения");
        return 1;
    }

    free_tree(root);

    FILE* restoreFile = fopen("tree_data.txt", "r");
    if (restoreFile) {
        Node* restoredRoot = restore_tree_from_file(restoreFile);
        fclose(restoreFile);
        printf("Дерево успешно восстановлено из файла tree_data.txt\n");

        
        printf("Слова восстановленного дерева:\n");
        print_all_words(restoredRoot);

        free_tree(restoredRoot);
    } else {
        printf("Ошибка открытия файла для восстановления");
    }

    return 0;
}
