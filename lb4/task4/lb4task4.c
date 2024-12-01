#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

#define COUNT_VECTORS 26

typedef enum {
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    MISSING, 
    FILE_OPEN_ERROR
} ErrorCode;

typedef struct {
    char* arr;
    size_t len;
    size_t capacity;
} String;

typedef struct {
    int init;
    char name;
    long value;
} Vector;


ErrorCode string_to_long(const char* input_str, long* output, int base)
{
    char* endptr;
    if (!output || !input_str) {
        return MEMORY_ERROR;
    }
    *output = strtol(input_str, &endptr, base);
    if (*output == LONG_MAX || *output == LONG_MIN) {
        return INPUT_ERROR;
    }
    if (*endptr != '\0') {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

ErrorCode validate_file(char** argv)
{
    if (!argv) {
        return INPUT_ERROR;
    }
    char resolved_path1[BUFSIZ], resolved_path2[BUFSIZ];
    resolved_path2[0] = '\0';

    if (!realpath(argv[1], resolved_path1)) {
        return INPUT_ERROR;
    }
    realpath(argv[3], resolved_path2);

    if (!strcmp(resolved_path1, resolved_path2)) {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

ErrorCode convert_to_base(long number, long base, char* output)
{
    if (!output) {
        return MEMORY_ERROR;
    }
    int i, is_negative = 0;
    char temp;
    int result_len = 0;

    if (number == 0)
    {
        output[0] = '0';
        output[1] = '\0';
        return SUCCESS;
    }

    if (number < 0)
    {
        number = labs(number);
        is_negative = 1;
    }

    while (number)
    {
        output[result_len++] = (number % base > 9) ? (number % base - 10 + 'A') : (number % base + '0');
        number /= base;
    }

    if (is_negative)
        output[result_len++] = '-';

    for (i = 0; i < result_len / 2; i++)
    {
        temp = output[i];
        output[i] = output[result_len - 1 - i];
        output[result_len - 1 - i] = temp;
    }

    output[result_len] = '\0';
    return SUCCESS;
}

ErrorCode convert_to_bit_string(long number, char* output)
{
    if (!output) {
        return MEMORY_ERROR;
    }
    for (size_t i = 0; i < sizeof(long) * 8; ++i)
    {
        output[i] = ((1UL << i) & number) ? '1' : '0';
    }

    output[sizeof(long) * 8] = '\0';
    return SUCCESS;
}

ErrorCode string_initialize(String* str)
{
    if (!str) {
        return MEMORY_ERROR;
    }
    str->capacity = 1;
    str->len = 0;
    str->arr = (char*)malloc(sizeof(char));

    if (!str->arr) {
        return MEMORY_ERROR;
    }
    str->arr[0] = '\0';
    return SUCCESS;
}

ErrorCode delete_string(String* str)
{
    if (!str) {
        return MEMORY_ERROR;
    }
    free(str->arr);
    str->arr = NULL;
    str->len = 0;
    str->capacity = 0;
    return SUCCESS;
}

int compare_strings(const String* str1, const String* str2)
{
    if (str1->len != str2->len) {
        return str1->len - str2->len;
    }
    return strcmp(str1->arr, str2->arr);
}

int are_strings_equal(const String* str1, const String* str2)
{
    return strcmp(str1->arr, str2->arr) == 0;
}

ErrorCode copy_string(String* destination, const String* source)
{
    if (!destination || !source) {
        return MEMORY_ERROR;
    }
    destination->capacity = source->capacity;
    destination->len = source->len;

    if (!source->arr)
    {
        destination->arr = NULL;
        return SUCCESS;
    }

    destination->arr = (char*)malloc(source->capacity * sizeof(char));

    if (!destination->arr) {
        return MEMORY_ERROR;
    }
    strcpy(destination->arr, source->arr);
    return SUCCESS;
}

String* destroy_string(String* str)
{
    if (!str) {
        return NULL;
    }
    free(str->arr);
    free(str);
    return NULL;
}

ErrorCode add_char_to_string(String* str, char ch)
{
    if (!str) {
        return MEMORY_ERROR;
    }
    if (str->capacity == 0)
    {
        if (string_initialize(str) != SUCCESS) {
            return MEMORY_ERROR;
        }
    }

    if (str->capacity == str->len + 1)
    {
        char* new_arr = (char*)realloc(str->arr, str->capacity * 2 * sizeof(char));
        if (!new_arr)
            return MEMORY_ERROR;

        str->arr = new_arr;
        str->capacity *= 2;
    }

    str->arr[str->len++] = ch;
    str->arr[str->len] = '\0';
    return SUCCESS;
}

ErrorCode read_word (String* str)
{
    char ch;
    if (!str) {
        return MEMORY_ERROR;
    }
    if (string_initialize(str) != SUCCESS) {
        return MEMORY_ERROR;
    }
    ch = getchar();
    while (ch != EOF && !isspace(ch))
    {
        if (add_char_to_string(str, ch) != SUCCESS)
            return MEMORY_ERROR;

        ch = getchar();
    }

    return SUCCESS;
}

ErrorCode read_string_from_file(FILE* input, String* str)
{
    char ch;
    if (!input || !str) {
        return MEMORY_ERROR;
    }
    if (string_initialize(str) != SUCCESS) {
        return MEMORY_ERROR;
    }
    ch = getc(input);
    while (ch != EOF && ch != ';')
    {
        if (ch == '%')
        {
            while (ch != EOF && ch != '\n')
                ch = getc(input);
        }
        else if (ch == '{')
        {
            while (ch != EOF && ch != '}')
                ch = getc(input);

            if (ch == EOF)
                return INPUT_ERROR;
        }
        else if (ch != EOF && !isspace(ch))
        {
            ch = toupper(ch);
            if (add_char_to_string(str, ch) != SUCCESS)
                return MEMORY_ERROR;
        }

        ch = getc(input);
    }

    if (ch != EOF)
    {
        if (add_char_to_string(str, ch) != SUCCESS)
            return MEMORY_ERROR;
    }

    return SUCCESS;
}

ErrorCode read_vector(Vector* vector, const char* value, long base)
{
    if (!vector) {
        return MEMORY_ERROR;
    }
    vector->init = 1;
    if (string_to_long(value, &vector->value, (int)base) != SUCCESS)
    {
        return INPUT_ERROR;
    }

    return SUCCESS;
}

ErrorCode perform_operation(Vector* a, Vector* b, const char* operation, Vector* result)
{
    if (!a || !b || !result)
        return MEMORY_ERROR;

    if (!a->init || !b->init)
        return MISSING;

    result->init = 1;

    if (!strcmp(operation, "+"))
        result->value = a->value | b->value;

    else if (!strcmp(operation, "&"))
        result->value = a->value & b->value;

    else if (!strcmp(operation, "->"))
        result->value = (!a->value) | b->value;

    else if (!strcmp(operation, "<-"))
        result->value = a->value | (!b->value);

    else if (!strcmp(operation, "~"))
        result->value = ((!a->value) | b->value) & (a->value | (!b->value));

    else if (!strcmp(operation, "<>"))
        result->value = a->value ^ b->value;

    else if (!strcmp(operation, "+>"))
        result->value = a->value & (!b->value);

    else if (!strcmp(operation, "?"))
        result->value = (!a->value) | (!b->value);

    else if (!strcmp(operation, "!"))
        result->value = (!a->value) & (!b->value);

    return SUCCESS;
}

ErrorCode initialize_vector_array(Vector* vector_array)
{
    if (!vector_array) {
        return MEMORY_ERROR;
    }
    for (int i = 0; i < COUNT_VECTORS; ++i)
    {
        vector_array[i].init = 0;
        vector_array[i].name = 'A' + i;
        vector_array[i].value = 0;
    }

    return SUCCESS;
}

void print_message(const char* msg, FILE* out, ...)
{
    va_list args;
    if (!msg)
        return;

    va_start(args, out);
    if (out)
    {
        vfprintf(out, msg, args);
    }
    else
    {
        vprintf(msg, args);
    }
    va_end(args);
}


int main(int argc, char** argv)
{
    FILE* input_file = NULL, * trace_file = NULL;
    String current_line;
    Vector* vectors_array;
    char separator, vector_name, base_str[BUFSIZ];
    long base;
    

    if (argc != 2 && argc != 4)
    {
        printf("Input error.\n");
        return INPUT_ERROR;
    }

    if (argc == 4)
    {
        if (strcmp("/trace", argv[2]) || validate_file(argv))
        {
            printf("Input error.\n");
            return INPUT_ERROR;
        }
        trace_file = fopen(argv[3], "w");
        if (!trace_file)
        {
            printf("File open error\n");
            return FILE_OPEN_ERROR;
        }
    }

    input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        printf("File open error\n");
        if (trace_file)
            fclose(trace_file);
        return FILE_OPEN_ERROR;
    }

    vectors_array = (Vector*)malloc(sizeof(Vector) * COUNT_VECTORS);
    if (!vectors_array)
    {
        printf("Memory allocation error\n");
        fclose(input_file);
        if (trace_file)
            fclose(trace_file);
        return MEMORY_ERROR;
    }
    initialize_vector_array(vectors_array);

    while (!feof(input_file))
    {
        if (read_string_from_file(input_file, &current_line) != SUCCESS)
        {
            print_message("Error: memory issue while reading line\n", trace_file);
            fclose(input_file);
            delete_string(&current_line);
            free(vectors_array);
            if (trace_file)
                fclose(trace_file);
            return MEMORY_ERROR;
        }

        if (current_line.len == 0)
            break;

        if (strstr(current_line.arr, "READ"))
        {
            String input_value;
            input_value.arr = NULL;

            if (sscanf(current_line.arr, "READ(%c, %9[^)])%c", &vector_name, base_str, &separator) != 3 ||
                !isalpha(vector_name) || separator != ';' || string_to_long(base_str, &base, 10) != SUCCESS || base < 2 || base > 36)
            {
                print_message("READ command error\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return INPUT_ERROR;
            }
            printf("Enter value (%c): ", vector_name);
            String value_input;
            if (read_word(&value_input) != SUCCESS)
            {
                print_message("Error: memory issue while reading value\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return MEMORY_ERROR;
            }


            if (vectors_array[vector_name - 'A'].init)
            {
                convert_to_bit_string(vectors_array[vector_name - 'A'].value, base_str);
                print_message("READ Operation (%c): before: %s\n", trace_file, vector_name, base_str);
            }
            else
                print_message("READ Operation (%c): before: not initialized\n", trace_file, vector_name);

            if (read_vector(&vectors_array[vector_name - 'A'], value_input.arr, base) != SUCCESS)
            {
                print_message("READ command error: input issue\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                delete_string(&value_input);
                return INPUT_ERROR;
            }
            convert_to_bit_string(vectors_array[vector_name - 'A'].value, base_str);
            print_message("After: %s\n\n", trace_file, base_str);

            delete_string(&value_input);
        }
        else if (strstr(current_line.arr, "WRITE"))
        {
            char base_value_str[10];
            char output_value[128];
            long base_value;

            if (sscanf(current_line.arr, "WRITE(%c, %9[^)])%c", &vector_name, base_value_str, &separator) != 3 ||
                !isalpha(vector_name) || separator != ';')
            {
                print_message("WRITE command error\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return INPUT_ERROR;
            }

            // Checking if the base is correct
            if (string_to_long(base_value_str, &base_value, 10) != SUCCESS || base_value < 2 || base_value > 36)
            {
                print_message("WRITE command error: invalid base\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return INPUT_ERROR;
            }
            if (!vectors_array[vector_name - 'A'].init)
            {
                print_message("WRITE command error: vector missing\n\n", trace_file);
            }
            else
            {
                convert_to_base(vectors_array[vector_name - 'A'].value, base_value, output_value);
                print_message("WRITE Operation (%c, %ld) = %s\n\n", trace_file, vector_name, base_value, output_value);
                printf("%c = %s\n", vector_name, output_value);
            }
        }
        else if (current_line.len == 6 && current_line.arr[3] == '\\')
        {
            char result_name;
            if (sscanf(current_line.arr, "%c:=\\%c%c", &result_name, &vector_name, &separator) != 3 ||
                !isalpha(vector_name) || separator != ';' || !isalpha(result_name))
            {
                print_message("Negation command error\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return INPUT_ERROR;
            }

            if (!vectors_array[vector_name - 'A'].init)
            {
                print_message("Negation command error: vector not initialized\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return MISSING;
            }

            if (vectors_array[result_name - 'A'].init)
            {
                convert_to_bit_string(vectors_array[result_name - 'A'].value, base_str);
                print_message("Operation (%c) := \\(%c): before: %s\n", trace_file, result_name, vector_name, base_str);
            }
            else
                print_message("Operation (%c) := \\(%c): before: not initialized\n", trace_file, result_name, vector_name);

            vectors_array[result_name - 'A'].init = 1;
            vectors_array[result_name - 'A'].value = ~vectors_array[vector_name - 'A'].value;
            convert_to_bit_string(vectors_array[result_name - 'A'].value, base_str);
            print_message("After: %s\n\n", trace_file, base_str);
        }
        else
        {
            char vector1, vector2, result_vector, operator[4];
            if (sscanf(current_line.arr, "%c:=%c%2[^A-Z]%c%c", &result_vector, &vector1, operator, &vector2, &separator) != 5 ||
                !isalpha(result_vector) || !isalpha(vector1) || !isalpha(vector2) || separator != ';' ||
                (strcmp(operator, "+") && strcmp(operator, "&") && strcmp(operator, "->") && strcmp(operator, "<-") && strcmp(operator, "~") &&
                    strcmp(operator, "<>") && strcmp(operator, "+>") && strcmp(operator, "!") && strcmp(operator, "?")))
            {
                print_message("Error: incorrect command\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return INPUT_ERROR;
            }

            if (!vectors_array[vector1 - 'A'].init || !vectors_array[vector2 - 'A'].init)
            {
                print_message("Error: one of the vectors not initialized\n", trace_file);
                fclose(input_file);
                delete_string(&current_line);
                free(vectors_array);
                if (trace_file)
                    fclose(trace_file);
                return INPUT_ERROR;
            }

            if (vectors_array[result_vector - 'A'].init)
            {
                convert_to_bit_string(vectors_array[result_vector - 'A'].value, base_str);
                print_message("Operation (%c) := (%c) %s (%c): before: %s\n", trace_file, result_vector, vector1, operator, vector2, base_str);
            }
            else
                print_message("Operation (%c) := (%c) %s (%c): before: not initialized\n", trace_file, result_vector, vector1, operator, vector2);
        }
    }

    fclose(input_file);
    if (trace_file)
        fclose(trace_file);
    free(vectors_array);
    return SUCCESS;
}

