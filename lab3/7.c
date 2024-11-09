#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int day, month, year;
} Date;

typedef struct Liver {
    char surname[50];
    char name[50];
    char patronymic[50];
    Date birthDate;
    char gender;
    float income;
    struct Liver *next;
} Liver;

typedef struct Modification {
    int type; // 1: добавление, 2: удаление, 3: изменение
    Liver *liverData; // Сохранённые данные о жителе для отката
    struct Modification *next;
} Modification;

Liver *createLiver(char *surname, char *name, char *patronymic, Date birthDate, char gender, float income) {
    Liver *newLiver = (Liver *)malloc(sizeof(Liver));
    if (!newLiver) return NULL;
    strcpy(newLiver->surname, surname);
    strcpy(newLiver->name, name);
    strcpy(newLiver->patronymic, patronymic);
    newLiver->birthDate = birthDate;
    newLiver->gender = gender;
    newLiver->income = income;
    newLiver->next = NULL;
    return newLiver;
}

int compareDates(Date d1, Date d2) {
    if (d1.year != d2.year) return d1.year - d2.year;
    if (d1.month != d2.month) return d1.month - d2.month;
    return d1.day - d2.day;
}

int insertSorted(Liver **head, Liver *newLiver) {
    if (!newLiver) return -1;
    if (*head == NULL || compareDates(newLiver->birthDate, (*head)->birthDate) < 0) {
        newLiver->next = *head;
        *head = newLiver;
        return 0;
    }

    Liver *current = *head;
    while (current->next != NULL && compareDates(newLiver->birthDate, current->next->birthDate) >= 0) {
        current = current->next;
    }

    newLiver->next = current->next;
    current->next = newLiver;
    return 0;
}

int loadFromFile(Liver **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Не удалось открыть файл %s\n", filename);
        return -1;
    }

    char surname[50], name[50], patronymic[50], gender;
    int day, month, year;
    float income;

    while (fscanf(file, "%s\n%s\n%[^\n]\n%d %d %d\n%c\n%f\n", surname, name, patronymic, &day, &month, &year, &gender, &income) != EOF) {
        if (strcmp(patronymic, "") == 0) {
            strcpy(patronymic, "");
        }

        Date birthDate = {day, month, year};
        Liver *newLiver = createLiver(surname, name, patronymic, birthDate, gender, income);
        if (newLiver) {
            insertSorted(head, newLiver);
        }
    }

    fclose(file);
    return 0;
}

int saveToFile(Liver *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Не удалось открыть файл для записи: %s\n", filename);
        return -1;
    }

    Liver *current = head;
    while (current != NULL) {
        fprintf(file, "%s\n%s\n%s\n%d %d %d\n%c\n%.2f\n", current->surname, current->name,
                current->patronymic, current->birthDate.day, current->birthDate.month,
                current->birthDate.year, current->gender, current->income);
        current = current->next;
    }
    fclose(file);
    return 0;
}

int searchLiver(Liver *head, char *surname, char *name, char *patronymic, Date *birthDate, char *gender, float *income) {
    Liver *current = head;
    while (current != NULL) {
        int match = 1;

        if (surname != NULL && strcmp(current->surname, surname) != 0) match = 0;
        if (name != NULL && strcmp(current->name, name) != 0) match = 0;
        if (patronymic != NULL && strcmp(current->patronymic, patronymic) != 0) match = 0;
        if (birthDate != NULL && compareDates(current->birthDate, *birthDate) != 0) match = 0;
        if (gender != NULL && current->gender != *gender) match = 0;
        if (income != NULL && current->income != *income) match = 0;

        if (match) {
            printf("Найден житель: %s %s %s, Дата рождения: %02d.%02d.%d, Пол: %c, Доход: %.2f\n",
                   current->surname, current->name, current->patronymic, current->birthDate.day,
                   current->birthDate.month, current->birthDate.year, current->gender, current->income);
            return 0;
        }
        current = current->next;
    }
    printf("Житель с заданными параметрами не найден.\n");
    return -1;
}

