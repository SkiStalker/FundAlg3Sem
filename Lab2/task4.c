#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

typedef enum
{
    SUCCESS,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_ARGUMENTS,
    ERROR_FILE_OPEN,
    ERROR_NOT_KAPREKAR
} status_code;

double cross_product(double x1, double y1, double x2, double y2)
{
    return x1 * y2 - y1 * x2;
}

status_code is_convex_polygon(int vertex_count, ...)
{
    if (vertex_count < 3)
        return ERROR_INVALID_ARGUMENTS;

    va_list args;
    va_start(args, vertex_count);

    double *x_coords = malloc(vertex_count * sizeof(double));
    double *y_coords = malloc(vertex_count * sizeof(double));

    if (!x_coords || !y_coords)
    {
        free(x_coords);
        free(y_coords);
        va_end(args);
        return ERROR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < vertex_count; i++)
    {
        x_coords[i] = va_arg(args, double);
        y_coords[i] = va_arg(args, double);
    }
    va_end(args);

    int is_convex = 1;
    double initial_cross_product = 0;

    for (int i = 0; i < vertex_count; i++)
    {
        double dx1 = x_coords[(i + 1) % vertex_count] - x_coords[i];
        double dy1 = y_coords[(i + 1) % vertex_count] - y_coords[i];
        double dx2 = x_coords[(i + 2) % vertex_count] - x_coords[(i + 1) % vertex_count];
        double dy2 = y_coords[(i + 2) % vertex_count] - y_coords[(i + 1) % vertex_count];

        double cross = cross_product(dx1, dy1, dx2, dy2);

        if (i == 0)
            initial_cross_product = cross;
        else if (cross * initial_cross_product < 0)
        {
            is_convex = 0;
            break;
        }
    }

    free(x_coords);
    free(y_coords);

    return is_convex ? SUCCESS : ERROR_INVALID_ARGUMENTS;
}

status_code evaluate_polynomial(double point, int degree, double *result, ...)
{
    if (degree < 0 || !result)
        return ERROR_INVALID_ARGUMENTS;

    double value = 0.0;
    va_list args;
    va_start(args, result);

    for (int i = 0; i <= degree; i++)
    {
        double coeff = va_arg(args, double);
        value += coeff * pow(point, degree - i);
    }

    va_end(args);
    *result = value;
    return SUCCESS;
}

status_code find_kaprekar_numbers(int base, status_code *result, int *count, ...)
{
    if (base < 2 || !result || !count)
        return ERROR_INVALID_ARGUMENTS;

    va_list args;
    va_start(args, count);
    *count = 0;

    char *number;
    while ((number = va_arg(args, char *)) != NULL)
    {
        int n = (int)strtol(number, NULL, base);
        int sq = n * n;
        char buf[40];
        sprintf(buf, "%d", sq);

        int len_num = strlen(number);
        int len_sq = strlen(buf);

        char left_buf[40] = {0};
        char right_buf[40] = {0};

        if (len_sq > len_num)
        {
            strncpy(left_buf, buf, len_sq - len_num);
            strcpy(right_buf, buf + (len_sq - len_num));
        }
        else
        {
            strcpy(right_buf, buf);
        }

        int left_part = (strlen(left_buf) > 0) ? strtol(left_buf, NULL, 10) : 0;
        int right_part = (strlen(right_buf) > 0) ? strtol(right_buf, NULL, 10) : 0;

        if (left_part + right_part == n)
        {
            (*count)++;
        }
    }

    va_end(args);
    *result = SUCCESS;
    return SUCCESS;
}

int main()
{
    printf("1. Let's check for the convexity of a polygon with points: (0;0), (1;0), (1;1), (0;1)\n"); // выпуклый
    status_code status1 = is_convex_polygon(4, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0);
    if (status1 == SUCCESS)
    {
        printf("The polygon is convex\n");
    }
    else
    {
        printf("The polygon is not convex or ERROR: %d\n", status1);
    }
    printf("1. Let's check for the convexity of a polygon with points: (0;0), (2;2), (4;0), (3;3), (1;3)\n"); // невыпуклый

    status_code status2 = is_convex_polygon(5, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0);
    if (status2 == SUCCESS)
    {
        printf("The polygon is convex\n\n");
    }
    else
    {
        printf("The polygon is not convex or ERROR: %d\n\n", status2);
    }

    printf("2. Let's calculate a polynomial of the form: 3x^3 + 2x^2 + x + 5 at the point 2\n"); // 39
    double polynomial_value3;
    status_code status3 = evaluate_polynomial(2.0, 3, &polynomial_value3, 3.0, 2.0, 1.0, 5.0);
    if (status3 == SUCCESS)
    {
        printf("The value of the polynomial: %lf\n", polynomial_value3);
    }
    else
    {
        printf("ERROR in calculating the polynomial: %d\n", status3);
    }

    printf("2. Let's calculate a polynomial of the form: -x^2 + 4x - 7 at the point -1\n"); // -12
    double polynomial_value4;
    status_code status4 = evaluate_polynomial(-1.0, 2, &polynomial_value4, -1.0, 4.0, -7.0);
    if (status4 == SUCCESS)
    {
        printf("The value of the polynomial: %lf\n\n", polynomial_value4);
    }
    else
    {
        printf("ERROR in calculating the polynomial: %d\n\n", status4);
    }

    printf("3. Let's find the Kaprekar numbers in the sequence (1, 9, 23, 45, 297) with the base 10\n"); // 3
    int count5;
    status_code kaprekar_status5;
    kaprekar_status5 = find_kaprekar_numbers(10, &kaprekar_status5, &count5, "9", "45", "297", NULL);
    if (kaprekar_status5 == SUCCESS)
    {
        printf("Found %d Kaprekar numbers\n", count5);
    }
    else
    {
        printf("ERROR checking Kaprekar numbers: %d\n", kaprekar_status5);
    }

    printf("3. Let's find the Kaprekar numbers in the sequence (88, 55, 34, A) with the base 17\n");
    int count6;
    status_code kaprekar_status6;
    kaprekar_status6 = find_kaprekar_numbers(17, &kaprekar_status6, &count6, "88", "55", "34", "A", NULL); // 2
    if (kaprekar_status6 == SUCCESS)
    {
        printf("Found %d Kaprekar numbers\n", count6);
    }
    else
    {
        printf("ERROR checking Kaprekar numbers: %d\n", kaprekar_status6);
    }

    return 0;
}