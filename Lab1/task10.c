#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BASE 36

long long convert_to_decimal(const char *number_string, int base)
{
    long long result = 0;
    int length = strlen(number_string);
    for (int i = 0; i < length; i++)
    {
        char digit = number_string[i];
        int value = 0;
        if (isdigit(digit))
        {
            value = digit - '0';
        }
        else if (isalpha(digit))
        {
            value = toupper(digit) - 'A' + 10;
        }
        result = result * base + value;
    }
    return result;
}

void convert_from_decimal(long long number, int base, char *output_string)
{
    char data[65];
    char *ptr = data + sizeof(data) - 1;
    *ptr = '\0';

    int flag = 0;
    if (number < 0)
    {
        number = -number;
        flag = 1;
    }

    do
    {
        int digit = number % base;
        *--ptr;
        if (digit < 10)
        {
            *ptr = '0' + digit;
        }
        else
        {
            *ptr = 'A' + digit - 10;
        }
        //*--ptr = (digit < 10) ? '0' + digit : 'A' + digit - 10;
        number /= base;
    } while (number > 0);

    if (flag == 1)
    {
        *--ptr = '-';
    }

    strcpy(output_string, ptr);
}

int main()
{
    int base;
    printf("Enter the number system from 2 to 36: ");

    if (scanf("%d", &base) != 1)
    {
        printf("ERROR: Invalid input\n");
        return -1;
    }

    if (base < 2 || base > MAX_BASE)
    {
        printf("ERROR: Invalid input\n");
        return -1;
    }

    char input_data[65];
    long long max_number = -1;
    char max_number_string[65] = {'\0'};

    printf("Enter the numbers in the same number system (Stop - to complete the input):\n");
    while (1)
    {
        scanf("%s", input_data);
        if (strcmp(input_data, "Stop") == 0)
        {
            break;
        }

        long long number = convert_to_decimal(input_data, base);
        if (llabs(number) > max_number)
        {
            max_number = llabs(number);
            strcpy(max_number_string, input_data);
        }
    }

    if (max_number_string[0] != '\0')
    {
        char converted_data[65];
        printf("The maximum modul number: %s\n", max_number_string);
        for (int target_base = 9; target_base <= 36; target_base += 9)
        {
            convert_from_decimal(max_number, target_base, converted_data);
            printf("In the base number system %d: %s\n", target_base, converted_data);
        }
    }
    else
    {
        printf("The numbers are missing\n");
    }

    return 0;
}