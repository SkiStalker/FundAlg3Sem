#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

typedef enum StatusCode {
    SUCCESS = 0,
    ERROR,
} StatusCode;

typedef enum ActionType {
    ADD = 0,
    DELETE,
    MODIFY
} ActionType;

typedef struct Liver {
    char last_name[MAX_NAME_LENGTH];
    char first_name[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];
    int birth_day;
    int birth_month;
    int birth_year;
    char gender;
    double income;
    struct Liver *next; 
} Liver;

typedef struct History {
    ActionType action_type;
    Liver *data; 
    struct History *next; 
} History;

Liver *head = NULL;  
History *history = NULL;  

Liver* create_liver(const char* last_name, const char* first_name, const char* patronymic, int day, int month, int year, char gender, double income) {
    Liver *new_liver = (Liver*)malloc(sizeof(Liver));
    strcpy(new_liver->last_name, last_name);
    strcpy(new_liver->first_name, first_name);
    strcpy(new_liver->patronymic, patronymic);
    new_liver->birth_day = day;
    new_liver->birth_month = month;
    new_liver->birth_year = year;
    new_liver->gender = gender;
    new_liver->income = income;
    new_liver->next = NULL;
    return new_liver;
}

StatusCode add_liver(Liver *new_liver) {
    if (!head || (new_liver->birth_year < head->birth_year || 
       (new_liver->birth_year == head->birth_year && new_liver->birth_month < head->birth_month) ||
       (new_liver->birth_year == head->birth_year && new_liver->birth_month == head->birth_month && new_liver->birth_day < head->birth_day))) {
        new_liver->next = head;
        head = new_liver;
    } else {
        Liver *current = head;
        while (current->next && (current->next->birth_year < new_liver->birth_year ||
               (current->next->birth_year == new_liver->birth_year && current->next->birth_month < new_liver->birth_month) ||
               (current->next->birth_year == new_liver->birth_year && current->next->birth_month == new_liver->birth_month && current->next->birth_day < new_liver->birth_day))) {
            current = current->next;
        }
        new_liver->next = current->next;
        current->next = new_liver;
    }

    History *new_history = (History*)malloc(sizeof(History));
    new_history->action_type = ADD;
    new_history->data = new_liver;
    new_history->next = history;
    history = new_history;

    return SUCCESS;
}

StatusCode print_livers() {
    if (head == NULL) {
        printf("Список жителей пуст.\n");
        return ERROR;
    }
    Liver *current = head;
    while (current != NULL) {
        printf("%s %s %s, Дата рождения: %d.%d.%d, Пол: %c, Доход: %.2f\n", 
            current->last_name, current->first_name, current->patronymic,
            current->birth_day, current->birth_month, current->birth_year, 
            current->gender, current->income);
        current = current->next;
    }

    return SUCCESS;
}

StatusCode delete_liver(const char *last_name) {
    if (head == NULL) {
        printf("Список пуст, нечего удалять.\n");
        return ERROR;
    }

    Liver *current = head;
    if (strcmp(current->last_name, last_name) == 0) {
        head = head->next;

        History *new_history = (History*)malloc(sizeof(History));
        new_history->action_type = DELETE;
        new_history->data = current;
        new_history->next = history;
        history = new_history;

        free(current);
        printf("Житель удален.\n");
        return SUCCESS;
    }

    while (current->next != NULL && strcmp(current->next->last_name, last_name) != 0) {
        current = current->next;
    }

    if (current->next == NULL) {
        printf("Житель с такой фамилией не найден.\n");
        return ERROR;
    }

    Liver *temp = current->next;
    current->next = current->next->next;

    History *new_history = (History*)malloc(sizeof(History));
    new_history->action_type = DELETE;
    new_history->data = temp;
    new_history->next = history;
    history = new_history;

    free(temp);
    printf("Житель удален.\n");

    return SUCCESS;
}

