#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>

typedef enum ErrorCode
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    MISSING,
    FILE_OPEN_ERROR,
    DIVISION_BY_ZERO_ERROR
} ErrorCode;

typedef struct String
{
    char* arr;
    size_t capacity;
    size_t len;
} String;

typedef struct MemoryCell
{
    String name;
    int value;
} MemoryCell;

typedef struct Variables
{
    size_t capacity;
    size_t size;
    MemoryCell* array;
} Variables;

ErrorCode execute_expression(String* expression, Variables* vars);
ErrorCode init_variables(Variables* vars);
ErrorCode print_var(const char* var_name, Variables* vars);
ErrorCode print_all(Variables* vars);
void delete_variables(Variables* vars);
ErrorCode read_string(FILE* input, String* str);
ErrorCode delete_string_content(String* string);
ErrorCode add_char_to_string(String* str, char ch);
ErrorCode string_init(String* string);

ErrorCode string_to_int(const char* str, int* result)
{
    char* endinp;
    long res;
    res = strtol(str, &endinp, 10);
    if (res > INT_MAX || res < INT_MIN)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    *result = res;
    return SUCCESS;
}

ErrorCode string_to_size_t(const char* str, size_t* result)
{
    char* endinp;
    *result = (size_t)strtoul(str, &endinp, 10);
    if (*result == ULONG_MAX || *endinp != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

ErrorCode string_init(String* string)
{
    if (!string)
        return MEMORY_ERROR;
    string->capacity = 1;
    string->len = 0;
    if (!(string->arr = (char*)malloc(sizeof(char))))
        return MEMORY_ERROR;
    string->arr[0] = '\0';
    return SUCCESS;
}

ErrorCode delete_string_content(String* string)
{
    if (!string)
        return MEMORY_ERROR;
    free(string->arr);
    string->arr = NULL;
    string->len = 0;
    string->capacity = 0;
    return SUCCESS;
}

int string_compare(const String* str_1, const String* str_2)
{
    if (str_1->len != str_2->len)
        return str_1->len - str_2->len;
    return strcmp(str_1->arr, str_2->arr);
}

int string_equivalence_relation(const String* str_1, const String* str_2)
{
    return strcmp(str_1->arr, str_2->arr) == 0;
}

ErrorCode string_dup(String* dest, const String* sourse)
{
    if (!dest || !sourse)
        return MEMORY_ERROR;

    dest->capacity = sourse->capacity;
    dest->len = sourse->len;
    if (!sourse->arr)
    {
        dest->arr = NULL;
        return SUCCESS;
    }
    dest->arr = (char*)malloc(sourse->capacity * sizeof(char));
    if (!dest->arr)
        return MEMORY_ERROR;
    strcpy(dest->arr, sourse->arr);
    return SUCCESS;
}

String* string_destroy(String* str)
{
    if (!str)
        return NULL;
    free(str->arr);
    free(str);
    return NULL;
}

ErrorCode add_char_to_string(String* str, char ch)
{
    if (!str)
        return MEMORY_ERROR;
    if (str->capacity == 0)
    {
        if (string_init(str))
            return MEMORY_ERROR;
    }
    if (str->capacity == str->len + 1)
    {
        char* for_realloc;
        str->capacity *= 2;
        for_realloc = (char*)realloc(str->arr, str->capacity * sizeof(char));
        if (!for_realloc)
            return MEMORY_ERROR;
        str->arr = for_realloc;
    }
    str->arr[str->len++] = ch;
    str->arr[str->len] = '\0';
    return SUCCESS;
}

ErrorCode read_word(FILE* input, String* str)
{
    char ch;
    if (!input || !str)
        return MEMORY_ERROR;

    if (string_init(str) == MEMORY_ERROR)
        return MEMORY_ERROR;

    ch = getc(input);
    while (ch != EOF && isalnum(ch))
    {
        if (add_char_to_string(str, ch))
            return MEMORY_ERROR;
        ch = getc(input);
    }
    return SUCCESS;
}

ErrorCode read_string(FILE* input, String* str)
{
    char ch;
    if (!input || !str)
        return MEMORY_ERROR;

    if (string_init(str) == MEMORY_ERROR)
        return MEMORY_ERROR;

    ch = getc(input);

    while (ch != EOF && ch != ';')
    {
        if (!isspace(ch))
        {
            if (add_char_to_string(str, ch))
                return MEMORY_ERROR;
        }
        ch = getc(input);
    }
    return SUCCESS;
}

MemoryCell* search_variable(Variables* vars, const char* name)
{
    if (!vars || !name)
        return NULL;

    int left = 0;
    int right = vars->size - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int res = strcmp(vars->array[mid].name.arr, name);
        if (res == 0)
        {
            return &vars->array[mid];
        }
        if (res < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return NULL;
}

void delete_memory_cell(MemoryCell* mc)
{
    if (!mc)
        return;

    delete_string_content(&mc->name);
    mc->value = 0;
}

void delete_variables(Variables* vars)
{
    size_t i;
    if (!vars)
        return;

    for (i = 0; i < vars->size; ++i)
    {
        delete_memory_cell(&vars->array[i]);
    }
    free(vars->array);
    vars->size = 0;
    vars->capacity = 0;
    vars->array = NULL;
}

ErrorCode execute_operation(int var_1, int var_2, char op, int* res)
{
    if (!res) {
        return MEMORY_ERROR;
    }
    if (op == '+')
    {
        if (var_1 > 0 && INT_MAX - var_1 < var_2)
            return OVERFLOW_ERROR;
        else if (var_1 < 0 && INT_MIN - var_1 > var_2)
            return OVERFLOW_ERROR;

        *res = var_1 + var_2;
    }
    else if (op == '-')
    {
        var_2 *= -1;
        if (var_1 > 0 && INT_MAX - var_1 < var_2)
            return OVERFLOW_ERROR;
        else if (var_1 < 0 && INT_MIN - var_1 > var_2)
            return OVERFLOW_ERROR;

        *res = var_1 + var_2;
    }
    else if (op == '*')
    {
        *res = var_1 * var_2;
        if (var_1 != 0 && *res / var_1 != var_2)
            return OVERFLOW_ERROR;
    }
    else if (op == '/')
    {
        if (var_2 == 0)
            return DIVISION_BY_ZERO_ERROR;
        *res = var_1 / var_2;
    }

    else if (op == '%')
    {
        if (var_2 == 0)
            return DIVISION_BY_ZERO_ERROR;
        *res = var_1 % var_2;
    }

    return SUCCESS;
}

int compare(const void* a, const void* b)
{
    MemoryCell* mc1 = (MemoryCell*)a;
    MemoryCell* mc2 = (MemoryCell*)b;
    return strcmp(mc1->name.arr, mc2->name.arr);
}

ErrorCode add_variable(Variables* vars, MemoryCell* var)
{
    if (!vars || !var)
        return MEMORY_ERROR;

    if (vars->capacity == vars->size)
    {
        MemoryCell* for_realloc;
        vars->capacity *= 2;
        for_realloc = (MemoryCell*)realloc(vars->array, vars->capacity * sizeof(MemoryCell));
        if (!for_realloc)
            return MEMORY_ERROR;

        vars->array = for_realloc;
    }
    if (string_dup(&vars->array[vars->size].name, &var->name))
        return MEMORY_ERROR;
    vars->array[vars->size++].value = var->value;

    qsort(vars->array, vars->size, sizeof(MemoryCell), compare);
    return SUCCESS;
}

ErrorCode execute_expression(String* expression, Variables* vars)
{
    String var_1, var_2;
    MemoryCell result, * mc_var1, * mc_var2;
    size_t idx = 0;
    ErrorCode state;
    int var_1_num, var_2_num;
    char operation;
    var_2.arr = NULL;

    if (!expression || !vars)
        return MEMORY_ERROR;

    if (string_init(&result.name))
        return MEMORY_ERROR;

    while (isalpha(expression->arr[idx]))
    {
        if (add_char_to_string(&result.name, expression->arr[idx++]))
        {
            delete_string_content(&result.name);
            delete_memory_cell(&result);
            return MEMORY_ERROR;
        }
    }
    idx++;

    if (string_init(&var_1))
    {
        delete_string_content(&result.name);
        delete_memory_cell(&result);
        return MEMORY_ERROR;
    }
    while (isalnum(expression->arr[idx]))
    {
        if (add_char_to_string(&var_1, expression->arr[idx++]))
        {
            delete_string_content(&var_1);
            delete_memory_cell(&result);
            return MEMORY_ERROR;
        }
    }

    if (isdigit(var_1.arr[0]))
    {
        state = string_to_int(var_1.arr, &var_1_num);
        if (state != SUCCESS)
        {
            delete_memory_cell(&result);
            delete_string_content(&var_1);
            return state;
        }
    }
    else
    {
        mc_var1 = search_variable(vars, var_1.arr);
        if (!mc_var1)
        {
            delete_string_content(&var_1);
            delete_memory_cell(&result);
            return INPUT_ERROR;
        }
        var_1_num = mc_var1->value;
    }
    result.value = var_1_num;

    if (expression->arr[idx] != '\0')
    {
        operation = expression->arr[idx++];

        if (string_init(&var_2))
        {
            delete_memory_cell(&result);
            delete_string_content(&var_1);
            return MEMORY_ERROR;
        }
        while (isalnum(expression->arr[idx]))
        {
            if (add_char_to_string(&var_2, expression->arr[idx++]))
            {
                delete_memory_cell(&result);
                delete_string_content(&var_1);
                delete_string_content(&var_2);
                return MEMORY_ERROR;
            }
        }
        if (isdigit(var_2.arr[0]))
        {
            state = string_to_int(var_2.arr, &var_2_num);
            if (state != SUCCESS)
            {
                delete_memory_cell(&result);
                delete_string_content(&var_1);
                delete_string_content(&var_2);
                return state;
            }
        }
        else
        {
            mc_var2 = search_variable(vars, var_2.arr);
            if (!mc_var2)
            {
                delete_string_content(&var_1);
                delete_string_content(&var_2);
                delete_memory_cell(&result);
                return INPUT_ERROR;
            }
            var_2_num = mc_var2->value;
        }
        if ((state = execute_operation(var_1_num, var_2_num, operation, &result.value)) != SUCCESS)
        {
            delete_string_content(&var_1);
            delete_string_content(&var_2);
            delete_memory_cell(&result);
            return state;
        }
    }
    mc_var1 = search_variable(vars, result.name.arr);
    if (!mc_var1)
    {
        if (add_variable(vars, &result))
        {
            delete_string_content(&var_1);
            delete_string_content(&var_2);
            delete_memory_cell(&result);
            return MEMORY_ERROR;
        }
    }
    else
    {
        mc_var1->value = result.value;
    }
    delete_memory_cell(&result);
    delete_string_content(&var_1);
    delete_string_content(&var_2);
    return SUCCESS;
}


ErrorCode init_variables(Variables* vars)
{
    if (!vars)
        return MEMORY_ERROR;

    vars->array = (MemoryCell*)malloc(sizeof(MemoryCell) * 2);
    if (!vars->array)
        return MEMORY_ERROR;

    vars->capacity = 2;
    vars->size = 0;
    return SUCCESS;
}

ErrorCode print_var(const char* var_name, Variables* vars)
{
    MemoryCell* var;
    if (!vars || !var_name)
        return MEMORY_ERROR;

    var = search_variable(vars, var_name);
    if (!var)
        return INPUT_ERROR;
    printf("%d\n", var->value);
    return SUCCESS;
}

ErrorCode print_all(Variables* vars)
{
    size_t i;
    if (!vars)
        return MEMORY_ERROR;

    for (i = 0; i < vars->size; ++i)
    {
        printf("%s = %d; ", vars->array[i].name.arr, vars->array[i].value);
    }
    return SUCCESS;
}



int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Error: Invalid number of arguments.\n");
        return INPUT_ERROR;
    }

    const char* input_file = argv[1];
    Variables vars;
    FILE* input;
    String line;
    ErrorCode err = SUCCESS;
    size_t number_of_string = 0;

    input = fopen(input_file, "r");
    if (!input)
    {
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }

    if (init_variables(&vars) == MEMORY_ERROR)
    {
        printf("Memory error\n");
        fclose(input);
        return MEMORY_ERROR;
    }

    while (!feof(input))
    {
        number_of_string++;
        err = read_string(input, &line);
        if (err != SUCCESS)
        {
            break;
        }

        if (line.len == 0)
            continue;

        if (strcmp(line.arr, "printall") == 0)
        {
            printf("print all: ");
            print_all(&vars);
            putchar('\n');
        }
        else if (strstr(line.arr, "print") == line.arr)
        {
            String var_name;
            err = string_init(&var_name);
            if (err != SUCCESS)
                break;

            for (size_t i = 5; i < line.len; ++i)
            {
                err = add_char_to_string(&var_name, line.arr[i]);
                if (err != SUCCESS)
                {
                    delete_string_content(&var_name);
                    break;
                }
            }

            if (err != SUCCESS)
            {
                delete_string_content(&var_name);
                break;
            }
            err = print_var(var_name.arr, &vars);
            if (err != SUCCESS)
            {
                delete_string_content(&var_name);
                break;
            }
            delete_string_content(&var_name);
        }
        else
        {
            err = execute_expression(&line, &vars);
            if (err != SUCCESS)
                break;
        }
        delete_string_content(&line);
    }

    delete_string_content(&line);
    delete_variables(&vars);
    fclose(input);

    if (err == MEMORY_ERROR)
    {
        printf("Memory error. Line: %zu\n", number_of_string);
        return MEMORY_ERROR;
    }
    if (err == INPUT_ERROR)
    {
        printf("Input error. Line: %zu\n", number_of_string);
        return INPUT_ERROR;
    }
    if (err == OVERFLOW_ERROR)
    {
        printf("Overflow error. Line: %zu\n", number_of_string);
        return OVERFLOW_ERROR;
    }
    if (err == DIVISION_BY_ZERO_ERROR)
    {
        printf("Division by zero. Line: %zu\n", number_of_string);
        return DIVISION_BY_ZERO_ERROR;
    }

    return SUCCESS;
}
