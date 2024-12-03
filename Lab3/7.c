#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char lastName[50];
    char firstName[50];
    char middleName[50];
    int birthDay;
    int birthMonth;
    int birthYear;
    char gender;
    float averageIncome;
} Liver;

typedef struct Node {
    Liver resident;
    struct Node* next;
} Node;

// Функция для создания нового узла
Node* createNode(Liver resident) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    newNode->resident = resident;
    newNode->next = NULL;
    return newNode;
}

int shouldInsertBefore(Node* head, Liver resident) {
    if (head == NULL || (resident.birthYear < head->resident.birthYear ||
        (resident.birthYear == head->resident.birthYear && resident.birthMonth < head->resident.birthMonth) ||
        (resident.birthYear == head->resident.birthYear && resident.birthMonth == head->resident.birthMonth &&
        resident.birthDay < head->resident.birthDay))){
            return 1;
        }
    else {
        return 0;
        };
}

// Функция для вставки узла в упорядоченный список
Node* insertSorted(Node* head, Liver resident) {
    Node* newNode = createNode(resident);
    if (shouldInsertBefore(head, resident)) {
        newNode->next = head;
        return newNode;
    } else {
        Node* current = head;
        while (current->next != NULL && !shouldInsertBefore(current->next, resident)) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        return head;
    }
}

// Функция для чтения данных из файла и заполнения списка
Node* readFromFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла!\n");
        exit(1);
    }

    Node* head = NULL;
    Liver resident;
    while (fscanf(file, "%49s %49s %49s %d %d %d %c %f",
                  resident.lastName, resident.firstName, resident.middleName,
                  &resident.birthDay, &resident.birthMonth, &resident.birthYear,
                  &resident.gender, &resident.averageIncome) == 8) {
        head = insertSorted(head, resident);
    }

    fclose(file);
    return head;
}

// Функция для вывода списка на экран в виде таблицы
void printList(Node* head) {
    printf("+-----------------+-----------------+-----------------+-------------+-----+---------------+\n");
    printf("|     Фамилия     |      Имя        |    Отчество     | Дата рож.   | Пол | Средний доход |\n");
    printf("+-----------------+-----------------+-----------------+-------------+-----+---------------+\n");

    Node* current = head;
    while (current != NULL) {
        printf("| %-15s | %-15s | %-15s | %02d/%02d/%04d  |  %c  | %13.2f |\n",
               current->resident.lastName, current->resident.firstName, current->resident.middleName,
               current->resident.birthDay, current->resident.birthMonth, current->resident.birthYear,
               current->resident.gender, current->resident.averageIncome);
        current = current->next;
    }

    printf("+-----------------+-----------------+-----------------+-------------+-----+---------------+\n");
}

// Функция для освобождения памяти списка
Node* freeList(Node* head) {
    Node* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    return NULL;
}

// Функция для добавления нового жителя
Node* addResident(Node* head) {
    Liver newResident;
    printf("Введите данные нового жителя (Фамилия Имя Отчество День Месяц Год Пол СреднийДоход):\n");
    scanf("%49s %49s %49s %d %d %d %c %f",
          newResident.lastName, newResident.firstName, newResident.middleName,
          &newResident.birthDay, &newResident.birthMonth, &newResident.birthYear,
          &newResident.gender, &newResident.averageIncome);
    return insertSorted(head, newResident);
}

// Функция для поиска жителя по фамилии
Node* searchResident(Node* head, const char* lastName) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->resident.lastName, lastName) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Функция для изменения данных жителя
Node* modifyResident(Node* head, const char* lastName) {
    Node* found = searchResident(head, lastName);
    if (found != NULL) {
        printf("Введите новые данные (Фамилия Имя Отчество День Месяц Год Пол СреднийДоход):\n");
        scanf("%49s %49s %49s %d %d %d %c %f",
              found->resident.lastName, found->resident.firstName, found->resident.middleName,
              &found->resident.birthDay, &found->resident.birthMonth, &found->resident.birthYear,
              &found->resident.gender, &found->resident.averageIncome);
        printf("Данные обновлены.\n");
    } else {
        printf("Житель с фамилией %s не найден.\n", lastName);
    }
    return head;
}

// Функция для удаления жителя из списка
Node* deleteResident(Node* head, const char* lastName) {
    if (head == NULL) {
        return NULL;
    }
    if (strcmp(head->resident.lastName, lastName) == 0) {
        Node* temp = head;
        head = head->next;
        free(temp);
        printf("Житель с фамилией %s удален.\n", lastName);
        return head;
    }
    Node* current = head;
    while (current->next != NULL && strcmp(current->next->resident.lastName, lastName) != 0) {
        current = current->next;
    }
    if (current->next != NULL) {
        Node* temp = current->next;
        current->next = current->next->next;
        free(temp);
        printf("Житель с фамилией %s удален.\n", lastName);
    } else {
        printf("Житель с фамилией %s не найден.\n", lastName);
    }
    return head;
}

