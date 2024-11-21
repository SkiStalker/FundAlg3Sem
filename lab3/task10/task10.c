#include "functions.h"

int main(int argc, char* argv[]) {
    FILE* input_file, *output_file;
    char absolute_path_1[PATH_MAX];
    char absolute_path_2[PATH_MAX];
    if (argc != 3) {
        printf("invalid amount of argumnets\n");
        return INVALID_INPUT;
    }

    if (strcmp(absolute_path_1, absolute_path_2) == 0) {
        printf("The same paths\n");
        return INVALID_INPUT;
    }
    if (!(input_file = fopen(argv[1], "r")) || !(output_file = fopen(argv[2], "w"))) {
        fclose(input_file);
        fclose(output_file);
        printf("file openning error\n");
        return FILE_OPENNING_ERROR;
    }

    return_code rc_of_building_tree = construct_tree(input_file, output_file);
    if (rc_of_building_tree == MEMORY_ALLOCATION_ERROR) {
        printf("memory allocation error\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    fclose(input_file);
    fclose(output_file);
}