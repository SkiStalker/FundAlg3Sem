#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum ret_type_t {
    SUCCESS, 
    ERROR_OPEN, 
};
typedef int (*callback)(char*, char*);

bool find_element(const char* db_output_file[], const char* element_to_check, int argc) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(db_output_file[i], element_to_check) == 0) {
            return true;
        }
    }
    return false;
}

int process_file(const char* input_file, const char* output_file, int exclude_numbers, int count_letters, int count_specials, int substitute_specials, const char* db_output_file[], int argc) {
    FILE* in = fopen(input_file, "r");
    if (!in) {
        return ERROR_OPEN;
    }

    FILE* out;
    if (find_element(db_output_file, output_file, argc)) {
        out = fopen(output_file, "a+");
    }
    else {
        out = fopen(output_file, "w+");
    }

    if (!out) {
        fclose(in);
        return ERROR_OPEN;
    }

    char line[256];
    while (fgets(line, sizeof(line), in)) {
        char processed_line[256] = "";
        int letter_count = 0;
        int special_count = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            char c = line[i];
            if (exclude_numbers && isdigit(c)) {
                continue;
            }
            if (isalpha(c)) {
                letter_count++;
            }
            else if (!isalnum(c) && c != ' ') {
                special_count++;
            }

            if (substitute_specials && !isalnum(c)) {
                char buffer[10];
                sprintf(buffer, "\\x%02X", (unsigned char)c);
                strcat(processed_line, buffer);
            }
            else {
                strncat(processed_line, &c, 1);
            }
        }

        if (exclude_numbers) {
            fprintf(out, "%s", processed_line);
        }

        if (substitute_specials) {
            fprintf(out, "%s\n", processed_line);
        }

        if (count_letters) {
            fprintf(out, "Latin letters count: %d\n", letter_count);
        }

        if (count_specials) {
            fprintf(out, "Special characters count: %d\n", special_count);
        }
    }

    fclose(in);
    fclose(out);
    return SUCCESS;
}

int findFlag(char* qarg, const char** flags, int size) {
    for (int i = 0; i < size; ++i) {
        if (!(strcmp(qarg, flags[i]))) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Wrong amount arguments.\n");
        return 1;
    }

    const char* flags[] = { "-d", "/d", "-i", "/i", "-s", "/s", "-a", "/a", "-nd", "/nd", "-ni", "/ni", "-ns", "/ns", "-na", "/na" };
    const char* db_output_file[argc];
    int len = 0;

    for (int i = 1; i < argc; ++i) {
        int exclude_numbers = 0;
        int count_letters = 0;
        int count_specials = 0;
        int substitute_specials = 0;

        int ret = findFlag(argv[i], flags, sizeof(flags) / sizeof(char*));
        if (ret == -1) {
            printf("THIS FLAG DOES NOT EXIST: %s\n", argv[i]);
            return 1;
        }

        int findCbsInt = ret / 2;
        char* input_file = NULL;
        char* output_file = NULL;

        if (findCbsInt == 0) {
            exclude_numbers = 1;
        }
        else if (findCbsInt == 1) {
            count_letters = 1;
        }
        else if (findCbsInt == 2) {
            count_specials = 1;
        }
        else if (findCbsInt == 3) {
            substitute_specials = 1;
        }
        if (findCbsInt >= 4) {
            if (i + 2 >= argc) {
                printf("There are not enough arguments for flag %s\n", argv[i]);
                return 1;
            }
            input_file = argv[i + 1];
            output_file = argv[i + 2];

            if (strcmp(input_file, output_file) == 0) {
                printf("Names input's and output's are same: %s\n", input_file);
                return 1;
            }

            i += 2;
        }
        else {
  
            if (i + 1 >= argc) {
                printf("Íå õâàòàåò àðãóìåíòîâ äëÿ ôëàãà %s\n", argv[i]);
                return 1;
            }
            input_file = argv[i + 1];
            char* token = strrchr(input_file, '/');
            token = token ? token + 1 : input_file;

            char output_path[256];
            char* last_slash = strrchr(input_file, '/');
            if (last_slash != NULL) {
                size_t length = last_slash - input_file + 1;
                strncpy(output_path, input_file, length);
                output_path[length] = '\0';
            }
            else {
                strcpy(output_path, "./");
            }

            char output_file1[256];
            snprintf(output_file1, sizeof(output_file1), "%sout_%s", output_path, token);

            if (strcmp(input_file, output_file1) == 0) {
                printf("Names input's and output's files are same: %s\n", input_file);
                return 1;
            }

            output_file = output_file1;
            i++;
        }

        int h = process_file(input_file, output_file, exclude_numbers, count_letters, count_specials, substitute_specials, db_output_file, len);
        if (h == ERROR_OPEN) {
            perror("Unable to open file");
            return 1;
        }

        db_output_file[len] = strdup(output_file);
        len++;
    }

    return 0;
}