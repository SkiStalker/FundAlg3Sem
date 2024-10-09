#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


typedef enum StatusCode{
    SUCCESS = 0,
    IT_IS_ONE = 0,
    ERR_INVALID_INPUT = 1,
    ERR_OUT_OF_BOUNDS = 2,
    ERR_MEMORY_FAILURE = 3,
    ERR_UNKNOWN_FLAG = 4,
    ERR_X_TOO_LARGE = 5
} StatusCode;


StatusCode is_prime(int x, bool *result) {
    if (x < 1) {
        *result = false;
        return ERR_INVALID_INPUT;
    }
    else if(x == 1) {
        return IT_IS_ONE;
    }

    for (int i = 2; (i * i) <= x; i++) {
        if (x % i == 0) {
            *result = false;
            return SUCCESS;
        }
    }

    *result = true;
    return SUCCESS;
}


StatusCode find_multiples(int x, int *multiples, int *count) {
    if (x <= 0) {
        return ERR_INVALID_INPUT;
    }

    *count = 0;
    for (int i = 1; i <= 100; i++) {
        if (i % x == 0) {
            multiples[(*count)++] = i;
        }
    }

    if (*count == 0) {
        return ERR_OUT_OF_BOUNDS;
    }

    return SUCCESS;
}


StatusCode split_to_hex_digits(int x, char *hex_str) {
    if (x < 0) {
        return ERR_INVALID_INPUT;
    }

    sprintf(hex_str, "%X", x);
    return SUCCESS;
}


StatusCode generate_powers_table(int x, long result[10][10]) {
    if (x > 10) {
        return ERR_X_TOO_LARGE;
    }

    for (int base = 1; base <= 10; base++) {
        long value = 1; 
        for (int exp = 1; exp <= x; exp++) {
            value *= base; 
            result[base - 1][exp - 1] = value;
        }
    }

    return SUCCESS;
}


StatusCode sum_to_x(int x, int *sum) {
    if (x <= 0) {
        return ERR_INVALID_INPUT;
    }

    *sum = (x * (x + 1)) / 2;
    return SUCCESS;
}


StatusCode factorial(int x, long long *result) {
    if (x < 0) {
        return ERR_INVALID_INPUT;
    }

    *result = 1;

    for (int i = 1; i <= x; i++) {
        *result *= i;
    }

    return SUCCESS;
}


StatusCode validate_arguments(int argc, char *argv[], int *x, char **flag) {
    if (argc != 3) {
        return ERR_INVALID_INPUT;
    }

    *x = atoi(argv[1]);
    *flag = argv[2];

    if (*x < 0) {
        return ERR_INVALID_INPUT;
    }

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    int x;
    char *flag;
    StatusCode status = validate_arguments(argc, argv, &x, &flag);

    if (status != SUCCESS) {
        printf("Ошибка: Неверные аргументы.\n");
        return status;
    }

    if (strcmp(flag, "-h") == 0) {
        int multiples[100];
        int count;
        status = find_multiples(x, multiples, &count);

        if (status == SUCCESS) {
            printf("Числа, кратные %d: ", x);
            for (int i = 0; i < count; i++) {
                printf("%d ", multiples[i]);
            }
            printf("\n");
        }
        else {
            printf("Нет чисел, кратных %d в пределах 100\n", x);
        }
    }
    else if (strcmp(flag, "-p") == 0) {
        bool is_prime_result;
        status = is_prime(x, &is_prime_result);
        if (status == SUCCESS) {
            if (is_prime_result) {
                printf("%d является простым числом\n", x);
            }
            else {
                printf("%d является составным числом\n", x);
            }
        }
        else if (status == IT_IS_ONE) {
            printf("%d не является ни простым, ни составным числом\n", x);
        }
    }
    else if (strcmp(flag, "-s") == 0) {
        char hex_str[20];
        status = split_to_hex_digits(x, hex_str);
        if (status == SUCCESS) {
            printf("Цифры в 16-ричной системе: ");
            for (int i = 0; hex_str[i] != '\0'; i++) {
                printf("%c ", hex_str[i]);
            }
            printf("\n");
        }
    }
    else if (strcmp(flag, "-e") == 0) {
        long powers[10][10];
        status = generate_powers_table(x, powers);
        if (status == SUCCESS) {
            for (int base = 0; base < 10; base++) {
                printf("Основание %d: ", base + 1);
                for (int exp = 0; exp < x; exp++) {
                    printf("%ld ", powers[base][exp]);
                }
                printf("\n");
            }
        }
        else {
            printf("Ошибка: x должен быть не больше 10.\n");
        }
    }
    else if (strcmp(flag, "-a") == 0) {
        int sum;
        status = sum_to_x(x, &sum);
        if (status == SUCCESS) {
            printf("Сумма чисел от 1 до %d: %d\n", x, sum);
        }
    }
    else if (strcmp(flag, "-f") == 0) {
        long long fact;
        status = factorial(x, &fact);
        if (status == SUCCESS) {
            printf("Факториал %d: %lld\n", x, fact);
        }
    }
    else {
        printf("Ошибка: Неизвестный флаг %s\n", flag);
        return ERR_UNKNOWN_FLAG;
    }

    return SUCCESS;
} 