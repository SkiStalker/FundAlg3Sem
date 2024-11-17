#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_HASHSIZE 128
#define RESIZE_FACTOR 2


typedef enum {
    ERROR_NONE,
    ERROR_INVALIDARG,
    ERROR_FILE_NOT_FOUND,
    ERROR_MEMORY_ALLOCATION,
    ERROR_SYNTAX,
    ERROR_UNKNOWN
} ErrorCode;


typedef struct MacroNode {
    char* name;
    char* value;
    struct MacroNode* next;
} MacroNode;


typedef struct {
    MacroNode** table;
    int size;
} HashTable;


HashTable* createHashTable(int size) {
    HashTable* hashTable = malloc(sizeof(HashTable));
    if (!hashTable) return NULL;
    hashTable->table = calloc(size, sizeof(MacroNode*));
    if (!hashTable->table) {
        free(hashTable);
        return NULL;
    }
    hashTable->size = size;
    return hashTable;
}


unsigned int hash(const char* name, int hashSize) {
    unsigned long hashValue = 0;
    while (*name) {
        if (isdigit(*name)) hashValue = hashValue * 62 + (*name - '0');
        else if (*name >= 'A' && *name <= 'Z') hashValue = hashValue * 62 + (*name - 'A' + 10);
        else if (*name >= 'a' && *name <= 'z') hashValue = hashValue * 62 + (*name - 'a' + 36);
        name++;
    }
    return hashValue % hashSize;
}


ErrorCode insertMacro(HashTable* hashTable, const char* name, const char* value) {
    unsigned int index = hash(name, hashTable->size);
    MacroNode* newNode = malloc(sizeof(MacroNode));
    if (!newNode) return ERROR_MEMORY_ALLOCATION;

    newNode->name = strdup(name);
    newNode->value = strdup(value);
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;

    return ERROR_NONE;
}


const char* findMacro(HashTable* hashTable, const char* name) {
    unsigned int index = hash(name, hashTable->size);
    MacroNode* current = hashTable->table[index];
    while (current) {
        if (strcmp(current->name, name) == 0) return current->value;
        current = current->next;
    }
    return NULL;
}


void freeHashTable(HashTable* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        MacroNode* current = hashTable->table[i];
        while (current) {
            MacroNode* next = current->next;
            free(current->name);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(hashTable->table);
    free(hashTable);
}


ErrorCode parseFile(const char* filePath, HashTable* hashTable, char** textBuffer) {
    FILE* file = fopen(filePath, "r");
    if (!file) return ERROR_FILE_NOT_FOUND;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "#define", 7) == 0) {
            char name[64], value[128];
            if (sscanf(line, "#define %63s %127[^\n]", name, value) != 2) return ERROR_SYNTAX;
            insertMacro(hashTable, name, value);
        }
        else {
            size_t len = strlen(*textBuffer);
            *textBuffer = realloc(*textBuffer, len + strlen(line) + 1);
            if (!*textBuffer) return ERROR_MEMORY_ALLOCATION;
            strcat(*textBuffer, line);
        }
    }
    fclose(file);
    return ERROR_NONE;
}


void substituteMacros(HashTable* hashTable, const char* text) {
    char buffer[1024] = "";
    char word[64];
    int i = 0, j = 0;
    int replacements;

    do {
        replacements = 0; 
        buffer[0] = '\0';  

        while (text[i] != '\0') {
            if (isalnum(text[i])) {
                word[j++] = text[i];
            }
            else {
                if (j > 0) {
                    word[j] = '\0';
                    const char* replacement = findMacro(hashTable, word);
                    if (replacement) {
                        strcat(buffer, replacement);
                        replacements++;
                    }
                    else {
                        strcat(buffer, word);
                    }
                    j = 0;
                }
                strncat(buffer, &text[i], 1);
            }
            i++;
        }

        if (j > 0) { 
            word[j] = '\0';
            const char* replacement = findMacro(hashTable, word);
            if (replacement) {
                strcat(buffer, replacement);
                replacements++;
            }
            else {
                strcat(buffer, word);
            }
        }

        printf("Intermediate Text after iteration:\n%s\n", buffer);
        strcpy((char*)text, buffer);  
        i = 0;
        j = 0;

    } while (replacements > 0); 

    printf("Processed Text:\n%s\n", buffer);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Error: Invalid number of arguments.\n");
        return EXIT_FAILURE;
    }

    HashTable* hashTable = createHashTable(INITIAL_HASHSIZE);
    if (!hashTable) return ERROR_MEMORY_ALLOCATION;

    char* textBuffer = calloc(1, sizeof(char));
    ErrorCode err = parseFile(argv[1], hashTable, &textBuffer);
    if (err != ERROR_NONE) {
        printf("Error parsing file.\n");
        freeHashTable(hashTable);
        free(textBuffer);
        return ERROR_FILE_NOT_FOUND;
    }

    substituteMacros(hashTable, textBuffer);

    freeHashTable(hashTable);
    free(textBuffer);
    return ERROR_NONE;
}



