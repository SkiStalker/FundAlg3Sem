#include "functions.h"

int main(int argc, char **argv) {
    FILE *input, *output;
    if (argc != 3) {
        puts("Input error! Enter (program_name) (file_in) (file_out)");
        return INPUT_ERROR;
    }

    input = fopen(argv[1], "r");
    if (!input) {
        puts("File open error.\n");
        return INPUT_ERROR;
    }
    output = fopen(argv[2], "w");
    if (!output) {
        fclose(input);
        puts("File open error.\n");
        return INPUT_ERROR;
    }
    if (build_tree(input, output) == MEMORY_ERROR) {
        puts("Build_tree memory error");
        fclose(input);
        fclose(output);
        return MEMORY_ERROR;
    }
    fclose(input);
    fclose(output);

    return 0;
}