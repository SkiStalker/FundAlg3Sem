// lb1task8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h> 


typedef enum {
    SUCCESS,
    ERROR_INVALID_NUMBER,
    ERROR_FILE_NOT_FOUND,
    ERROR_FILE_IS_EMPTY,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_CHARACTER,
    ERROR_INVALID_BASE
}ErrorCode;

typedef struct {
    char number[256];
    ErrorCode error_code;
} Result;


int find_intmin(const char* num, ErrorCode* error) {
    int minbase = 2;
    for (const char* p = num; *p; ++p) {
        if (isdigit(*p)) {
            int x = *p - '0';
            minbase = (x + 1 > minbase) ? x + 1 : minbase;
        }
        else if (isalpha(*p)) {
            int x = toupper(*p) - 'A' + 10;
            minbase = (x + 1 > minbase) ? x + 1 : minbase;
        }
        else {
            *error = ERROR_INVALID_CHARACTER;
            return minbase;
        }
    }
    if (minbase > 36) {
        *error = ERROR_INVALID_NUMBER; 
        return minbase;
    }

    *error = SUCCESS;
    return minbase;
}


long long convert_to10(const char* num, int base, ErrorCode* error) {
    long long result = 0;
    for (const char* p = num; *p; ++p) {
        int x;
        if (isdigit(*p)) {
            x = *p - '0';
        }
        else if (isalpha(*p)) {
            x = toupper(*p) - 'A' + 10;
        }
        else {
            *error = ERROR_INVALID_NUMBER;
            return 0;
        }
        if (x >= base || base > 36) {
            *error = ERROR_INVALID_BASE;
            return 0;
        }
        if (result > (LLONG_MAX - x) / base) {
            *error = ERROR_INVALID_NUMBER;
            return 0;
        }

        result = result * base + x;
    }
    *error = SUCCESS;
    return result;
}




ErrorCode process_file(const char* filename, Result results[], int* result_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }
    if (fgetc(file) == EOF) {
        fclose(file);
        return ERROR_FILE_IS_EMPTY;
    }
    rewind(file); 

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, " \t\n");
        while (token) {
            while (*token == '0' && *(token + 1) != '\0') {
                token++;
            }
            ErrorCode error;
            int min_base = find_intmin(token, &error);
            if (error != SUCCESS) {
                results[count].error_code = ERROR_INVALID_CHARACTER;
                strcpy(results[count].number, "");
                count++;
                token = strtok(NULL, " \t\n");
                continue;
            }

            long long decimal_value = convert_to10(token, min_base, &error);
            if (error != SUCCESS) {
                results[count].error_code = ERROR_INVALID_NUMBER;
                strcpy(results[count].number, "");
            }
            else {
                snprintf(results[count].number, sizeof(results[count].number), "%s in base %d is %lld", token, min_base, decimal_value);
                results[count].error_code = SUCCESS;
            }
            count++;
            token = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    *result_count = count;
    return SUCCESS;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Error: Invalid number of arguments.\n");
        return EXIT_FAILURE;
    }

    Result results[100]; 
    int result_count = 0;

    ErrorCode result = process_file(argv[1], results, &result_count);
    if (result != SUCCESS) {
        switch (result) {
        case ERROR_FILE_NOT_FOUND:
            printf("Error: file not found.\n");
            return ERROR_FILE_NOT_FOUND;
        case ERROR_MEMORY_ALLOCATION:
            printf("Error: memory allocation failed.\n");
            return ERROR_MEMORY_ALLOCATION;
        case ERROR_INVALID_NUMBER:
            printf("Error: Invalid number or too large number.\n");
            return ERROR_INVALID_NUMBER;
        case ERROR_FILE_IS_EMPTY:
            printf("Error: Input file is empty.\n");
            return ERROR_FILE_IS_EMPTY;
        default:
            printf("Error: unknown error occurred.\n");
            break;
        }
        return EXIT_FAILURE;
    }

    FILE* output_file = fopen(argv[2], "w");
    if (!output_file) {
        printf("Error: could not open output file.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < result_count; i++) {
        if (results[i].error_code != SUCCESS) {
            printf("Error: invalid number.\n");
            return ERROR_INVALID_NUMBER;
        }
        else {
            fprintf(output_file, "%s\n", results[i].number);
        }
    }

    fclose(output_file);
    return 0;
}