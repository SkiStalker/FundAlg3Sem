#include "functions.h"

int main(int argc, char *argv[])
{
    int a, d, count, b, c;
    float f_1, f_2;
    unsigned int uns_int;

    puts("........................................OVERSSCANF TEST........................................\n");

    char str[20];

    puts("< Test_1 >");
    puts("String: 'This is string: ccc 123 1000 and 4555'");
    count = oversscanf("Thissvvv is string:ccc 123 1000 and 4555", "%5svvv is string :%Cv %d  %CV and%d", str, &a, 16, &b, &c, 10, &d);
    printf("Output: %d, %d, %d, %d, %s; Count scanned: %d\n\n\n", a, b, c, d, str, count);

    puts("< Test_2 >");
    puts("String: 'This is the number 10: 0010011'");
    count = oversscanf("This is the number 10: 0010011", "This is the number 10: %Zr", &uns_int);
    printf("Output: %u; Count scanned: %d\n\n\n", uns_int, count);

    puts("< Test_3 >");
    puts("String: 'This is the number 10 & 3954: X & MMMCMLIV'");
    count = oversscanf("This is the number 10 & 3954: X & MMMCMLIV", "This is the number 10 & 3954: %Ro & %Ro", &a, &b);
    printf("Output: %d %d; Count scanned: %d\n\n\n", a, b, count);

    puts("< Test_4 >");
    puts("String: 'This is float, float and Ro:  1.33, 9.23, XIX'");
    count = oversscanf("This is float, float and Ro:  1.33, 9.23, XIX", "This is float, float and Ro:%f, %f, %Ro", &f_1, &f_2, &a);
    printf("Output: %f %f %d; Count scanned: %d\n\n\n", f_1, f_2, a, count);

    puts("< Test_5 >");
    puts("String: '194394'");
    count = oversscanf("194394", "%Cv", &a, 100); // base = 100;
    printf("Output: %d; Count scanned: %d\n\n\n", a, count);

    puts("........................................OVERFSCANF TEST........................................\n");
    FILE *stream = fopen("test1.txt", "r");
    if (!stream)
        return FILE_OPEN_ERROR;

    puts("< Test_1 >");
    puts("String: 'This is string: ccc 123 1000 and 4555'");
    count = overfscanf(stream, "This is string :%Cv %d  %CV and%d", &a, 16, &b, &c, 10, &d);
    printf("Output: %d, %d, %d, %d; Count scanned: %d\n\n\n", a, b, c, d, count);
    fclose(stream);

    stream = fopen("test2.txt", "r");
    if (!stream)
        return FILE_OPEN_ERROR;
    puts("< Test_2 >");
    puts("String: 'This is the number 10: 0010011'");
    count = overfscanf(stream, "This is the number 10: %Zr", &uns_int);
    printf("Output: %u; Count scanned: %d\n\n\n", uns_int, count);
    fclose(stream);

    stream = fopen("test3.txt", "r");
    if (!stream)
        return FILE_OPEN_ERROR;
    puts("< Test_3 >");
    puts("String: 'This is the number 10 & 3954: X & MMMCMLIV'");
    count = overfscanf(stream, "This is the number 10 & 3954: %Ro & %Ro", &a, &b);
    printf("Output: %d %d; Count scanned: %d\n\n\n", a, b, count);
    fclose(stream);

    stream = fopen("test4.txt", "r");
    if (!stream)
        return FILE_OPEN_ERROR;
    puts("< Test_4 >");
    puts("String: 'This is float, float and Ro:  1.33, 9.23, XIX'");
    count = overfscanf(stream, "This is float, float and Ro:%f, %f, %Ro", &f_1, &f_2, &a);
    printf("Output: %f %f %d; Count scanned: %d\n\n\n", f_1, f_2, a, count);
    fclose(stream);

    stream = fopen("test5.txt", "r");
    if (!stream)
        return FILE_OPEN_ERROR;
    puts("< Test_5 >");
    puts("String: '194394'");
    count = overfscanf(stream, "%Cv", &a, 100); // base = 100;
    printf("Output: %d; Count scanned: %d\n\n\n", a, count);
    fclose(stream);

    return 0;
}