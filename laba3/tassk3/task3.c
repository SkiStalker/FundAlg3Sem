#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 50

typedef struct {
    int id;
    char first_name[NAME_LEN];
    char last_name[NAME_LEN];
    double salary;
} Employee;

// Перечисление кодов возврата
enum return_codes {
    SUCCESS = 0,
    FILE_NOT_EXIST,
    WRONG_ARGS_COUNT,
    MEMORY_ERROR,
    INVALID_FLAG,
    INVALID_DATA_FORMAT,
    INVALID_DATA_CONTENT,
    OUTPUT_FILE_ERROR
};

// Глобальная переменная для хранения порядка сортировки
int sortOrder = 1; // 1 для возрастания, -1 для убывания

// Универсальная функция сравнения с учетом порядка сортировки
int compareEmployees(const void *a, const void *b) {
    Employee *emp1 = (Employee *)a;
    Employee *emp2 = (Employee *)b;

    // Сравнение зарплаты
    if (emp1->salary < emp2->salary)
        return -1 * sortOrder;
    if (emp1->salary > emp2->salary)
        return 1 * sortOrder;

    // Сравнение фамилии
    int last_name_cmp = strcmp(emp1->last_name, emp2->last_name);
    if (last_name_cmp != 0)
        return last_name_cmp * sortOrder;

    // Сравнение имени
    int first_name_cmp = strcmp(emp1->first_name, emp2->first_name);
    if (first_name_cmp != 0)
        return first_name_cmp * sortOrder;

    // Сравнение id
    if (emp1->id < emp2->id)
        return -1;
    if (emp1->id > emp2->id)
        return 1;
    return 0;
}

// Функция для проверки, начинается ли строка с '-' или '/'
int isValidFlag(char *flag) {
    if (flag[0] != '-' && flag[0] != '/')
        return 0;
    if (flag[1] != 'a' && flag[1] != 'd')
        return 0;
    if (flag[2] != '\0')
        return 0;
    return 1;
}

// Функция для чтения сотрудников из файла
Employee* read_employees(const char *filename, int *count, int *error_code) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: Не удалось открыть входной файл '%s'.\n", filename);
        *error_code = FILE_NOT_EXIST;
        return NULL;
    }

    // Динамический массив сотрудников
    size_t capacity = 10;
    size_t size = 0;
    Employee *employees = malloc(capacity * sizeof(Employee));
    if (!employees) {
        printf("Ошибка: Не удалось выделить память для сотрудников.\n");
        fclose(file);
        *error_code = MEMORY_ERROR;
        return NULL;
    }

    // Чтение данных сотрудников
    char line[200];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;

        // Игнорирование пустых строк
        if (strlen(line) <= 1) continue;

        Employee emp;
        char first[NAME_LEN], last[NAME_LEN];
        double salary;
        int id;

        if (sscanf(line, "%d %49s %49s %lf", &id, first, last, &salary) != 4) {
            printf("Ошибка: Неверный формат данных на строке %d: %s", line_number, line);
            *error_code = INVALID_DATA_FORMAT;
            free(employees);
            fclose(file);
            return NULL;
        }

        // Валидация данных
        if (id < 0 || salary < 0) {
            printf("Ошибка: Отрицательные значения в данных на строке %d: %s", line_number, line);
            *error_code = INVALID_DATA_CONTENT;
            free(employees);
            fclose(file);
            return NULL;
        }

        // Проверка, что имена содержат только буквы
        int valid = 1;
        for (int i = 0; first[i]; i++) {
            if (!isalpha(first[i])) { valid = 0; break; }
        }
        for (int i = 0; last[i]; i++) {
            if (!isalpha(last[i])) { valid = 0; break; }
        }
        if (!valid) {
            printf("Ошибка: Имена содержат недопустимые символы на строке %d: %s %s\n", line_number, first, last);
            *error_code = INVALID_DATA_CONTENT;
            free(employees);
            fclose(file);
            return NULL;
        }

        // Заполнение структуры Employee
        emp.id = id;
        strcpy(emp.first_name, first);
        strcpy(emp.last_name, last);
        emp.salary = salary;

        // Добавление сотрудника в массив
        if (size >= capacity) {
            capacity *= 2;
            Employee *temp = realloc(employees, capacity * sizeof(Employee));
            if (!temp) {
                printf("Ошибка: Не удалось перераспределить память для сотрудников.\n");
                free(employees);
                fclose(file);
                *error_code = MEMORY_ERROR;
                return NULL;
            }
            employees = temp;
        }
        employees[size++] = emp;
    }

    fclose(file);
    *count = size;
    *error_code = SUCCESS;
    return employees;
}

// Функция для записи сотрудников в файл
int write_employees(const char *filename, Employee *employees, int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка: Не удалось открыть выходной файл '%s'.\n", filename);
        return OUTPUT_FILE_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(file, "%d %s %s %.2lf\n", employees[i].id, employees[i].first_name, employees[i].last_name, employees[i].salary) < 0) {
            printf("Ошибка: Не удалось записать данные в файл '%s'.\n", filename);
            fclose(file);
            return OUTPUT_FILE_ERROR;
        }
    }

    fclose(file);
    return SUCCESS;
}

int main(int argc, char *argv[]) {
    // Проверка количества аргументов
    if (argc != 4) {
        printf("Ошибка: Неверное количество аргументов, введите в формате: <входной_файл> <флаг (-a или -d)> <выходной_файл>.\n");
        return WRONG_ARGS_COUNT;
    }

    char *input_file = argv[1];
    char *flag = argv[2];
    char *output_file = argv[3];
    int ret_code = SUCCESS;

    // Проверка корректности флага
    if (!isValidFlag(flag)) {
        printf("Ошибка: Некорректный флаг '%s'. Допустимы флаги '-a' или '-d'.\n", flag);
        return INVALID_FLAG;
    }

    // Установка порядка сортировки
    sortOrder = (flag[1] == 'a') ? 1 : -1;

    // Чтение сотрудников из файла
    int employee_count = 0;
    Employee *employees = read_employees(input_file, &employee_count, &ret_code);
    if (ret_code != SUCCESS) {
        // Соответствующее сообщение уже выведено в функции read_employees
        return ret_code;
    }

    // Сортировка массива
    qsort(employees, employee_count, sizeof(Employee), compareEmployees);

    // Запись отсортированных данных в выходной файл
    ret_code = write_employees(output_file, employees, employee_count);
    if (ret_code != SUCCESS) {
        // Соответствующее сообщение уже выведено в функции write_employees
        free(employees);
        return ret_code;
    }

    free(employees);
    printf("Сортировка завершена успешно. Результат записан в '%s'.\n", output_file);
    return SUCCESS;
}