// Функция для выгрузки данных в файл
void exportToFile(Node* head) {
    char fileName[100];
    printf("Введите путь к файлу для сохранения данных: ");
    scanf("%99s", fileName);
    
    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи!\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %s %d %d %d %c %.2f\n",
                current->resident.lastName, current->resident.firstName, current->resident.middleName,
                current->resident.birthDay, current->resident.birthMonth, current->resident.birthYear,
                current->resident.gender, current->resident.averageIncome);
        current = current->next;
    }
    fclose(file);
    printf("Данные успешно сохранены в файл %s.\n", fileName);
}


// Реализация отмены:
// Стек для хранения состояний списка
typedef struct StackNode {
    Node* listState;
    struct StackNode* next;
} StackNode;

// Функция для создания копии списка
Node* copyList(Node* head) {
    if (head == NULL) {
        return NULL;
    }
    Node* newHead = (Node*)malloc(sizeof(Node));
    newHead->resident = head->resident;
    newHead->next = NULL;

    Node* current = head->next;
    Node* newCurrent = newHead;

    while (current != NULL) {
        newCurrent->next = (Node*)malloc(sizeof(Node));
        newCurrent = newCurrent->next;
        newCurrent->resident = current->resident;
        newCurrent->next = NULL;
        current = current->next;
    }
    return newHead;
}

// Функция для создания нового узла стека
StackNode* createStackNode(Node* listState) {
    StackNode* newStackNode = (StackNode*)malloc(sizeof(StackNode));
    if (newStackNode == NULL) {
        printf("Ошибка выделения памяти для стека!\n");
        exit(1);
    }
    newStackNode->listState = listState;
    newStackNode->next = NULL;
    return newStackNode;
}

// Функция для добавления состояния списка в стек
void push(StackNode** top, Node* listState) {
    StackNode* newStackNode = createStackNode(copyList(listState));
    newStackNode->next = *top;
    *top = newStackNode;
}

// Функция для извлечения состояния списка из стека
Node* pop(StackNode** top) {
    if (*top == NULL) {
        printf("Стек пуст, невозможно отменить изменение.\n");
        return NULL;
    }
    StackNode* temp = *top;
    Node* listState = temp->listState;
    *top = temp->next;
    free(temp);
    return listState;
}

// Функция для выполнения команды отмена
Node* undoLastChange(StackNode** stackTop) {
    Node* lastState = pop(stackTop);
    if (lastState != NULL) {
        printf("Последнее изменение отменено.\n");
        return lastState;
    } else {
        printf("Отменить изменение не удалось.\n");
        return NULL;
    }
}

int main() {
    const char* fileName = "7.txt";
    int choice;
    Node* head = readFromFile(fileName);
    StackNode* historyStack = NULL;
    char lastName[50];
    do {
        printf("\nМеню:\n");
        printf("1. Добавить жителя\n");
        printf("2. Найти жителя\n");
        printf("3. Изменить данные жителя\n");
        printf("4. Удалить жителя\n");
        printf("5. Выгрузить данные в файл\n");
        printf("6. Вывести список\n");
        printf("7. Отмена\n");
        printf("0. Выйти\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                push(&historyStack, head);
                head = addResident(head);
                break;
            case 2:
                printf("Введите фамилию для поиска: ");
                scanf("%49s", lastName);
                Node* found = searchResident(head, lastName);
                if (found != NULL) {
                    printf("Найден житель: %s %s %s, Дата рождения: %02d/%02d/%04d, Пол: %c, Средний доход: %.2f\n",
                           found->resident.lastName, found->resident.firstName, found->resident.middleName,
                           found->resident.birthDay, found->resident.birthMonth, found->resident.birthYear,
                           found->resident.gender, found->resident.averageIncome);
                } else {
                    printf("Житель с фамилией %s не найден.\n", lastName);
                }
                break;
            case 3:
                push(&historyStack, head);
                printf("Введите фамилию для изменения данных: ");
                scanf("%49s", lastName);
                head = modifyResident(head, lastName);
                break;
            case 4:
                push(&historyStack, head);
                printf("Введите фамилию для удаления: ");
                scanf("%49s", lastName);
                head = deleteResident(head, lastName);
                break;
            case 5:
                exportToFile(head);
                break;
            case 6:
                printList(head);
                break;
            case 7:
                head = undoLastChange(&historyStack);
                break;
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Некорректный выбор, попробуйте снова.\n");
        }
    } while (choice != 0);

    head = freeList(head);
    while (historyStack != NULL) {
        Node* temp = pop(&historyStack);
        freeList(temp);
    }
    return 0;
}
