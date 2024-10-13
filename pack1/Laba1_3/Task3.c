#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

enum return_type {
    FLAG_ERROR = -1000,
    SUCCESS = 0,
    BREAK,
    CONTINUE,
    ERROR,
    MEMORY_ERROR,
    ZERO_DIVISION_ERROR,
    MULTIPLE,
    N_MULTIPLE,
    RECTANGLE,
    N_RECTANGLE,
    MUST_BE_A_NUMBER_ERROR,
    MUST_BE_A_NUMBER_ERROR_BREAK,
    MUST_BE_BIGGER_THAN_ZERO
};

typedef int (*callback)(double*);

int has_only_numbers(char* argument) {
    for (int i = 0; i < strlen(argument); i++) {
        if (!((argument[i] >= '0' && argument[i] <= '9') || argument[i] == '.' || argument[i] == '-')) {
            return 0;
        }
    }
    return 1;
}

int find_flag(char* argument, const char** flags, int size) {
    for (int i = 0; i < size; i++) {
        if (!strcmp(argument, flags[i])) {
            return argument[1];
        }
    }
    return FLAG_ERROR;
}

int double_compare(double epsilon, double value1, double value2) {
    if (fabs(value1 - value2) < epsilon) {
        return 1;
    }
    else {
        return 0;
    }
}

int solve_quadratic(double epsilon, double a, double b, double c) {
    if (a == 0 && b != 0) {
        printf("%fx^2 + %fx + %f = 0\nx1 = x2 = %f\n\n", a, b, c, -c / b);
        return SUCCESS;
    }
    else if (a == 0 && b == 0 && c == 0) {
        printf("%fx^2 + %fx + %f = 0\nNo solutions!\n\n", a, b, c);
        return SUCCESS;
    }
    double discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) {
        discriminant = -discriminant;
        printf("%fx^2 + %fx + %f = 0\nNo solutions in R! Only in C.\nx1/x2 = (-%f +- i(sqrt(%f))) / (2 * %f)\n\n", a, b, c, b, discriminant, a);
        return SUCCESS;
    }
    double x1 = (-b + sqrt(discriminant)) / (2 * a);
    double x2 = (-b - sqrt(discriminant)) / (2 * a);
    if (fabs(x1 - x2) < epsilon) {
        printf("%fx^2 + %fx + %f = 0\nx1 = x2 = %f\n\n", a, b, c, x1);
        return SUCCESS;
    }
    printf("%fx^2 + %fx + %f = 0\nx1 = %f, x2 = %f\n\n", a, b, c, x1, x2);
    return SUCCESS;
}

int process_quadratic(double epsilon, double a, double b, double c) {
    int coefficients[] = { a, b, c };
    double combinations[6][3];
    double current_combination[3];
    int count = 0;
    for (int k = 0; k < 3; k++) {
        for (int i = 1; i <= 2; i++) {
            for (int j = k, n = 0; j < 3, n < 3; j = (j + i) % 3, n++) {
                current_combination[n] = coefficients[j % 3];
            }
            int flag = 0;
            for (int i = 0; i < count; i++) {
                for (int j = 0; j < 3; j++) {
                    if (double_compare(epsilon, current_combination[j], combinations[i][j])) {
                        flag++;
                    }
                }
                if (flag == 3) {
                    flag = -1;
                    break;
                }
                else {
                    flag = 0;
                }
            }
            if (!flag) {
                solve_quadratic(epsilon, current_combination[0], current_combination[1], current_combination[2]);
                for (int i = 0; i < 3; i++) {
                    combinations[count][i] = current_combination[i];
                }
                count++;
            }
            else {
                break;
            }
        }
    }
    return SUCCESS;
}

int check_multiple(int number1, int number2) {
    if (number2 == 0) {
        return ZERO_DIVISION_ERROR;
    }
    if (number1 % number2 == 0) {
        return MULTIPLE;
    }
    else {
        return N_MULTIPLE;
    }
}

int check_rectangular(double epsilon, double side1, double side2, double side3) {
    if (double_compare(epsilon, 0.0, side1) || double_compare(epsilon, 0.0, side2) || double_compare(epsilon, 0.0, side3) || side1 < 0 || side2 < 0 || side3 < 0) {
        return MUST_BE_BIGGER_THAN_ZERO;
    }
    if (double_compare(epsilon, side1 * side1 + side2 * side2, side3 * side3) || double_compare(epsilon, side1 * side1 + side3 * side3, side2 * side2) || double_compare(epsilon, side2 * side2 + side3 * side3, side1 * side1)) {
        return RECTANGLE;
    }
    else {
        return N_RECTANGLE;
    }
}