int deleteLiver(Liver **head, char *surname, char *name, Modification **modifications) {
    if (*head == NULL) return -1;
    Liver *current = *head, *prev = NULL;

    while (current != NULL && (strcmp(current->surname, surname) != 0 || strcmp(current->name, name) != 0)) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Житель с именем %s %s не найден для удаления.\n", name, surname);
        return -1;
    }

    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }

    // Сохранение модификации для отмены
    Modification *mod = (Modification *)malloc(sizeof(Modification));
    mod->type = 2;
    mod->liverData = current;
    mod->next = *modifications;
    *modifications = mod;

    return 0;
}

int updateLiver(Liver *head, char *surname, char *name, Modification **modifications) {
    Liver *current = head;
    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0 && strcmp(current->name, name) == 0) {
            // Сохранение старых данных для отката
            Liver *oldData = createLiver(current->surname, current->name, current->patronymic, current->birthDate, current->gender, current->income);
            Modification *mod = (Modification *)malloc(sizeof(Modification));
            mod->type = 3;
            mod->liverData = oldData;
            mod->next = *modifications;
            *modifications = mod;

            // Ввод новых данных
            printf("Введите новые данные:\n");
            printf("Отчество (если нет, введите пустую строку): ");
            scanf(" %[^\n]", current->patronymic);
            printf("Дата рождения (день месяц год): ");
            scanf("%d %d %d", &current->birthDate.day, &current->birthDate.month, &current->birthDate.year);
            printf("Пол (M/W): ");
            scanf(" %c", &current->gender);
            printf("Средний доход: ");
            scanf("%f", &current->income);

            return 0;
        }
        current = current->next;
    }
    printf("Житель с именем %s %s не найден для изменения.\n", name, surname);
    return -1;
}

int undoLastModifications(Liver **head, Modification **modifications) {
    int count = 0;
    Modification *mod = *modifications;
    
    // Подсчитаем количество модификаций
    while (mod != NULL) {
        count++;
        mod = mod->next;
    }

    int undoCount = count / 2;
    for (int i = 0; i < undoCount && *modifications != NULL; i++) {
        Modification *currentMod = *modifications;
        *modifications = (*modifications)->next;

        if (currentMod->type == 1) {
            // Удаление добавленного жителя
            deleteLiver(head, currentMod->liverData->surname, currentMod->liverData->name, modifications);
        } else if (currentMod->type == 2) {
            // Восстановление удалённого жителя
            insertSorted(head, currentMod->liverData);
        } else if (currentMod->type == 3) {
            // Откат изменения
            Liver *current = *head;
            while (current != NULL) {
                if (strcmp(current->surname, currentMod->liverData->surname) == 0 &&
                    strcmp(current->name, currentMod->liverData->name) == 0) {
                    strcpy(current->patronymic, currentMod->liverData->patronymic);
                    current->birthDate = currentMod->liverData->birthDate;
                    current->gender = currentMod->liverData->gender;
                    current->income = currentMod->liverData->income;
                    break;
                }
                current = current->next;
            }
        }
        free(currentMod); // Освобождаем текущую модификацию
    }

    // Освободим все оставшиеся модификации в списке
    while (*modifications != NULL) {
        Modification *temp = *modifications;
        *modifications = (*modifications)->next;
        free(temp); // Освобождаем память
    }

    return undoCount;
}

int printMenu() {
    printf("Меню:\n");
    printf("1. Загрузить данные из файла\n");
    printf("2. Сохранить данные в файл\n");
    printf("3. Найти жителя\n");
    printf("4. Удалить жителя\n");
    printf("5. Изменить данные жителя\n");
    printf("6. Отменить последние N/2 модификаций\n");
    printf("7. Добавить жителя\n");
    printf("0. Выйти\n");
    return 0;
}

