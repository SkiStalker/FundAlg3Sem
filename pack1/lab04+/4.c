#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

typedef enum StatusCode {
    SUCCESS = 0,
    FILE_OPEN_ERROR = 1,
    MEMORY_ALLOCATION_ERROR = 2,
    INVALID_FLAG_ERROR = 3,
    ARGUMENT_ERROR = 4,
} StatusCode;


StatusCode exclude_digits(const char *input_file, FILE *output_file) {
    FILE *in_file = fopen(input_file, "r");
    if (!in_file) {
        return FILE_OPEN_ERROR;
    }

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, in_file)) {
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isdigit(line[i])) {
                fputc(line[i], output_file);
            }
        }
    }

    fclose(in_file);
    return SUCCESS;
}


StatusCode count_latin_letters(const char *input_file, FILE *output_file) {
    FILE *in_file = fopen(input_file, "r");
    if (!in_file) {
        return FILE_OPEN_ERROR;
    }

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, in_file)) {
        int count = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (isalpha(line[i])) {
                count++;
            }
        }
        fprintf(output_file, "Latin letters: %d\n", count);
    }

    fclose(in_file);
    return SUCCESS;
}


StatusCode count_special_symbols(const char *input_file, FILE *output_file) {
    FILE *in_file = fopen(input_file, "r");
    if (!in_file) {
        return FILE_OPEN_ERROR;
    }

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, in_file)) {
        int count = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isalnum(line[i]) && !isspace(line[i])) {
                count++;
            }
        }
        fprintf(output_file, "Special symbols: %d\n", count);
    }

    fclose(in_file);
    return SUCCESS;
}


StatusCode replace_with_ascii(const char *input_file, FILE *output_file) {
    FILE *in_file = fopen(input_file, "r");
    if (!in_file) {
        return FILE_OPEN_ERROR;
    }

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, in_file)) {
        for (int i = 0; line[i] != '\0'; i++) {
            if ((unsigned char)line[i] <= 127) {
                fprintf(output_file, "%02x ", (unsigned char)line[i]);
            } else {
                fprintf(output_file, "?? "); 
            }
        }
        fputc('\n', output_file); 
    }

    fclose(in_file);
    return SUCCESS;
}


StatusCode process_file(const char *flag, const char *input_file, const char *output_file) {
    FILE *out_file = fopen(output_file, "w");
    if (!out_file) {
        return FILE_OPEN_ERROR;
    }

    StatusCode result = INVALID_FLAG_ERROR;

    if (strcmp(flag, "-nd") == 0 || strcmp(flag, "/nd") == 0) {
        result = exclude_digits(input_file, out_file);
    } 
    else if (strcmp(flag, "-ni") == 0 || strcmp(flag, "/ni") == 0) {
        result = count_latin_letters(input_file, out_file);
    } 
    else if (strcmp(flag, "-ns") == 0 || strcmp(flag, "/ns") == 0) {
        result = count_special_symbols(input_file, out_file);
    } 
    else if (strcmp(flag, "-na") == 0 || strcmp(flag, "/na") == 0) {
        result = replace_with_ascii(input_file, out_file);
    } 
    else if (strcmp(flag, "-d") == 0 || strcmp(flag, "/d") == 0) {
        result = exclude_digits(input_file, out_file);
    }
    else if (strcmp(flag, "-i") == 0 || strcmp(flag, "/i") == 0) {
        result = count_latin_letters(input_file, out_file);
    }
    else if (strcmp(flag, "-s") == 0 || strcmp(flag, "/s") == 0) {
        result = count_special_symbols(input_file, out_file);
    }
    else if (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0) {
        result = replace_with_ascii(input_file, out_file);
    }

    fclose(out_file);
    return result;
}


int main(int argc, const char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <flag> <input_file> [output_file]\n", argv[0]);
        return ARGUMENT_ERROR;
    }

    const char *flag = argv[1];
    const char *input_file = argv[2];
    char output_file[MAX_LINE];

    
    if (flag[1] == 'n') {
        if (argc == 4) {
            strcpy(output_file, argv[3]);
        } else {
            fprintf(stderr, "Ошибка: для флага с 'n' необходимо указать выходной файл.\n");
            return ARGUMENT_ERROR;
        }
    } else {
        snprintf(output_file, sizeof(output_file), "out_%s", input_file);
    }

    StatusCode result = process_file(flag, input_file, output_file);

    switch (result) {
    case SUCCESS:
        printf("Операция завершена успешно.\n");
        break;
    case FILE_OPEN_ERROR:
        fprintf(stderr, "Ошибка: не удается открыть файл.\n");
        break;
    case MEMORY_ALLOCATION_ERROR:
        fprintf(stderr, "Ошибка: не удалось выделить память.\n");
        break;
    case INVALID_FLAG_ERROR:
        fprintf(stderr, "Ошибка: указан недопустимый флаг.\n");
        break;
    default:
        fprintf(stderr, "Произошла неизвестная ошибка.\n");
        break;
    }

    return result;
}
