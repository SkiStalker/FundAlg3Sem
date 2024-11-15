#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

Node* create_node(const char* word) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    if (!new_node) {
        return NULL;
    }

    strcpy(new_node->value, word);
    new_node->count = 1;
    new_node->left = new_node->right = NULL;

    return new_node;
}

Node* add_word(Node* root, const char* word) {
    if (root == NULL) {
        return create_node(word);
    }
    int cmp = strcmp(word, root->value);
    if (cmp == 0) {
        root->count++;
    } else if (cmp < 0) {
        root->left = add_word(root->left, word);
    } else {
        root->right = add_word(root->right, word);
    }
    return root;
}

void free_tree(Node* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int is_separator(char ch, char* separators[], int sep_count) {
    for (int i = 0; i < sep_count; i++) {
        if (ch == separators[i][0]) {
            return 1;
        }
    }
    return 0;
}

int find_word_count(Node* root, const char* word) {
    if (root == NULL) {
        return 0;
    }

    int cmp = strcmp(word, root->value);

    if (cmp == 0) {
        return root->count;
    } else if (cmp < 0) {
        return find_word_count(root->left, word);
    } else {
        return find_word_count(root->right, word);
    }
}

void find_longest_and_shortest_words(Node* root, char* longest, char* shortest) {
    if (root == NULL) {
        return;
    }

    if (strlen(root->value) > strlen(longest)) {
        strcpy(longest, root->value);
    }
    if (strlen(root->value) < strlen(shortest) || shortest[0] == '\0') {
        strcpy(shortest, root->value);
    }

    find_longest_and_shortest_words(root->left, longest, shortest);
    find_longest_and_shortest_words(root->right, longest, shortest);
}

return_code process_file(const char* filename, char* separators[], int sep_count, Node** result) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Couldn't open file\n");
        return FILE_OPENNING_ERROR;
    }

    char word[MAX_LENGTH] = {0};
    int index = 0;
    char ch;

    *result = NULL;

    while ((ch = fgetc(file)) != EOF) {
        if (is_separator(ch, separators, sep_count)) {
            if (index > 0) {
                word[index] = '\0';
                *result = add_word(*result, word);  
                index = 0;
            }
        } else if (index < MAX_LENGTH - 1 && !isspace(ch)) {
            word[index++] = ch;  
        }
    }

    if (index > 0) {
        word[index] = '\0';
        *result = add_word(*result, word);
    }

    fclose(file);
    return OK;
}

int count_nodes(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

void fill_node_array(Node* current, Node** nodes, int* index) {
    if (current == NULL) {
        return;
    }

    fill_node_array(current->left, nodes, index);
    nodes[(*index)++] = current;
    fill_node_array(current->right, nodes, index);
}

int compare_nodes(const void* a, const void* b) {
    Node* node1 = *(Node**)a;
    Node* node2 = *(Node**)b;
    return node2->count - node1->count; 
}

return_code print_top_n_words(Node* root, int n) {
    int count_of_nodes = 0;
    int index = 0;
    Node** nodes;
    int i; 
    if (!root) {
        return ACCSESS_ERROR;
    }

    count_of_nodes = count_nodes(root);

    nodes = malloc(count_of_nodes * sizeof(Node*));
    if (nodes == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    fill_node_array(root, nodes, &index);

    qsort(nodes, count_of_nodes, sizeof(Node*), compare_nodes);

    printf("first %d most common words:\n", n);
    for (i = 0; i < n && i < count_of_nodes; i++) {
        printf("%s: %d\n", nodes[i]->value, nodes[i]->count);
    }

    free(nodes);
}


int find_tree_depth(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int left_depth = find_tree_depth(root->left);
    int right_depth = find_tree_depth(root->right);
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

void save_tree(Node* root, FILE* file) {
    if (!root) {
        fprintf(file, "null");
        return;
    }

    fprintf(file, "[\"%s\", ", root->value);
    if (root->left || root->right) {
        save_tree(root->left, file);
        fprintf(file, ", ");
        save_tree(root->right, file);
    } else {
        fprintf(file, "null, null");
    }
    fprintf(file, "]");

}

return_code save_tree_to_file(Node* root, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Couldn't open file for saving\n");
        return;
    }

    save_tree(root, file);
    fclose(file);
    printf("Tree saved successfully\n");

    return OK;
}

Node* load_tree(FILE* file) {
    char ch;
    char word[MAX_LENGTH];
    int idx = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '[' || ch == ',' || ch == ' ' || ch == ']') {
            if (idx > 0) {
                word[idx] = '\0';
                Node* node = create_node(word);
                idx = 0;
                return node;
            }
            if (ch == ']') {
                return NULL; 
            }
        } else if (ch == '"') {
            while ((ch = fgetc(file)) != '"' && ch != EOF) {
                word[idx++] = ch;
            }
            word[idx] = '\0';
            Node* node = create_node(word);

            fgetc(file); 
            node->left = load_tree(file);
            fgetc(file); 
            node->right = load_tree(file);

            return node;
        }
    }
    return NULL;
}

return_code load_tree_from_file(Node** root, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Couldn't open file for loading\n");
        return;
    }

    *root = load_tree(file);
    fclose(file);
    printf("Tree loaded successfully\n");

    return OK;
}



void interactive_dialog(Node* root, char** separators) {
    int choice;
    char input[MAX_LENGTH];

    while (1) {
        printf("\nchoose:\n");
        printf("1. count of occurrences\n");
        printf("2. print the first n most common words\n");
        printf("3. find the shortest and the longest word\n");
        printf("4. get depth of tree\n");
        printf("5. save tree to file\n");
        printf("6. load tree from file\n");
        printf("7. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char input[MAX_LENGTH];
                scanf("%s", input);
                printf("count of occurrences '%s' --- %d\n", input, find_word_count(root, input));
                break;
            }

            case 2: {
                int n;
                printf("enter n:");
                scanf("%d", &n);
                print_top_n_words(root, n);
                break;
            }

            case 3: {
                char longest[MAX_LENGTH] = "";
                char shortest[MAX_LENGTH] = "";
                find_longest_and_shortest_words(root, longest, shortest);
                printf("the longest %s\n", longest);
                printf("the shortest %s\n", shortest);
                break;
            }

            case 4: {
                int depth = find_tree_depth(root);
                printf("depth: %d\n", depth);
                break;
            }

            case 5: {
                char filename[MAX_LENGTH];
                printf("Enter file name for saving: ");
                scanf("%s", filename);
                save_tree_to_file(root, filename);
                break;
            }
            case 6: {
                char filename[MAX_LENGTH];
                printf("Enter file name for loading: ");
                scanf("%s", filename);
                free_tree(root);
                root = NULL;
                load_tree_from_file(&root, filename);
                break;
            }


            case 7:
                free_tree(root);
                return;

            default:
                printf("Invalid choice.n");
                break;
        }
    }
}

