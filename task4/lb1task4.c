// lb1task4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    SUCCESS,
    FILE_OPEN_ERROR,
    MEMORY_ALLOCATION_ERROR,
    INVALID_ARGUMENTS,
    UNKNOWN_FLAG
} ErrorCode;

ErrorCode FuncD(const char* line, char** output_buffer, size_t* buffer_size);
ErrorCode FuncI(const char* line, char** output_buffer, size_t* buffer_size);
ErrorCode FuncS(const char* line, char** output_buffer, size_t* buffer_size);
ErrorCode FuncA(const char* line, char** output_buffer, size_t* buffer_size);

ErrorCode process_file(const char* input_path, char flag, char** output_buffer, size_t* output_size) {
    FILE* infile = fopen(input_path, "r");
    if (!infile) {
        *output_buffer = NULL;
        *output_size = 0;
        return FILE_OPEN_ERROR;
    }

    char line[1024];
    *output_buffer = NULL;
    size_t buffer_size = 0;

    while (fgets(line, sizeof(line), infile)) {
        ErrorCode result;
        switch (flag) {
        case 'd':
            result = FuncD(line, output_buffer, &buffer_size);
            break;
        case 'i':
            result = FuncI(line, output_buffer, &buffer_size);
            break;
        case 's':
            result = FuncS(line, output_buffer, &buffer_size);
            break;
        case 'a':
            result = FuncA(line, output_buffer, &buffer_size);
            break;
        default:
            fclose(infile);
            *output_buffer = NULL;
            *output_size = 0;
            return UNKNOWN_FLAG;
        }
        if (result != SUCCESS) {
            fclose(infile);
            return result;  // Возврат ошибки, если она произошла
        }
    }

    fclose(infile);
    *output_size = buffer_size;
    return SUCCESS;
}


ErrorCode FuncD(const char* line, char** output_buffer, size_t* buffer_size) {
    for (int i = 0; line[i]; i++) {
        if (!isdigit(line[i])) {
            char* new_buffer = realloc(*output_buffer, *buffer_size + 2);
            if (!new_buffer) {
                *output_buffer = NULL;
                *buffer_size = 0;
                return MEMORY_ALLOCATION_ERROR;
            }
            *output_buffer = new_buffer;
            (*output_buffer)[(*buffer_size)++] = line[i];
        }
    }
    char* new_buffer = realloc(*output_buffer, *buffer_size + 2);
    if (new_buffer) {
        *output_buffer = new_buffer;
        (*output_buffer)[(*buffer_size)++] = '\n';
    }
    return SUCCESS;
}

ErrorCode FuncI(const char* line, char** output_buffer, size_t* buffer_size) {
    int local_count = 0;
    for (int i = 0; line[i]; i++) {
        if (isalpha(line[i]) && isascii(line[i])) {
            local_count++;
        }
    }
    char count_str[10];
    sprintf(count_str, "%d\n", local_count);
    size_t count_len = strlen(count_str);
    char* new_buffer = realloc(*output_buffer, *buffer_size + count_len + 1);
    if (!new_buffer) {
        *output_buffer = NULL;
        *buffer_size = 0;
        return MEMORY_ALLOCATION_ERROR;
    }
    *output_buffer = new_buffer;
    strcpy(*output_buffer + *buffer_size, count_str);
    *buffer_size += count_len;
    return SUCCESS;
}

ErrorCode FuncS(const char* line, char** output_buffer, size_t* buffer_size) {
    int special_char_count = 0;
    for (int i = 0; line[i]; i++) {
        if (!isalpha(line[i]) && !isdigit(line[i]) && !isspace(line[i])) {
            special_char_count++;
        }
    }
    char count_str[10];
    sprintf(count_str, "%d\n", special_char_count);
    size_t count_len = strlen(count_str);
    char* new_buffer = realloc(*output_buffer, *buffer_size + count_len + 1);
    if (!new_buffer) {
        *output_buffer = NULL;
        *buffer_size = 0;
        return MEMORY_ALLOCATION_ERROR;
    }
    *output_buffer = new_buffer;
    strcpy(*output_buffer + *buffer_size, count_str);
    *buffer_size += count_len;
    return SUCCESS;
}

ErrorCode FuncA(const char* line, char** output_buffer, size_t* buffer_size) {
    for (int i = 0; line[i]; i++) {
        char* new_buffer;
        if (!isdigit(line[i])) {
            new_buffer = realloc(*output_buffer, *buffer_size + 3);
            if (!new_buffer) {
                *output_buffer = NULL;
                *buffer_size = 0;
                return MEMORY_ALLOCATION_ERROR;
            }
            *output_buffer = new_buffer;
            *buffer_size += sprintf(*output_buffer + *buffer_size, "%02X", (unsigned char)line[i]);
        }
        else {
            new_buffer = realloc(*output_buffer, *buffer_size + 2);
            if (!new_buffer) {
                *output_buffer = NULL;
                *buffer_size = 0;
                return MEMORY_ALLOCATION_ERROR;
            }
            *output_buffer = new_buffer;
            (*output_buffer)[(*buffer_size)++] = line[i];
        }
    }
    *output_buffer = realloc(*output_buffer, *buffer_size + 2);
    (*output_buffer)[(*buffer_size)++] = '\n';
    return SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Error: Invalid arguments provided.\n");
        return INVALID_ARGUMENTS;
    }

    char* flag = argv[1];
    const char* input_path = argv[2];
    const char* output_path;

    if (flag[0] == '-' || flag[0] == '/') {
        char* output_buffer = NULL;
        size_t output_size = 0;

        if (flag[1] == 'n') {
            if (argc < 4) {
                fprintf(stderr, "Error: Invalid arguments provided.\n");
                return INVALID_ARGUMENTS;
            }
            output_path = argv[3];
        }
        else {
            output_path = malloc(strlen(input_path) + 5);
            if (!output_path) {
                fprintf(stderr, "Error: Memory allocation failed.\n");
                return MEMORY_ALLOCATION_ERROR;
            }
            sprintf((char*)output_path, "out_%s", input_path);
        }

        ErrorCode result = process_file(input_path, flag[1 + (flag[1] == 'n')], &output_buffer, &output_size);

        if (result != SUCCESS) {
            switch (result) {
            case UNKNOWN_FLAG:
                printf("Error: Unknown flag provided.\n");
                return UNKNOWN_FLAG;
                break;
            case FILE_OPEN_ERROR:
                printf("Error: Could not open input file.\n");
                return FILE_OPEN_ERROR;
                break;
            case MEMORY_ALLOCATION_ERROR:
                printf("Error: Memory allocation failed.\n");
                return MEMORY_ALLOCATION_ERROR;
                break;
                // Добавьте дополнительные случаи для других ошибок, если необходимо
            default:
                printf("Error: %d occurred during processing.\n", result);
                break;
            }
        }


        free(output_buffer);
        if (flag[1] != 'n') {
            free((void*)output_path);
        }
    }
    else {
        printf("Error: Unknown flag provided.\n");
        return UNKNOWN_FLAG;
    }

    return 0;
}


