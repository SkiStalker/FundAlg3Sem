#include <stdio.h>
#include <string.h>

enum errors {
    OK,
    INVALID_INPUT,
    INVALID_MEMORY,
    INVALID_FILE
};

enum errors check_file(char* name_in, char* name_out);
void print_tree(FILE* input, FILE* output);

enum errors check_file(char* name_in, char* name_out) {
    char* in = strrchr(name_in, '/');
    if (in) {
        in++;
    }
    else {
        in = name_in;
    }
    char* out = strrchr(name_out, '/');
    if (out) {
        out++;
    }
    else {
        out = name_out;
    }
    if (strcmp(in, out) == 0) {
        return INVALID_FILE;
    }
    return OK;
}

void print_tree(FILE* input, FILE* output) {
    char c = fgetc(input);
    if (c == '\n' || c == EOF) fprintf(output, "\\_\n");

    int count = 0;
    while (c != '\n' && c != EOF) {
        if (c == '(') {
            count++;
        }
        else if (c == ')') {
            count--;
        }
        else if (c == ' ' || c == ',') {
            c = fgetc(input);
            continue;
        }
        else {
            for (int i = 0; i < count * 3; ++i) {
                fprintf(output, " ");
            }
            fprintf(output, "\\_%c\n", c);
        }
        c = fgetc(input);
    }
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Invalid input.\n");
        return INVALID_INPUT;
    }
    if (check_file(argv[1], argv[2]) != OK) {
        printf("We need a different name file.\n");
        return INVALID_FILE;
    }
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    if (input == NULL || output == NULL) {
        printf("Error opening files.\n");
        if (input != NULL) fclose(input);
        if (output != NULL) fclose(output);
        return INVALID_FILE;
    }

    int count = 1;
    while (!feof(input)) {
        fprintf(output, "Tree %d\n", count);
        print_tree(input, output);
        fprintf(output, "\n");
        count++;
    }
    fclose(input);
    fclose(output);
    return 0;
}
