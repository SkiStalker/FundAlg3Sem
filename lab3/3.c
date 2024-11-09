#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char firstName[50];
    char lastName[50];
    double salary;
} Employee;

int compareAsc(const void *a, const void *b) {
    Employee *emp1 = (Employee *)a;
    Employee *emp2 = (Employee *)b;
    
    if (emp1->salary != emp2->salary)
        return (emp1->salary > emp2->salary) - (emp1->salary < emp2->salary);
    int lastNameCmp = strcmp(emp1->lastName, emp2->lastName);
    if (lastNameCmp != 0)
        return lastNameCmp;
    int firstNameCmp = strcmp(emp1->firstName, emp2->firstName);
    if (firstNameCmp != 0)
        return firstNameCmp;
    return emp1->id - emp2->id;
}

int compareDesc(const void *a, const void *b) {
    Employee *emp1 = (Employee *)a;
    Employee *emp2 = (Employee *)b;
    
    if (emp1->salary != emp2->salary)
        return (emp2->salary > emp1->salary) - (emp2->salary < emp1->salary);
    int lastNameCmp = strcmp(emp2->lastName, emp1->lastName);
    if (lastNameCmp != 0)
        return lastNameCmp;
    int firstNameCmp = strcmp(emp2->firstName, emp1->firstName);
    if (firstNameCmp != 0)
        return firstNameCmp;
    return emp2->id - emp1->id;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <-a/-d> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

    int ascending = (strcmp(argv[2], "-a") == 0 || strcmp(argv[2], "/a") == 0);
    if (!ascending && strcmp(argv[2], "-d") != 0 && strcmp(argv[2], "/d") != 0) {
        fprintf(stderr, "Invalid flag. Use -a, /a for ascending or -d, /d for descending.\n");
        fclose(inputFile);
        return 1;
    }

    Employee *employees = NULL;
    size_t count = 0;
    while (!feof(inputFile)) {
        Employee emp;
        if (fscanf(inputFile, "%d %49s %49s %lf", &emp.id, emp.firstName, emp.lastName, &emp.salary) == 4) {
            employees = realloc(employees, (count + 1) * sizeof(Employee));
            if (!employees) {
                perror("Memory allocation error");
                fclose(inputFile);
                return 1;
            }
            employees[count++] = emp;
        }
    }
    fclose(inputFile);

    qsort(employees, count, sizeof(Employee), ascending ? compareAsc : compareDesc);

    FILE *outputFile = fopen(argv[3], "w");
    if (!outputFile) {
        perror("Error opening output file");
        free(employees);
        return 1;
    }

    for (size_t i = 0; i < count; ++i) {
        fprintf(outputFile, "%d %s %s %.2f\n", employees[i].id, employees[i].firstName, employees[i].lastName, employees[i].salary);
    }

    fclose(outputFile);
    free(employees);
    return 0;
}