Liver* search_liver(const char *last_name) {
    Liver *current = head;
    while (current != NULL) {
        if (strcmp(current->last_name, last_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

StatusCode modify_liver(const char *last_name) {
    Liver *liver = search_liver(last_name);
    if (liver == NULL) {
        printf("Житель с такой фамилией не найден.\n");
        return ERROR;
    }

    Liver *old_data = (Liver*)malloc(sizeof(Liver));
    *old_data = *liver; 


    printf("Введите новое имя: ");
    scanf("%s", liver->first_name);
    printf("Введите новое отчество: ");
    scanf("%s", liver->patronymic);
    printf("Введите новую дату рождения (день месяц год): ");
    scanf("%d %d %d", &liver->birth_day, &liver->birth_month, &liver->birth_year);
    printf("Введите новый пол (M/W): ");
    scanf(" %c", &liver->gender);
    printf("Введите новый доход: ");
    scanf("%lf", &liver->income);

    History *new_history = (History*)malloc(sizeof(History));
    new_history->action_type = MODIFY;
    new_history->data = old_data;  
    new_history->next = history;
    history = new_history;

    printf("Данные обновлены.\n");

    return SUCCESS;
}

StatusCode undo() {
    if (history == NULL) {
        printf("Нет изменений для отмены.\n");
        return ERROR;
    }

    History *temp = history;
    history = history->next;

    if (temp->action_type == ADD) {
        Liver *current = head;
        if (current == temp->data) {
            head = current->next;
        } else {
            while (current->next != temp->data) {
                current = current->next;
            }
            current->next = temp->data->next;
        }
        free(temp->data);
        printf("Отмена действия: добавление.\n");

    } else if (temp->action_type == DELETE) {
        Liver *recovered = temp->data;
        add_liver(recovered);
        printf("Отмена действия: удаление.\n");

    } else if (temp->action_type == MODIFY) {
        *temp->data = *temp->data;
        printf("Отмена действия: изменение.\n");
    }

    free(temp);
    return SUCCESS;
}

StatusCode export_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Не удалось открыть файл для записи.\n");
        return ERROR;
    }

    Liver *current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %s %d.%d.%d %c %.2f\n", current->last_name, current->first_name, current->patronymic,
            current->birth_day, current->birth_month, current->birth_year, current->gender, current->income);
        current = current->next;
    }

    fclose(file);
    printf("Данные экспортированы в файл %s.\n", filename);

    return SUCCESS;
}

int main() {
    int choice;
    char filename[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];

    while (1) {
        printf("\nМеню:\n");
        printf("1. Добавить жителя\n");
        printf("2. Печать всех жителей\n");
        printf("3. Удалить жителя\n");
        printf("4. Изменить данные о жителе\n");
        printf("5. Поиск жителя\n");
        printf("6. Отменить последние изменения\n");
        printf("7. Экспортировать данные в файл\n");
        printf("8. Выход\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    char last_name[MAX_NAME_LENGTH], first_name[MAX_NAME_LENGTH], patronymic[MAX_NAME_LENGTH];
                    int day, month, year;
                    char gender;
                    double income;
                    printf("Введите фамилию: ");
                    scanf("%s", last_name);
                    printf("Введите имя: ");
                    scanf("%s", first_name);
                    printf("Введите отчество: ");
                    scanf("%s", patronymic);
                    printf("Введите день, месяц, год рождения: ");
                    scanf("%d %d %d", &day, &month, &year);
                    printf("Введите пол (M/W): ");
                    scanf(" %c", &gender);
                    printf("Введите доход: ");
                    scanf("%lf", &income);

                    Liver *new_liver = create_liver(last_name, first_name, patronymic, day, month, year, gender, income);
                    add_liver(new_liver);
                }
                break;
            case 2:
                print_livers();
                break;
            case 3:
                printf("Введите фамилию для удаления: ");
                scanf("%s", last_name);
                delete_liver(last_name);
                break;
            case 4:
                printf("Введите фамилию для изменения: ");
                scanf("%s", last_name);
                modify_liver(last_name);
                break;
            case 5:
                printf("Введите фамилию для поиска: ");
                scanf("%s", last_name);
                if (search_liver(last_name)) {
                    printf("Житель найден.\n");
                } else {
                    printf("Житель не найден.\n");
                }
                break;
            case 6:
                undo();
                break;
            case 7:
                printf("Введите имя файла: ");
                scanf("%s", filename);
                export_to_file(filename);
                break;
            case 8:
                return 0;
            default:
                printf("Некорректный выбор. Попробуйте снова.\n");
        }
    }
}
