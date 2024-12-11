#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Добавлен заголовочный файл для strdup
#include <ctype.h>

#define INITIAL_HASHSIZE 128
#define MAX_LINE_LENGTH 1024

typedef struct Node {
    char* name;
    char* value;
    unsigned long long cached_decimal;
    struct Node* next;
} Node;

typedef struct {
    Node** table;
    int size;
    int min_chain;
    int max_chain;
} HashTable;

int char_to_value(char c) {
    if (isdigit(c)) {
        return c - '0';
    }
    if (isupper(c)) {
        return c - 'A' + 10;
    }
    if (islower(c)) {
        return c - 'a' + 36;
    }
    return -1;
}

unsigned long long base62_to_decimal(const char* str) {
    unsigned long long result = 0;
    while (*str) {
        int value = char_to_value(*str);
        if (value == -1) break;
        result = result * 62 + value;
        str++;
    }
    return result;
}

int hash_function(unsigned long long decimal, int size) {
    return decimal % size;
}

HashTable* create_hash_table(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) {
        fprintf(stderr, "Memory allocation failed for HashTable\n");
        exit(1);
    }
    ht->table = (Node**)calloc(size, sizeof(Node*));
    if (!ht->table) {
        fprintf(stderr, "Memory allocation failed for HashTable table\n");
        free(ht);
        exit(1);
    }
    ht->size = size;
    ht->min_chain = 0;
    ht->max_chain = 0;
    return ht;
}

void insert(HashTable* ht, char* name, char* value) {
    unsigned long long decimal = base62_to_decimal(name);
    int index = hash_function(decimal, ht->size);

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed for Node\n");
        exit(1);
    }
    new_node->name = strdup(name);  // Используем strdup
    new_node->value = strdup(value); // Используем strdup
    new_node->cached_decimal = decimal;
    new_node->next = ht->table[index];
    ht->table[index] = new_node;

    int chain_length = 0;
    Node* current = ht->table[index];
    while (current) {
        chain_length++;
        current = current->next;
    }

    if (chain_length > ht->max_chain) ht->max_chain = chain_length;
    if (ht->min_chain == 0 || chain_length < ht->min_chain) ht->min_chain = chain_length;
}

char* find(HashTable* ht, const char* name) {
    unsigned long long decimal = base62_to_decimal(name);
    int index = hash_function(decimal, ht->size);

    Node* current = ht->table[index];
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

HashTable* rebuild_hash_table(HashTable* old_ht, int new_size) {
    HashTable* new_ht = create_hash_table(new_size);

    for (int i = 0; i < old_ht->size; i++) {
        Node* current = old_ht->table[i];
        while (current) {
            int new_index = hash_function(current->cached_decimal, new_size);
            Node* new_node = (Node*)malloc(sizeof(Node));
            if (!new_node) {
                fprintf(stderr, "Memory allocation failed for Node\n");
                exit(1);
            }
            new_node->name = strdup(current->name);  // Используем strdup
            new_node->value = strdup(current->value); // Используем strdup
            new_node->cached_decimal = current->cached_decimal;
            new_node->next = new_ht->table[new_index];
            new_ht->table[new_index] = new_node;
            current = current->next;
        }
    }

    // Освобождение старой таблицы
    for (int i = 0; i < old_ht->size; i++) {
        Node* current = old_ht->table[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp->name);
            free(temp->value);
            free(temp);
        }
    }
    free(old_ht->table);
    free(old_ht);

    return new_ht;
}

void check_and_rebuild(HashTable** ht) {
    if ((*ht)->max_chain >= (*ht)->min_chain * 2) {
        int new_size = (*ht)->size * 2;
        *ht = rebuild_hash_table(*ht, new_size);
    }
}

void process_line(HashTable* ht, char* line) {
    char result[MAX_LINE_LENGTH * 2] = "";
    char word[MAX_LINE_LENGTH] = "";
    int result_pos = 0;
    int word_pos = 0;

    for (int i = 0; line[i]; i++) {
        if (isalnum(line[i])) {
            word[word_pos++] = line[i];
            word[word_pos] = '\0';
        } else {
            if (word_pos > 0) {
                char* replacement = find(ht, word);
                if (replacement) {
                    strcat(result, replacement);
                    result_pos += strlen(replacement);
                } else {
                    strcat(result, word);
                    result_pos += strlen(word);
                }
                word_pos = 0;
            }
            result[result_pos++] = line[i];
            result[result_pos] = '\0';
        }
    }

    if (word_pos > 0) {
        char* replacement = find(ht, word);
        if (replacement) {
            strcat(result, replacement);
        } else {
            strcat(result, word);
        }
    }

    printf("%s", result);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Cannot open file: %s\n", argv[1]);
        return 1;
    }

    HashTable* ht = create_hash_table(INITIAL_HASHSIZE);
    char line[MAX_LINE_LENGTH];
    int processing_defines = 1;

    while (fgets(line, sizeof(line), file)) {
        if (processing_defines && strncmp(line, "#define ", 8) == 0) {
            char name[MAX_LINE_LENGTH] = "";
            char value[MAX_LINE_LENGTH] = "";
            char* ptr = line + 8;
            while (*ptr && isspace(*ptr)) ptr++;
            int i = 0;
            while (*ptr && !isspace(*ptr)) {
                name[i++] = *ptr++;
            }
            name[i] = '\0';

            while (*ptr && isspace(*ptr)) ptr++;
            i = 0;
            while (*ptr && *ptr != '\n' && *ptr != '\r') {
                value[i++] = *ptr++;
            }
            value[i] = '\0';

            if (strlen(name) > 0 && strlen(value) > 0) {
                insert(ht, name, value);
                check_and_rebuild(&ht);
            }
        } else {
            processing_defines = 0;
            if (!processing_defines) {
                process_line(ht, line);
            }
        }
    }

    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->table[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp->name);
            free(temp->value);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
    fclose(file);

    return 0;
}