int main() {
    Liver *head = NULL;
    Modification *modifications = NULL;
    int choice;
    char filename[100];

    while (1) {
        printMenu();
        printf("Выберите опцию: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите имя файла для загрузки: ");
                scanf("%s", filename);
                loadFromFile(&head, filename);
                break;
            case 2:
                printf("Введите имя файла для сохранения: ");
                scanf("%s", filename);
                saveToFile(head, filename);
                break;
            case 3: {
                char surname[50], name[50], patronymic[50], gender;
                int day, month, year;
                float income;
                Date birthDate;
                int useBirthDate = 0, useGender = 0, useIncome = 0;

                printf("Введите фамилию (или '-' чтобы пропустить): ");
                scanf(" %[^\n]", surname);
                if (strcmp(surname, "-") == 0) {
                    surname[0] = '\0';  // Задаем пустую строку для фамилии
                }

                printf("Введите имя (или '-' чтобы пропустить): ");
                scanf(" %[^\n]", name);
                if (strcmp(name, "-") == 0) {
                    name[0] = '\0';  // Задаем пустую строку для имени
                }

                printf("Введите отчество (или '-' чтобы пропустить): ");
                scanf(" %[^\n]", patronymic);
                if (strcmp(patronymic, "-") == 0) {
                    patronymic[0] = '\0';  // Задаем пустую строку для отчества
                }
                printf("Введите дату рождения (день месяц год, или -1 -1 -1, чтобы пропустить): ");
                scanf("%d %d %d", &day, &month, &year);
                if (day != -1 && month != -1 && year != -1) {
                    birthDate = (Date){day, month, year};
                    useBirthDate = 1;
                }
                printf("Введите пол (M/W, или X, чтобы пропустить): ");
                scanf(" %c", &gender);
                if (gender != 'X') {
                    useGender = 1;
                }
                printf("Введите средний доход (или -1, чтобы пропустить): ");
                scanf("%f", &income);
                if (income != -1) {
                    useIncome = 1;
                }

                searchLiver(head,
                            strlen(surname) > 0 ? surname : NULL,
                            strlen(name) > 0 ? name : NULL,
                            strlen(patronymic) > 0 ? patronymic : NULL,
                            useBirthDate ? &birthDate : NULL,
                            useGender ? &gender : NULL,
                            useIncome ? &income : NULL);
                break;
            }
            case 4: {
                char surname[50], name[50];
                printf("Введите фамилию и имя для удаления: ");
                scanf("%s %s", surname, name);
                deleteLiver(&head, surname, name, &modifications);
                break;
            }
            case 5: {
                char surname[50], name[50];
                printf("Введите фамилию и имя для изменения: ");
                scanf("%s %s", surname, name);
                updateLiver(head, surname, name, &modifications);
                break;
            }
            case 6:
                int modi = undoLastModifications(&head, &modifications);
                printf("Отменено %d последних модификаций\n", modi);
                break;
            case 7:
                char surname[50], name[50], patronymic[50];
                Date birthDate;
                char gender;
                float income;

                printf("Введите фамилию: ");
                scanf("%s", surname);
                printf("Введите имя: ");
                scanf("%s", name);
                printf("Введите отчество: ");
                scanf("%s", patronymic);
                printf("Введите дату рождения (день месяц год): ");
                scanf("%d %d %d", &birthDate.day, &birthDate.month, &birthDate.year);
                printf("Введите пол (M/W): ");
                scanf(" %c", &gender);
                printf("Введите доход: ");
                scanf("%f", &income);

                Liver *newLiver = createLiver(surname, name, patronymic, birthDate, gender, income);
                insertSorted(&head, newLiver);

                // Сохраняем модификацию добавления в список
                Modification *mod = (Modification *)malloc(sizeof(Modification));
                mod->type = 1;  // Тип 1 — добавление
                mod->liverData = newLiver;
                mod->next = modifications;
                modifications = mod;

                break;
            case 0:
                // Освободим все ресурсы, связанные с жителями
                while (head != NULL) {
                    Liver *temp = head;
                    head = head->next;
                    free(temp); // Освобождаем память каждого жителя
                }

                // Освободим все ресурсы, связанные с модификациями
                while (modifications != NULL) {
                    Modification *temp = modifications;
                    modifications = modifications->next;
                    free(temp); // Освобождаем память каждой модификации
                }

                // Выход из программы
                printf("Все ресурсы освобождены. Выход из программы.\n");
                return 0;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
    return 0;
}
