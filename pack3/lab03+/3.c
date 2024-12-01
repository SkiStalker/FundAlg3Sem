#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum StatusCode {
    SUCCESS = 0,
    ERROR
} StatusCode;

typedef struct Employee {
    int id;
    char name[100];
    char surname[100];
    double salary;
} Employee;

int compare_ascending(const void* a, const void* b) {
    const Employee* emp1 = (const Employee*)a;
    const Employee* emp2 = (const Employee*)b;

    if (emp1->salary < emp2->salary) {
        return -1;
    }
    if (emp1->salary > emp2->salary) {
        return 1;
    }

    int surname_cmp = strcmp(emp1->surname, emp2->surname);
    if (surname_cmp != 0) {
        return surname_cmp;
    }

    int name_cmp = strcmp(emp1->name, emp2->name);
    if (name_cmp != 0) { 
        return name_cmp;
    }
    
    if (emp1->id < emp2->id) {
        return -1;
    }
    if (emp1->id > emp2->id) {
        return 1;
    }
    return 0;
}

int compare_descending(const void* a, const void* b) {
    return -compare_ascending(a, b);
}

StatusCode print_employees(const Employee* employees, size_t count) {
    for (size_t i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Surname: %s, Salary: %.2f\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].salary);
    }
    return SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Ошибка: неверное количество аргументов.\n");
        return ERROR;
    }

    const char* input_file = argv[1];
    const char* flag = argv[2];
    const char* output_file = argv[3];

    int ascending = 0;
    if (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0) {
        ascending = 1;
    } else if (strcmp(flag, "-d") == 0 || strcmp(flag, "/d") == 0) {
        ascending = 0;
    } else {
        printf("Ошибка: неверный флаг сортировки.\n");
        return ERROR;
    }

    FILE* infile = fopen(input_file, "r");
    if (infile == NULL) {
        perror("Ошибка при открытии входного файла");
        return ERROR;
    }

    Employee* employees = NULL;
    size_t employee_count = 0;
    size_t capacity = 10;
    employees = (Employee*)malloc(capacity * sizeof(Employee));
    if (employees == NULL) {
        perror("Ошибка выделения памяти");
        fclose(infile);
        return ERROR;
    }

    while (fscanf(infile, "%d %99s %99s %lf", &employees[employee_count].id, employees[employee_count].name, employees[employee_count].surname, &employees[employee_count].salary) == 4) {
        employee_count++;
        if (employee_count == capacity) {
            capacity *= 2;
            Employee* temp = realloc(employees, capacity * sizeof(Employee));
            if (temp == NULL) {
                perror("Ошибка выделения памяти");
                free(employees);
                fclose(infile);
                return ERROR;
            }
            employees = temp;
        }
    }
    fclose(infile);


    if (ascending) {
        qsort(employees, employee_count, sizeof(Employee), compare_ascending);
    } else {
        qsort(employees, employee_count, sizeof(Employee), compare_descending);
    }


    FILE* outfile = fopen(output_file, "w");
    if (outfile == NULL) {
        perror("Ошибка при открытии выходного файла");
        free(employees);
        return 1;
    }

    for (size_t i = 0; i < employee_count; i++) {
        fprintf(outfile, "%d %s %s %.2f\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].salary);
    }

    fclose(outfile);
    free(employees);

    return SUCCESS;
}
