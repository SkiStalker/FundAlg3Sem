#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    SUCCESS = 0,
    ARGUMENT_ERROR = 1,
    FILE_ERROR = 2,
    MEMORY_ERROR = 3
} StatusCode;

StatusCode to_lowercase(const char *input, char *output, size_t size) {
    if (!input || !output || size == 0) {
        return ARGUMENT_ERROR;
    }
    
    size_t i;
    for (i = 0; i < size - 1 && input[i] != '\0'; i++) {
        output[i] = tolower(input[i]);
    }
    output[i] = '\0'; 
    return SUCCESS;
}

StatusCode to_base4_ascii(const char *input, char **output) {
    if (!input || !output) {
        return ARGUMENT_ERROR;
    }
    
    size_t len = strlen(input);
    size_t output_len = len * 5;  
    *output = (char *)malloc(output_len + 1); 
    if (*output == NULL) {
        return MEMORY_ERROR;
    }

    char *ptr = *output;
    for (size_t i = 0; i < len; i++) {
        if (isdigit(input[i]) || isalpha(input[i])) {
            ptr += sprintf(ptr, "%04d ", input[i]);
        }
    }
    *(ptr - 1) = '\0'; 
    return SUCCESS;
}

StatusCode to_base8_ascii(const char *input, char **output) {
    if (!input || !output) {
        return ARGUMENT_ERROR;
    }
    
    size_t len = strlen(input);
    size_t output_len = len * 4;  
    *output = (char *)malloc(output_len + 1); 
    if (*output == NULL) {
        return MEMORY_ERROR;
    }

    char *ptr = *output;
    for (size_t i = 0; i < len; i++) {
        if (isdigit(input[i]) || isalpha(input[i])) {
            ptr += sprintf(ptr, "%03o ", input[i]);
        }
    }
    *(ptr - 1) = '\0';  
    return SUCCESS;
}

StatusCode process_flag_r(const char *file1_path, const char *file2_path, const char *output_path) {
    FILE *file1 = fopen(file1_path, "r");
    FILE *file2 = fopen(file2_path, "r");
    FILE *output = fopen(output_path, "w");

    if (!file1 || !file2 || !output) {
        if (file1) 
        fclose(file1);
        if (file2) 
        fclose(file2);
        if (output) 
        fclose(output);
        return FILE_ERROR;
    }

    char word1[100], word2[100];
    int read_file1 = 1, read_file2 = 1;

    while (read_file1 || read_file2) {
        if (read_file1 && fscanf(file1, "%99s", word1) == 1) {
            fprintf(output, "%s ", word1);
        } else {
            read_file1 = 0;
        }

        if (read_file2 && fscanf(file2, "%99s", word2) == 1) {
            fprintf(output, "%s ", word2);
        } else {
            read_file2 = 0;
        }
    }

    fclose(file1);
    fclose(file2);
    fclose(output);

    return SUCCESS;
}

StatusCode process_flag_a(const char *input_path, const char *output_path) {
    FILE *input = fopen(input_path, "r");
    FILE *output = fopen(output_path, "w");

    if (!input || !output) {
        if (input) fclose(input);
        if (output) fclose(output);
        return FILE_ERROR;
    }

    char word[100];
    int word_count = 0;
    StatusCode status;
    char *converted = NULL;

    while (fscanf(input, "%99s", word) == 1) {
        word_count++;

        if (word_count % 10 == 0) {
            char lower_word[100];
            status = to_lowercase(word, lower_word, sizeof(lower_word));
            if (status != SUCCESS) return status;

            status = to_base4_ascii(lower_word, &converted);
            if (status != SUCCESS) return status;

            fprintf(output, "%s ", converted);
            free(converted);
        } else if (word_count % 5 == 0) {
            status = to_base8_ascii(word, &converted);
            if (status != SUCCESS) return status;

            fprintf(output, "%s ", converted);
            free(converted);
        } else if (word_count % 2 == 0) {
            char lower_word[100];
            status = to_lowercase(word, lower_word, sizeof(lower_word));
            if (status != SUCCESS) return status;

            fprintf(output, "%s ", lower_word);
        } else {
            fprintf(output, "%s ", word);
        }
    }

    fclose(input);
    fclose(output);
    
    return SUCCESS;
}

StatusCode validate_arguments(int argc, const char *argv[], char *flag) {
    if (argc < 4) {
        return ARGUMENT_ERROR;
    }

    *flag = argv[1][0];
    if (*flag != '-' && *flag != '/') {
        return ARGUMENT_ERROR;
    }

    if (strcmp(argv[1], "-r") == 0 && argc != 5) {
        return ARGUMENT_ERROR;
    }

    if (strcmp(argv[1], "-a") == 0 && argc != 4) {
        return ARGUMENT_ERROR;
    }

    return SUCCESS;
}

int main(int argc, const char *argv[]) {
    char flag;
    StatusCode status = validate_arguments(argc, argv, &flag);

    if (status != SUCCESS) {
        fprintf(stderr, "Usage error. Please check the flag and number of arguments.\n");
        return status;
    }

    if (strcmp(argv[1], "-r") == 0) {
        status = process_flag_r(argv[2], argv[3], argv[4]);
    } else if (strcmp(argv[1], "-a") == 0) {
        status = process_flag_a(argv[2], argv[3]);
    }

    if (status != SUCCESS) {
        fprintf(stderr, "An error occurred during file processing.\n");
    }

    return status;
}
