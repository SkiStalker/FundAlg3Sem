#include "functions.h"

int main(int argc, char **argv)
{
    int size_data;
    if (validate_input(argc, argv))
    {
        printf("You need to enter 3 arguments (file_in)(flag(-a /a -d /d))(file_out)\n");
        return INPUT_ERROR;
    }
    Employee *data = NULL;

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[3], "w");
    if (!input || !output)
    {
        if (!input)
            fclose(input);
        if (!output)
            fclose(output);
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }
    if (scan_employee_from_file(&data, input, &size_data) != SUCCESS)
    {
        printf("Error reading data from file\n");
        free(data);
        fclose(input);
        fclose(output);
        return INPUT_ERROR;
    }

    if (argv[2][1] == 'a' || argv[2][1] == 'A')
    {
        qsort(data, size_data, sizeof(Employee), compare_a);
    }
    else if (argv[2][1] == 'd' || argv[2][1] == 'D')
    {
        qsort(data, size_data, sizeof(Employee), compare_d);
    }

    for (int i = 0; i < size_data; i++)
    {
        fprintf(output, "%u %s %s %lf\n", data[i].id, data[i].name, data[i].surname, data[i].salary);
    }
    free(data);
    fclose(input);
    fclose(output);
    return SUCCESS;
}