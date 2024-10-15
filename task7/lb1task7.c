// lb1task7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    SUCCESS,
    FILE_ERROR,
    ARGUMENT_ERROR,
    MEMORY_ERROR,
    INVALID_FLAG
} ErrorCode;

void to_lower(const char* str, char* result) {
    while (*str) {
        if (isalpha(*str)) {
            *result++ = tolower(*str);
        }
        else {
            *result++ = *str;
        }
        str++;
    }
    *result = '\0';
}

void ascii_base(const char* str, char* result, int base) {
    unsigned char ch;
    while (*str) {
        ch = (unsigned char)*str++;
        char buff[32];
        int ind = 0;
        do {
            buff[ind++] = (ch % base) + '0';
            ch /= base;
        } while (ch > 0);
        for (int i = ind - 1; i >= 0; i--) {
            *result++ = buff[i];
        }
        *result++ = ' ';
    }
    *result = '\0';
}

void process_lexeme(const char* lexem, char* result, int index) {
    char temp[4096] = "";
    if ((index + 1) % 10 == 0) {
        char lower[1024];
        to_lower(lexem, lower);
        ascii_base(lower, temp, 4);
    }
    else if ((index + 1) % 2 == 0) {
        to_lower(lexem, temp);
    }
    else if ((index + 1) % 5 == 0) {
        ascii_base(lexem, temp, 8);
    }
    else {
        strcpy(temp, lexem);
    }
    strcpy(result, temp);
}

ErrorCode flagR(const char* file1, const char* file2, const char* output_file) {
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");
    FILE* fout = fopen(output_file, "w");
    if (!f1 || !f2 || !fout) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        if (fout) fclose(fout);
        return FILE_ERROR;
    }

    char lexem1[1024], lexem2[1024];
    int first = 1; 

    while (1) {
        int read1 = fscanf(f1, "%1023s", lexem1);
        int read2 = fscanf(f2, "%1023s", lexem2);

        if (read1 == EOF && read2 == EOF) {
            break;
        }

        if (read1 != EOF) {
            if (!first) fputs(" ", fout);
            fputs(lexem1, fout);
            first = 0;
        }

        if (read2 != EOF) {
            if (!first) fputs(" ", fout);
            fputs(lexem2, fout);
            first = 0;
        }
    }

    fclose(f1);
    fclose(f2);
    fclose(fout);
    return SUCCESS;
}

ErrorCode flagA(const char* input_file, const char* output_file) {
    FILE* fin = fopen(input_file, "r");
    FILE* fout = fopen(output_file, "w");
    if (!fin || !fout) {
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return FILE_ERROR;
    }

    char lexem[1024];
    int ind = 0;
    int first = 1;

    while (fscanf(fin, "%1023s", lexem) != EOF) {
        char proc[4096] = "";
        if ((ind + 1) % 10 == 0) {
            char lower[1024];
            to_lower(lexem, lower);
            ascii_base(lower, proc, 4);
        }
        else if ((ind + 1) % 2 == 0) {
            to_lower(lexem, proc);
        }
        else if ((ind + 1) % 5 == 0) {
            ascii_base(lexem, proc, 8);
        }
        else {
            strcpy(proc, lexem);
        }

        if (!first) fputs(" ", fout);
        fputs(proc, fout);
        first = 0;
        ind++;
    }

    fclose(fin);
    fclose(fout);
    return SUCCESS;
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        return ARGUMENT_ERROR;
    }

    ErrorCode result;
    if (argv[1][0] == '-' && strcmp(argv[1], "-r") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Error: Invalid number of arguments for -r flag.\n");
            return ARGUMENT_ERROR;
        }
        result = flagR(argv[2], argv[3], argv[4]);
    }
    else if (argv[1][0] == '-' && strcmp(argv[1], "-a") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Error: Invalid number of arguments for -a flag.\n");
            return ARGUMENT_ERROR;
        }
        result = flagA(argv[2], argv[3]);
    }
    else {
        fprintf(stderr, "Error: Invalid flag.\n");
        return INVALID_FLAG;
    }

    if (result != SUCCESS) {
        if (result == FILE_ERROR) {
            fprintf(stderr, "Error: One or more files could not be opened.\n");
        }
        return result;
    }

    return 0;
}







