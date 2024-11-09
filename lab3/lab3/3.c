#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char firstName[BUFSIZ];
    char lastName[BUFSIZ];
    double salary;
} Employee;

char* GetFileName(const char* filePath) {
    const char* fileName = strrchr(filePath, '\\');
    
    if (fileName == NULL) {
        return (char*)filePath;
    } else {
        return (char*)(fileName + 1);
    }
}

int IsInt(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

int IsString(const char* str) {
    if (str[0] == '\0') {
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

int IsDouble(const char* str) {
    char* endPtr;
    double val = strtod(str, &endPtr);
    return (*endPtr == '\0' && val >= 0);
}

int ReadEmployeesFromFile(const char *filePath, Employee **employees) {
    FILE *input = fopen(filePath, "r");
    if (!input) {
        printf("cannot open input file\n");
        return 0;
    }

    int count = 0;
    int capacity = 10;
    *employees = malloc(capacity * sizeof(Employee));
    if (!*employees) {
        perror("malloc error\n");
        fclose(input);
        return 0;
    }

    char idStr[BUFSIZ], firstName[BUFSIZ], lastName[BUFSIZ], salaryStr[BUFSIZ];

    // сначала считываю как строки для валидации
    while (fscanf(input, "%s %s %s %s", idStr, firstName, lastName, salaryStr) == 4) {
        if (count >= capacity) {
            capacity *= 2;
            Employee *temp = realloc(*employees, capacity * sizeof(Employee));
            if (!temp) {
                printf("realloc error\n");
                free(*employees);
                fclose(input);
                return 0;
            }
            *employees = temp;
        }

        if (!IsInt(idStr)) {
            printf("invalid id: %s\n", idStr);
            continue;
        }

        if (!IsString(firstName)) {
            printf("invalid first name: %s\n", firstName);
            continue;
        }

        if (!IsString(lastName)) {
            printf("invalid last name: %s\n", lastName);
            continue;
        }

        if (!IsDouble(salaryStr)) {
            printf("invalid salary: %s\n", salaryStr);
            continue;
        }

        // если всё валидно, то присваиваю эти значения полям
        (*employees)[count].id = atoi(idStr);
        strcpy((*employees)[count].firstName, firstName);
        strcpy((*employees)[count].lastName, lastName);
        (*employees)[count].salary = atof(salaryStr);
        count++;
    }

    fclose(input);
    return count;
}

void WriteEmployeesToFile(const char *filePath, Employee *employees, int count) {
    FILE *outputFile = fopen(filePath, "w");
    if (!outputFile) {
        printf("cannot open output file\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(outputFile, "%d %s %s %.2f\n", employees[i].id,
                employees[i].firstName, employees[i].lastName, employees[i].salary);
    }

    fclose(outputFile);
}

int CompareEmployeesAsc(const void *a, const void *b) {
    const Employee *empA = (const Employee *)a;
    const Employee *empB = (const Employee *)b;
    
    // пордок: зарплата > фамилия > имя > id
    if (empA->salary != empB->salary) {
        return (empA->salary < empB->salary) ? -1 : 1;
    }
    
    int lastNameCompare = strcmp(empA->lastName, empB->lastName);
    if (lastNameCompare != 0) {
        return lastNameCompare;
    }
    
    int firstNameCompare = strcmp(empA->firstName, empB->firstName);
    if (firstNameCompare != 0) {
        return firstNameCompare;
    }
    
    return empA->id - empB->id;
}

int CompareEmployeesDesc(const void *a, const void *b) {
    return -CompareEmployeesAsc(a, b); 
}

void SortEmployees(Employee *employees, int count, char flag) {
    if (flag == 'd') {
        qsort(employees, count, sizeof(Employee), CompareEmployeesDesc);
    } else {
        qsort(employees, count, sizeof(Employee), CompareEmployeesAsc);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("enter input file, flag and output file\n");
        return 1;
    }

    const char *input = argv[1];
    char flag = argv[2][1];  // только буква
    const char *output = argv[3];

    char* fileName1 = GetFileName(argv[1]);
    char* fileName2 = GetFileName(argv[3]);

    if (!strcmp(fileName1, fileName2)) {
        printf("input and output files cannot be the same\n");
        return 2;
    }

    if ((flag != 'a' && flag != 'd') || (argv[2][0] != '-' && argv[2][0] != '/')) {
        printf("invalid flag\n");
        return 2;
    }

    Employee *employees = NULL;
    int count = ReadEmployeesFromFile(input, &employees);
    if (count == 0) {  // пустой файл
        printf("cannot read employees\n");
        return 3;
    }

    SortEmployees(employees, count, flag);
    WriteEmployeesToFile(output, employees, count);  // номера 11-14 не валидные и выводятся отдельно в консоль

    free(employees);
    return 0;
}
