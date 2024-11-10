#include "functions.h"

int main(int argc, char* argv[]) {
    char absolute_path_1[PATH_MAX];
    char absolute_path_2[PATH_MAX];
    FILE* input_file, *output_file;
    int size = 0;
    Student* students = NULL;
    char* parameters[] = {"id", "name", "lname", "group"};
    Student finded_student;

    if (argc != 3) {
        printf("Invalid amount of arguments\n");
        return INVALID_INPUT;
    }

    strncpy(absolute_path_1, argv[1], PATH_MAX);
    strncpy(absolute_path_2, argv[2], PATH_MAX);

    if (strcmp(absolute_path_1, absolute_path_2) == 0) {
        printf ("The same paths\n");
        return INVALID_INPUT;
    }

    if (!(input_file = fopen(absolute_path_1, "r"))) {
        printf("Couldn't open file\n");
        return FILE_OPENNING_ERROR;
    }

    if (!(output_file = fopen(absolute_path_2, "w"))) {
        printf("Couldn't open file\n");
        return FILE_OPENNING_ERROR;
    }

    upload_info(&students, &size, input_file);
    interactive_dialog(students, size, output_file);

    fclose(input_file);
    fclose(output_file);
    for (int i = 0; i < size; ++i) {
        free(students[i].marks);
    }
    free(students);
}