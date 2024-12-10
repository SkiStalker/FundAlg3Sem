#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_HASH_SIZE 128
#define MAX_LINE_LENGTH 1024

typedef enum StatusCode {
    SUCCESS = 0,
    ERROR
} StatusCode;

typedef struct MacroNode {
    char *name;
    char *value;
    struct MacroNode *next;
} MacroNode;

typedef struct {
    MacroNode **table;
    size_t hash_size;
    size_t shortest_chain;
    size_t longest_chain;
} HashTable;

HashTable *create_table(size_t hash_size) {
    HashTable *table = malloc(sizeof(HashTable));
    table->hash_size = hash_size;
    table->table = calloc(hash_size, sizeof(MacroNode *));
    table->shortest_chain = 0;
    table->longest_chain = 0;
    return table;
}

StatusCode free_table(HashTable *table) {
    for (size_t i = 0; i < table->hash_size; ++i) {
        MacroNode *node = table->table[i];
        while (node) {
            MacroNode *temp = node;
            node = node->next;
            free(temp->name);
            free(temp->value);
            free(temp);
        }
    }
    free(table->table);
    free(table);
    return SUCCESS;
}

size_t hash_function(const char *key, size_t hash_size) {
    const char *base62_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    size_t hash_value = 0;
    for (const char *p = key; *p; ++p) {
        const char *pos = strchr(base62_chars, *p);
        if (pos) {
            hash_value = hash_value * 62 + (pos - base62_chars);
        }
    }
    return hash_value % hash_size;
}

StatusCode add_macro(HashTable *table, const char *name, const char *value) {
    size_t index = hash_function(name, table->hash_size);
    MacroNode *node = table->table[index];
    while (node) {
        if (strcmp(node->name, name) == 0) {
            free(node->value);
            node->value = strdup(value);
            return ERROR;
        }
        node = node->next;
    }
    MacroNode *new_node = malloc(sizeof(MacroNode));
    new_node->name = strdup(name);
    new_node->value = strdup(value);
    new_node->next = table->table[index];
    table->table[index] = new_node;
    return SUCCESS;
}

const char *get_macro(HashTable *table, const char *name) {
    size_t index = hash_function(name, table->hash_size);
    MacroNode *node = table->table[index];
    while (node) {
        if (strcmp(node->name, name) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

StatusCode replace_macros(HashTable *table, const char *input, char *output) {
    const char *start = input;
    char buffer[MAX_LINE_LENGTH];
    while (*start) {
        if (isalnum(*start) || *start == '_') {
            const char *end = start;
            while (isalnum(*end) || *end == '_') {
                ++end;
            }
            size_t length = end - start;
            strncpy(buffer, start, length);
            buffer[length] = '\0';
            const char *replacement = get_macro(table, buffer);
            if (replacement) {
                strcat(output, replacement);
            } else {
                strncat(output, start, length);
            }
            start = end;
        } else {
            strncat(output, start, 1);
            ++start;
        }
    }
    return SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file.\n");
        return EXIT_FAILURE;
    }
    HashTable *table = create_table(INITIAL_HASH_SIZE);
    char line[MAX_LINE_LENGTH];
    int in_macro_section = 1;
    char output[MAX_LINE_LENGTH * 100] = "";
    while (fgets(line, sizeof(line), file)) {
        if (in_macro_section && strncmp(line, "#define", 7) == 0) {
            char name[MAX_LINE_LENGTH], value[MAX_LINE_LENGTH];
            if (sscanf(line + 7, "%s %[^\n]", name, value) == 2) {
                add_macro(table, name, value);
            }
        } else {
            in_macro_section = 0;
            replace_macros(table, line, output);
        }
    }
    fclose(file);
    printf("%s", output);
    free_table(table);
    return EXIT_SUCCESS;
}
