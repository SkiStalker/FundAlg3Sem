#include "functions.h"

int main(int argc, char* argv[]) {
    char* filename = argv[1];
    char* separators[argc - 2];
    int sep_count = argc - 2;
    int i;
    Node* root = NULL;

    if (argc < 3) {
        printf("Invalid amount of arguments\n");
        return INVALID_INPUT;
    }

    for (i = 0; i < sep_count; i++) {
        separators[i] = argv[i + 2];
    }

    if (process_file(filename, separators, sep_count, &root) != OK) {
        printf("Error processing file\n");
        return FILE_OPENNING_ERROR;
    }

    interactive_dialog(root, separators);

    free_tree(root);

    return OK;
}
