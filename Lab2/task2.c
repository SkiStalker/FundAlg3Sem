#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

double average_geometric_value(int count, ...)
{
    if (count < 0)
    {
        printf("ERROR: The number of elements cannot be negative\n");
        return -1.0;
    }

    va_list numbers;
    va_start(numbers, count);

    double x = 1.0;
    for (int i = 0; i < count; ++i)
    {
        double value = va_arg(numbers, double);
        if (value < 0)
        {
            printf("ERROR: The number cannot be negative\n");
            return -1.0;
        }
        else if (value == 0)
        {
            return 0;
        }
        x *= value;
    }

    va_end(numbers);

    return pow(x, 1.0 / count);
}

double exponentiation_function_number(double base, int num)
{
    if (num == 0)
    {
        return 1;
    }
    else if (num < 0)
    {
        return 1.0 / exponentiation_function_number(base, -num);
    }
    else if (num % 2 == 0)
    {
        double current = exponentiation_function_number(base, num / 2);
        return current * current;
    }
    else
    {
        return base * exponentiation_function_number(base, num - 1);
    }
}

int main()
{
    printf("Calculate the geometric mean number: 8.0 16.0 32.0\n");
    double func_1 = average_geometric_value(3, 8.0, 16.0, 32.0);
    if (func_1 < 0)
    {
        return -1;
    }
    printf("The average geometric value: %f\n", func_1);

    double func_2 = exponentiation_function_number(8.0, 4);
    printf("Raising the number 8.0 to a power 4: %f\n", func_2);

    func_2 = exponentiation_function_number(2.0, -3);
    printf("Raising the number 2.0 to a power -3: %f\n", func_2);

    printf("Calculate the geometric mean number: 0\n");
    func_1 = average_geometric_value(1, 0, 0);
    if (func_1 < 0)
    {
        return -1;
    }
    printf("The average geometric value: %f\n", func_1);

    return 0;
}
