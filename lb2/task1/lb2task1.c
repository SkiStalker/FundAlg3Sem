#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


typedef enum {
    SUCCESS,
    ERR_NOT_ENOUGH_ARGS,
    ERR_MEMORY_ALLOCATION,
    ERR_UNKNOWN_FLAG,
    ERR_INVALID_CONCATENATION_ARGS
} ErrorCode;


char* length_of_string(const char* str, ErrorCode* err);
char* reverse_string(const char* str, ErrorCode* err);
char* odd_uppercase_string(const char* str, ErrorCode* err);
char* separate_characters(const char* str, ErrorCode* err);
char* concatenate_strings(int count, char** strings, unsigned int seed, ErrorCode* err);


int my_strlen(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

char* my_strcat(char* dest, const char* src) {
    int dest_len = my_strlen(dest);
    int i;
    for (i = 0; src[i] != '\0'; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + i] = '\0'; 
    return dest;
}


int my_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}


char* length_of_string(const char* str, ErrorCode* err) {
    if (!str) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    int length = my_strlen(str);
    char* result = (char*)malloc(20 * sizeof(char)); 
    if (!result) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }
    sprintf(result, "%d", length);
    *err = SUCCESS;
    return result;
}


char* reverse_string(const char* str, ErrorCode* err) {
    if (!str) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    int length = my_strlen(str);
    char* result = (char*)malloc((length + 1) * sizeof(char));
    if (!result) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        result[i] = str[length - 1 - i];
    }
    result[length] = '\0';
    *err = SUCCESS;
    return result;
}


char* odd_uppercase_string(const char* str, ErrorCode* err) {
    if (!str) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    int length = my_strlen(str);
    char* result = (char*)malloc((length + 1) * sizeof(char));
    if (!result) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        result[i] = (i % 2 == 1) ? toupper(str[i]) : str[i];
    }
    result[length] = '\0';
    *err = SUCCESS;
    return result;
}


char* separate_characters(const char* str, ErrorCode* err) {
    if (!str) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    int digit_count = 0, letter_count = 0, other_count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            digit_count++;
        }
        else if (isalpha(str[i])) {
            letter_count++;
        }
        else {
            other_count++;
        }
    }

    char* result = (char*)malloc((digit_count + letter_count + other_count + 1) * sizeof(char));
    if (!result) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    int idx = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            result[idx++] = str[i];
        }
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            result[idx++] = str[i];
        }
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i])) {
            result[idx++] = str[i];
        }
    }

    result[idx] = '\0'; 
    *err = SUCCESS;
    return result;
}



char* concatenate_strings(int count, char** strings, unsigned int seed, ErrorCode* err) {
    if (count < 1 || !strings) {
        *err = ERR_INVALID_CONCATENATION_ARGS;
        return NULL;
    }

    srand(seed);
    char* result = (char*)malloc(1024 * sizeof(char)); 
    if (!result) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }

    result[0] = '\0'; 
    for (int i = 0; i < count; i++) {
        int random_index = rand() % count;
        my_strcat(result, strings[random_index]);
    }
    *err = SUCCESS;
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Not enough arguments.\n");
        return ERR_NOT_ENOUGH_ARGS;
    }

    const char* flag = argv[1];
    const char* input_string = argv[2];
    ErrorCode err;

    if (my_strcmp(flag, "-l") == 0) {
        char* length_result = length_of_string(input_string, &err);
        if (err == SUCCESS) {
            printf("Length of string: %s\n", length_result);
            free(length_result);
        }
        else {
            fprintf(stderr, "Error occurred: Memory allocation failed.\n");
            return err;
        }
    }
    else if (my_strcmp(flag, "-r") == 0) {
        char* reversed_result = reverse_string(input_string, &err);
        if (err == SUCCESS) {
            printf("Reversed string: %s\n", reversed_result);
            free(reversed_result);
        } 
        else {
            fprintf(stderr, "Error occurred: Memory allocation failed.\n");
            return err;
        }
    }
    else if (my_strcmp(flag, "-u") == 0) {
        char* odd_upper_result = odd_uppercase_string(input_string, &err);
        if (err == SUCCESS) {
            printf("String with uppercase at odd positions: %s\n", odd_upper_result);
            free(odd_upper_result);
        }
        else {
            fprintf(stderr, "Error occurred: Memory allocation failed.\n");
            return err;
        }
    }
    else if (my_strcmp(flag, "-n") == 0) {
        char* separated_result = separate_characters(input_string, &err);
        if (err == SUCCESS) {
            printf("String with separated characters: %s\n", separated_result);
            free(separated_result);
        }
        else {
            fprintf(stderr, "Error occurred: Memory allocation failed.\n");
            return err;
        }
    }
    else if (my_strcmp(flag, "-c") == 0) {
        if (argc < 5) {
            printf("For flag -c, you must specify a count, a seed, and at least one string.\n");
            return ERR_INVALID_CONCATENATION_ARGS;
        }

        int expected_count = atoi(argv[2]);
        unsigned int seed = atoi(argv[3]);

        if (argc - 4 != expected_count) {
            printf("Error: Expected %d strings, but got %d.\n", expected_count, argc - 3);
            return ERR_INVALID_CONCATENATION_ARGS;
        }

        char* concatenated_result = concatenate_strings(expected_count, &argv[4], seed, &err);
        if (err == SUCCESS) {
            printf("Concatenated string: %s\n", concatenated_result);
            free(concatenated_result);
        }
        else {
            printf("Error occurred: %s\n", (err == ERR_MEMORY_ALLOCATION) ? "Memory allocation failed." : "Invalid concatenation arguments.");
            return err;
        }
    }

    else {
        printf("Unknown flag\n");
        return ERR_UNKNOWN_FLAG;
    }

    return SUCCESS;
}

