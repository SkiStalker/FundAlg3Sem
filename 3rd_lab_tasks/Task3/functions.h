#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#define EPS 1e-10
typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    FILE_OPEN_ERROR
} status;

typedef struct Employee
{
    unsigned int id;
    char name[64];
    char surname[64];
    double salary;
} Employee;

status validate_input(int argc, char **argv)
{
    if (argc != 4 || (argv[2][0] != '/' && argv[2][0] != '-') || (argv[2][1] != 'a' && argv[2][1] != 'd') || argv[2][2] != '\0')
        return INPUT_ERROR;
    char full_path_1[BUFSIZ], full_path_2[BUFSIZ];
    if (!realpath(argv[1], full_path_1) || !realpath(argv[3], full_path_2))
        return INPUT_ERROR;
    if (!strcmp(full_path_1, full_path_2))
        return INPUT_ERROR;
    return SUCCESS;
}

status validate_data(Employee data)
{
    int len = strlen(data.name), i;
    for (i = 0; i < len; ++i)
    {
        if (!((data.name[i] >= 'a' && data.name[i] <= 'z') || (data.name[i] >= 'A' && data.name[i] <= 'Z')))
            return INPUT_ERROR;
    }
    len = strlen(data.surname);
    for (i = 0; i < len; ++i)
    {
        if (!((data.surname[i] >= 'a' && data.surname[i] <= 'z') || (data.surname[i] >= 'A' && data.surname[i] <= 'Z')))
            return INPUT_ERROR;
    }
    if (data.salary < EPS)
    {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

status scan_employee_from_file(Employee **out_data, FILE *in, int *size_data)
{
    int capacity = 2, count_read;
    *size_data = 0;
    if (!out_data)
        return INPUT_ERROR;
    *out_data = NULL;

    Employee *data = (Employee *)malloc(sizeof(Employee) * capacity);
    Employee *for_realloc;
    if (!data)
        return MEMORY_ERROR;

    while ((count_read = fscanf(in, "%u%s%s%lf\n", &data[*size_data].id, data[*size_data].name, data[*size_data].surname, &data[*size_data].salary)) == 4)
    {
        if (validate_data(data[(*size_data)]))
        {
            free(data);
            return INPUT_ERROR;
        }
        (*size_data)++;
        if (*size_data == capacity)
        {
            capacity *= 2;
            for_realloc = (Employee *)realloc(data, capacity * sizeof(Employee));
            if (!for_realloc)
            {
                free(data);
                return MEMORY_ERROR;
            }
            data = for_realloc;
        }
    }

    if (count_read < 4 && count_read > 0)
    {
        free(data);
        return INPUT_ERROR;
    }
    *out_data = data;
    return SUCCESS;
}

int compare_a(const void *a, const void *b)
{
    const Employee *emp_a = (const Employee *)a;
    const Employee *emp_b = (const Employee *)b;
    int temp;

    if (emp_a->salary - emp_b->salary > EPS)
    {
        return 1;
    }
    if (emp_b->salary - emp_a->salary > EPS)
    {
        return -1;
    }

    temp = strcmp(emp_a->surname, emp_b->surname);
    if (temp)
        return temp;

    temp = strcmp(emp_a->name, emp_b->name);
    if (temp)
        return temp;

    return emp_a->id - emp_b->id;
}

int compare_d(const void *a, const void *b)
{
    return compare_a(a, b) * -1;
}