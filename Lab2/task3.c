#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum
{
    SUCCESS,
    ERROR_FILE_OPEN,
    ERROR_MEMORY_ALLOCATION,
    ERROR_SUBSTRING_NOT_FOUND,
} status_code;

status_code find_substring_in_files(const char *substring, int file_count, ...)
{
    if (!substring || file_count < 1)
    {
        return ERROR_SUBSTRING_NOT_FOUND;
    }

    va_list args;
    va_start(args, file_count);

    char *filename;
    status_code result = SUCCESS;

    for (int i = 0; i < file_count; ++i)
    {
        filename = va_arg(args, char *);

        FILE *file = fopen(filename, "r");
        if (!file)
        {
            // printf("ERROR: File %s could not be opened\n", filename);
            result = ERROR_FILE_OPEN;
            continue;
        }

        char *line = NULL;
        size_t len = 0;
        int line_number = 0;
        int found = 0;

        while (getline(&line, &len, file) != -1)
        {
            line_number++;
            char *ptr = line;

            while (*ptr != '\0')
            {
                if (*ptr == substring[0])
                {
                    int match = 1;
                    for (int j = 1; substring[j] != '\0'; j++)
                    {
                        if (ptr[j] != substring[j])
                        {
                            match = 0;
                            break;
                        }
                    }
                    if (match)
                    {
                        printf("Found in the file %s, line %d, position %ld\n", filename, line_number, ptr - line + 1);
                        found = 1;
                        break; // для первого вхождения
                    }
                }
                ptr++;
            }
        }

        free(line);
        fclose(file);

        if (!found)
        {
            result = ERROR_SUBSTRING_NOT_FOUND;
        }
    }
    va_end(args);

    return result;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("ERROR: Invalid input\n");
        return -1;
    }

    const char *substring = argv[1];
    status_code status = find_substring_in_files(substring, argc - 2, argv[2], argv[3]);

    switch (status)
    {
    case ERROR_FILE_OPEN:
        printf("ERROR: One or more files could not be opened\n");
        return -1;
    case ERROR_SUBSTRING_NOT_FOUND:
        printf("INFO: Substring was not found in the specified files\n");
        break;
    default:
        printf("Operation completed.\n");
        break;
    }

    return SUCCESS;
}