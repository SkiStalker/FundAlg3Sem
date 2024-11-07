#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

typedef struct Liver {
    char *surname;
    char *name;
    char *patronymic;
    Date birth_date;
    char gender;
    double income;
    struct Liver *next;
} Liver;

typedef enum { ADD, DELETE, UPDATE } ActionType;

typedef struct Action {
    ActionType type;
    Liver *liver;
    Liver *previous;
    struct Action *next;
} Action;

typedef struct Database {
    Liver *head;
    Action *actionStack;
    int totalModifications;
} Database;

Liver *createLiver(const char *surname, const char *name, const char *patronymic, Date birth_date, char gender, double income);
void insertSorted(Database *db, Liver *newLiver);
void readFromFile(Database *db, const char *filename);
void writeToFile(Database *db, const char *filename);
void pushAction(Database *db, ActionType type, Liver *liver, Liver *previous);
void undoLastModifications(Database *db);
Liver *findLiver(Database *db, const char *surname, const char *name);
void updateLiver(Database *db, const char *surname, const char *name, Date birth_date, char gender, double income);
void deleteLiver(Database *db, const char *surname, const char *name);
void freeList(Database *db);

Liver *createLiver(const char *surname, const char *name, const char *patronymic, Date birth_date, char gender, double income) {
    Liver *newLiver = (Liver*)malloc(sizeof(Liver));
    newLiver->surname = strdup(surname);
    newLiver->name = strdup(name);
    newLiver->patronymic = patronymic ? strdup(patronymic) : NULL;
    newLiver->birth_date = birth_date;
    newLiver->gender = gender;
    newLiver->income = income;
    newLiver->next = NULL;
    return newLiver;
}

void insertSorted(Database *db, Liver *newLiver) {
    if (!db->head || (newLiver->birth_date.year < db->head->birth_date.year) || 
       (newLiver->birth_date.year == db->head->birth_date.year && 
       newLiver->birth_date.month < db->head->birth_date.month) || 
       (newLiver->birth_date.year == db->head->birth_date.year && 
       newLiver->birth_date.month == db->head->birth_date.month && 
       newLiver->birth_date.day < db->head->birth_date.day)) {
        newLiver->next = db->head;
        db->head = newLiver;
    } else {
        Liver *current = db->head;
        while (current->next && 
               (newLiver->birth_date.year > current->next->birth_date.year || 
               (newLiver->birth_date.year == current->next->birth_date.year && 
               newLiver->birth_date.month > current->next->birth_date.month) ||
               (newLiver->birth_date.year == current->next->birth_date.year && 
               newLiver->birth_date.month == current->next->birth_date.month && 
               newLiver->birth_date.day >= current->next->birth_date.day))) {
            current = current->next;
        }
        newLiver->next = current->next;
        current->next = newLiver;
    }
}

void readFromFile(Database *db, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    char surname[50], name[50], patronymic[50];
    Date birth_date;
    char gender;
    double income;

    while (fscanf(file, "%49s %49s %49s %d %d %d %c %lf", surname, name, patronymic, &birth_date.day, &birth_date.month, &birth_date.year, &gender, &income) == 8) {
        Liver *newLiver = createLiver(surname, name, strlen(patronymic) ? patronymic : NULL, birth_date, gender, income);
        insertSorted(db, newLiver);
    }

    fclose(file);
}

void writeToFile(Database *db, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка открытия файла для записи");
        return;
    }

    for (Liver *current = db->head; current; current = current->next) {
        fprintf(file, "%s %s %s %02d %02d %04d %c %.2f\n",
                current->surname, current->name, current->patronymic ? current->patronymic : "",
                current->birth_date.day, current->birth_date.month, current->birth_date.year,
                current->gender, current->income);
    }

    fclose(file);
}

void pushAction(Database *db, ActionType type, Liver *liver, Liver *previous) {
    Action *newAction = (Action *)malloc(sizeof(Action));
    newAction->type = type;
    newAction->liver = createLiver(liver->surname, liver->name, liver->patronymic, 
    liver->birth_date, liver->gender, liver->income);
    newAction->previous = previous;
    newAction->next = db->actionStack;
    db->actionStack = newAction;
    db->totalModifications++;
}

void undoLastModifications(Database *db) {
    int modificationsToUndo = db->totalModifications / 2;
    int undoneCount = 0;

    while (db->actionStack && undoneCount < modificationsToUndo) {
        Action *action = db->actionStack;
        db->actionStack = action->next;

        switch (action->type) {
            case ADD:
                deleteLiver(db, action->liver->surname, action->liver->name);
                break;
            case DELETE:
                insertSorted(db, action->liver);
                break;
            case UPDATE:
                {
                    Liver *found = findLiver(db, action->liver->surname, action->liver->name);
                    if (found) {
                        found->birth_date = action->liver->birth_date;
                        found->gender = action->liver->gender;
                        found->income = action->liver->income;
                    }
                }
                break;
        }

        free(action->liver->surname);
        free(action->liver->name);
        if (action->liver->patronymic) {
            free(action->liver->patronymic);
        }
        free(action->liver);
        free(action);

        undoneCount++;
    }

    db->totalModifications -= undoneCount;
}