double* validate_arguments(const char** flags, char* argv[], int argc, int* current_index, int expected_count, int* return_status, int flag_count) {
    int next_index = (*current_index) + 1;
    int counter = 0;
    (*return_status) = SUCCESS;

    while (next_index < argc) {
        if (find_flag(argv[next_index], flags, flag_count) == FLAG_ERROR) {
            ++counter;
            ++next_index;
            continue;
        }
        else {
            break;
        }
        ++counter;
        ++next_index;
    }

    if (counter != expected_count) {
        printf("Incorrect number of arguments! Must be %d for %s key!\n", expected_count, argv[(*current_index)]);
        if ((*current_index) + counter < argc) {
            (*current_index) = (*current_index) + counter;
        }
        else {
            (*return_status) = BREAK;
            return NULL;
        }
        (*return_status) = CONTINUE;
        return NULL;
    }

    (*current_index)++;
    double* arguments = (double*)malloc(sizeof(double) * expected_count);
    if (arguments == NULL) {
        (*return_status) = ERROR;
        return NULL;
    }

    int flag = 0;
    for (int j = (*current_index); j < (*current_index) + expected_count; j++) {
        if (has_only_numbers(argv[j])) {
            arguments[j - (*current_index)] = atof(argv[j]);
        }
        else {
            (*return_status) = MUST_BE_A_NUMBER_ERROR_BREAK;
            flag = 1;
            break;
        }
    }

    if (flag) {
        (*current_index) = (*current_index) + expected_count - 1;
        return NULL;
    }

    (*current_index) = (*current_index) + expected_count - 1;
    return arguments;
}

int main(int argc, char* argv[]) {
    const char* flags[] = { "-q", "/q", "-m", "/m", "-t", "/t" };

    for (int i = 1; i < argc; i++) {
        int ret = find_flag(argv[i], flags, sizeof(flags) / sizeof(char*));
        if (ret == FLAG_ERROR) {
            printf("NOT EXISTING FLAG - %s\n", argv[i]);
            return -1;
        }
        else {
            if ((char)ret == 'q') {
                int return_status = SUCCESS;
                double* arguments = validate_arguments(flags, argv, argc, &i, 4, &return_status, sizeof(flags) / sizeof(char*));
                if (return_status == BREAK) {
                    free(arguments);
                    break;
                }
                else if (return_status == MUST_BE_A_NUMBER_ERROR_BREAK) {
                    printf("The argument must be a number!\n");
                    break;
                }
                else if (return_status == CONTINUE) {
                    free(arguments);
                    continue;
                }
                else if (return_status == ERROR) {
                    printf("Memory error!\n");
                }
                process_quadratic(arguments[0], arguments[1], arguments[2], arguments[3]);
                free(arguments);
            }
            else if ((char)ret == 'm') {
                int return_status = SUCCESS;
                double* arguments = validate_arguments(flags, argv, argc, &i, 2, &return_status, sizeof(flags) / sizeof(char*));
                if (return_status == BREAK) {
                    free(arguments);
                    break;
                }
                else if (return_status == MUST_BE_A_NUMBER_ERROR_BREAK) {
                    printf("The argument must be a number!\n");
                    break;
                }
                else if (return_status == CONTINUE) {
                    free(arguments);
                    continue;
                }
                else if (return_status == ERROR) {
                    printf("Memory error!\n");
                }
                int result = check_multiple((int)arguments[0], (int)arguments[1]);
                if (result == ZERO_DIVISION_ERROR) {
                    printf("Zero division error! The second argument mustn't be equal to 0!\n");
                }
                else if (result == MULTIPLE) {
                    printf("The first number is a multiple of the second.\n");
                }
                else if (result == N_MULTIPLE) {
                    printf("The first number is not a multiple of the second.\n");
                }
                free(arguments);
            }
            else if ((char)ret == 't') {
                int return_status = SUCCESS;
                double* arguments = validate_arguments(flags, argv, argc, &i, 4, &return_status, sizeof(flags) / sizeof(char*));
                if (return_status == BREAK) {
                    free(arguments);
                    break;
                }
                else if (return_status == MUST_BE_A_NUMBER_ERROR_BREAK) {
                    printf("The argument must be a number!\n");
                    break;
                }
                else if (return_status == CONTINUE) {
                    free(arguments);
                    continue;
                }
                else if (return_status == ERROR) {
                    printf("Memory error!\n");
                }
                int result = check_rectangular(arguments[0], arguments[1], arguments[2], arguments[3]);
                if (result == RECTANGLE) {
                    printf("It can be a rectangle.\n");
                }
                else if (result == N_RECTANGLE) {
                    printf("It can't be a rectangle.\n");
                }
                else if (result == MUST_BE_BIGGER_THAN_ZERO) {
                    printf("Sides must be bigger than 0\n");
                }
                free(arguments);
            }
            else {
                printf("Invalid flag encountered.\n");
                
            }
        }
    }
    return 0;
}