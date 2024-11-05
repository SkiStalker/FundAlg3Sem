#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char name[50];
	char surname[50];
	double salary;
}Employee;

typedef enum {
	SUCCESS,
	ERROR_FILE_NOT_FOUND,
	ERROR_INVALID_ARGUMENT,
	ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_FLAG
} ErrorCode;

ErrorCode parse_arg(int argc, char* argv[], char** input_file, char** output_file, char* sort_flag) {
	if (argc != 4) {
		return ERROR_INVALID_ARGUMENT;
	}
	*input_file = argv[1];
	*output_file = argv[3];
    if (argv[2][0] == '-' && (argv[2][1] == 'a' || argv[2][1] == 'd') && argv[2][2] == '\0') {
        *sort_flag = argv[2][1];
    }
    else {
        return ERROR_INVALID_FLAG;
    }

    return SUCCESS;
}

ErrorCode loadEmployees(const char* filename, Employee** employees, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    int capacity = 10;
    *employees = malloc(capacity * sizeof(Employee));
    if (!*employees) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }

    *count = 0;
    while (fscanf(file, "%d %s %s %lf", &(*employees)[*count].id, (*employees)[*count].name,
        (*employees)[*count].surname, &(*employees)[*count].salary) == 4) {
        (*count)++;
        if (*count >= capacity) {
            capacity *= 2;
            *employees = realloc(*employees, capacity * sizeof(Employee));
            if (!*employees) {
                fclose(file);
                return ERROR_MEMORY_ALLOCATION;
            }
        }
    }

    fclose(file);
    return SUCCESS;
}


int compareAsc(const void* a, const void* b) {
    Employee* empA = (Employee*)a;
    Employee* empB = (Employee*)b;

    if (empA->salary < empB->salary) {
        return -1;
    }
    if (empA->salary > empB->salary) {
        return 1;
    }
    int surnameComp = strcmp(empA->surname, empB->surname);
    if (surnameComp != 0) {
        return surnameComp;
    }
    return strcmp(empA->name, empB->name);
}

int compareDesc(const void* a, const void* b) {
    return compareAsc(b, a);
}


void sortEmployees(Employee* employees, int count, char sortFlag) {
    if (sortFlag == 'a') {
        qsort(employees, count, sizeof(Employee), compareAsc);
    }
    else {
        qsort(employees, count, sizeof(Employee), compareDesc);
    }
}


ErrorCode writeEmployees(const char* filename, Employee* employees, int count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %s %.2lf\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].salary);
    }

    fclose(file);
    return SUCCESS;
}


int main(int argc, char* argv[]) {
    char* inputFile, * outputFile;
    char sortFlag;
    Employee* employees;
    int count;
    ErrorCode error;

    error = parse_arg(argc, argv, &inputFile, &outputFile, &sortFlag);
    if (error == ERROR_INVALID_ARGUMENT) {
        printf("Error: invalid number of arguments.\n");
        return ERROR_INVALID_ARGUMENT;
    }
    else if (error == ERROR_INVALID_FLAG) {
        printf("Error: invalid sort flag. Use -a or -d.\n");
        return ERROR_INVALID_FLAG;
    }

    error = loadEmployees(inputFile, &employees, &count);
    if (error != SUCCESS) {
        printf("Error loading employees from file.\n");
        return ERROR_FILE_NOT_FOUND;
    }

    sortEmployees(employees, count, sortFlag);

    error = writeEmployees(outputFile, employees, count);
    if (error != SUCCESS) {
        printf("Error writing employees to file.\n");
    }

    free(employees);
    return error;
}