Liver *findLiver(Database *db, const char *surname, const char *name) {
    for (Liver *current = db->head; current; current = current->next) {
        if (strcmp(current->surname, surname) == 0 && strcmp(current->name, name) == 0) {
            return current;
        }
    }
    return NULL;
}

void updateLiver(Database *db, const char *surname, const char *name, Date birth_date, char gender, double income) {
    Liver *liver = findLiver(db, surname, name);
    if (liver) {
        pushAction(db, UPDATE, liver, NULL);
        liver->birth_date = birth_date;
        liver->gender = gender;
        liver->income = income;
        printf("Данные успешно обновлены.\n");
    } else {
        printf("Житель не найден.\n");
    }
}

void deleteLiver(Database *db, const char *surname, const char *name) {
    Liver *current = db->head, *prev = NULL;
    while (current && (strcmp(current->surname, surname) != 0 || strcmp(current->name, name) != 0)) {
        prev = current;
        current = current->next;
    }
    if (!current) {
        printf("Житель не найден.\n");
        return;
    }

    if (prev) {
        prev->next = current->next;
    } else {
        db->head = current->next;
    }

    pushAction(db, DELETE, current, prev);
    free(current->surname);
    free(current->name);
    if (current->patronymic) {
        free(current->patronymic);
    }
    free(current);
}

void freeList(Database *db) {
    Liver *current = db->head;
    while (current) {
        Liver *temp = current;
        current = current->next;
        free(temp->surname);
        free(temp->name);
        if (temp->patronymic) {
            free(temp->patronymic);
        }
        free(temp);
    }

    Action *action = db->actionStack;
    while (action) {
        Action *temp = action;
        action = action->next;
        free(temp->liver->surname);
        free(temp->liver->name);
        if (temp->liver->patronymic) {
            free(temp->liver->patronymic);
        }
        free(temp->liver);
        free(temp);
    }
}

int main() {
    Database db = {NULL, NULL, 0};
    char filename[256];

    printf("Введите имя файла для чтения данных: ");
    scanf("%255s", filename);
    readFromFile(&db, filename);

    char action;
    while (1) {
        printf("Действия: \n(a) добавить \n(f) найти \n(u) обновить \n(d) удалить \n(o) отменить последнюю операцию \n(s) сохранить \n(q) выход\nВведите действие: ");
        scanf(" %c", &action);

        if (action == 'a') {
            char surname[50], name[50], patronymic[50];
            Date birth_date;
            char gender;
            double income;

            printf("Введите фамилию: ");
            scanf("%49s", surname);
            printf("Введите имя: ");
            scanf("%49s", name);
            printf("Введите отчество (или '-' если отсутствует): ");
            scanf("%49s", patronymic);
            printf("Введите дату рождения (дд мм гггг): ");
            scanf("%d %d %d", &birth_date.day, &birth_date.month, &birth_date.year);
            printf("Введите пол (M/F): ");
            scanf(" %c", &gender);
            printf("Введите доход: ");
            scanf("%lf", &income);

            Liver *newLiver = createLiver(surname, name, strcmp(patronymic, "-") == 0 ? NULL : patronymic, birth_date, gender, income);
            insertSorted(&db, newLiver);
            pushAction(&db, ADD, newLiver, NULL);
            printf("Житель добавлен.\n");

        } else if (action == 'f') {
            char surname[50], name[50];
            printf("Введите фамилию: ");
            scanf("%49s", surname);
            printf("Введите имя: ");
            scanf("%49s", name);

            Liver *found = findLiver(&db, surname, name);
            if (found) {
                printf("Найден житель: %s %s %s, Дата рождения: %02d.%02d.%04d, Пол: %c, Доход: %.2f\n",
                       found->surname, found->name, found->patronymic ? found->patronymic : "-", 
                       found->birth_date.day, found->birth_date.month, found->birth_date.year, found->gender, found->income);
            } else {
                printf("Житель не найден.\n");
            }

        } else if (action == 'u') {
            char surname[50], name[50];
            Date birth_date;
            char gender;
            double income;

            printf("Введите фамилию: ");
            scanf("%49s", surname);
            printf("Введите имя: ");
            scanf("%49s", name);
            printf("Введите новую дату рождения (дд мм гггг): ");
            scanf("%d %d %d", &birth_date.day, &birth_date.month, &birth_date.year);
            printf("Введите новый пол (M/W): ");
            scanf(" %c", &gender);
            printf("Введите новый доход: ");
            scanf("%lf", &income);

            updateLiver(&db, surname, name, birth_date, gender, income);
            printf("Данные обновлены.\n");

        } else if (action == 'd') {
            char surname[50], name[50];
            printf("Введите фамилию: ");
            scanf("%49s", surname);
            printf("Введите имя: ");
            scanf("%49s", name);

            deleteLiver(&db, surname, name);
            printf("Житель удален.\n");

        } else if (action == 'o') {
            undoLastModifications(&db);
            printf("Отмена последних изменений выполнена.\n");

        } else if (action == 's') {
            printf("Введите имя файла для сохранения: ");
            scanf("%255s", filename);
            writeToFile(&db, filename);
            printf("Данные успешно сохранены в файл %s.\n", filename);

        } else if (action == 'q') {
            break;

        } else {
            printf("Неверный выбор, попробуйте снова.\n");
        }
    }

    freeList(&db);
    return 0;
}