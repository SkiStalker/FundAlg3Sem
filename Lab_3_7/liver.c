#include "liver.h"

void menu(){
    printf("Choose one\n");
    printf("1. Search by surname\n2. Search by name\n3. Search by patronymic\n4. Update liver\n"
           "5. Add liver\n6. Delete liver\n7. Write data to file\n8. Undo\n9. Print data\n0. End\n");
}

void freeList(Node* head) {
    Node* current = head;
    Node* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}

void initUndoStack(UndoStack* stack) {
    stack->top = -1; // Стек пуст
}

callback pushUndoState(UndoStack* stack, Node* head) {
    if (stack->top >= MAX_UNDO - 1) {
        return MEMORY_ERROR; // Стек полон
    }

    Node* newState = NULL;
    Node* current = head;
    while (current != NULL) {
        Liver newData = current->data;

        append(&newState, newData);
        current = current->next;
    }
    stack->states[++stack->top] = newState;
    return SUCCESS;
}

Node* popUndoState(UndoStack* stack) {
    if (stack->top < 0) {
        return NULL;
    }
    return stack->states[stack->top--];
}

void freeUndoStack(UndoStack* stack) {
    for (int i = 0; i <= stack->top; i++) {
        freeList(stack->states[i]);
    }
}

bool is_number(const char *str) {
    if (*str == '-') {
        return false;
    }

    bool hasDot = false;
    if (*str == '\0') return false;

    while (*str) {
        if (*str == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}


callback validate_date(const char *date){
    if (date[2] != ':' || date[5] != ':' || !isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[3])
        || !isdigit(date[4]) || !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8])
        || !isdigit(date[9]) || date[10] != '\0') {
        return INPUT_ERROR;
    }


    int y, m, d;
    d = atoi(date);
    m = atoi(date + 3);
    y = atoi(date + 6);
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30)
        return INPUT_ERROR;

    if (m == 2 && d > 28)
        return INPUT_ERROR;

    if (d > 31)
        return INPUT_ERROR;

    if (m > 12 || d == 0 || y < 1900 || y > 2025)
        return INPUT_ERROR;

    return SUCCESS;
}

callback validate_data(char tmp[6][128]) {
    if (!is_number(tmp[5]) || strlen(tmp[4]) != 1 || strlen(tmp[0]) == 0 || strlen(tmp[1]) == 0
    || strlen(tmp[3]) == 0 || strlen(tmp[3]) != 10 || validate_date(tmp[3]) || (tmp[4][0] != 'M' && tmp[4][0] != 'W'))
        return INPUT_ERROR;

    for (int i = 0; i < strlen(tmp[0]); ++i){
        if (!isalpha(tmp[0][i]))
            return INPUT_ERROR;
    }

    for (int i = 0; i < strlen(tmp[1]); ++i){
        if (!isalpha(tmp[1][i]))
            return INPUT_ERROR;
    }

    for (int i = 0; i < strlen(tmp[2]); ++i){
        if (!isalpha(tmp[2][i]))
            return INPUT_ERROR;
    }

    return SUCCESS;
}

Node* createNode(Liver liver) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    newNode->data = liver;
    newNode->next = NULL;
    return newNode;
}

callback append(Node **head, Liver liver) {
    Node *newNode = createNode(liver);
    if (!newNode)
        return MEMORY_ERROR;
    if (*head == NULL) {
        *head = newNode;
        return SUCCESS;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;

    return SUCCESS;
}

callback read_liver(Node **liver_data, FILE *in){
    char tmp[6][128];
    char buffer[128];
    int n = 0;
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        if (strncmp(buffer, "S:", 2) == 0) {
            sscanf(buffer + 3, "%127s", tmp[0]);
        } else if (strncmp(buffer, "N:", 2) == 0) {
            sscanf(buffer + 3, "%127s", tmp[1]);
        } else if (strncmp(buffer, "P:", 2) == 0) {
            if (strlen(buffer) > 3)
                sscanf(buffer + 3, "%127s", tmp[2]);
            else
                memset(tmp[2], 0, sizeof(tmp[2]));
        } else if (strncmp(buffer, "D:", 2) == 0) {
            sscanf(buffer + 3, "%127s", tmp[3]);
        } else if (strncmp(buffer, "G:", 2) == 0) {
            sscanf(buffer + 3, "%127s", tmp[4]);
        } else if (strncmp(buffer, "A:", 2) == 0) {
            sscanf(buffer + 3, "%127s", tmp[5]);
            ++n;
        }
        if (n > 0) {
            n--;
            if (validate_data(tmp))
                return INPUT_ERROR;

            Liver liver;
            strcpy(liver.surname, tmp[0]);
            strcpy(liver.name, tmp[1]);
            strcpy(liver.patronymic, tmp[2]);
            strcpy(liver.date_of_birth, tmp[3]);
            liver.gender = tmp[4][0];

            double tmp2 =  strtod(tmp[5], NULL);
            if (errno == ERANGE)
                return OVERFLOW;
            liver.avg_income = tmp2;

            if (append(liver_data, liver))
                return MEMORY_ERROR;
        }
    }

    return SUCCESS;
}

int getLength(Node* head) {
    int length = 0;
    Node* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

Node* getElement(Node* head, int i) {
    Node* current = head;
    int index = 0;

    while (current != NULL) {
        if (index == i) {
            return current;
        }
        index++;
        current = current->next;
    }

    return NULL;
}

callback search_by_surname(Node *liver_data, Liver **arr, int *arr_size) {
    if (!liver_data || !arr)
        return MEMORY_ERROR;

    *arr_size = 0;
    int size = getLength(liver_data);
    char surname[128];
    printf("Enter surname: ");
    if (scanf("%127s", surname) != 1)
        return INPUT_ERROR;

    Liver *tmp_arr = (Liver *) malloc(sizeof(Liver) * size);
    if (!tmp_arr)
        return MEMORY_ERROR;

    for (int i = 0; i < size; ++i){
        if (!strcmp(surname, getElement(liver_data, i)->data.surname)){
            tmp_arr[(*arr_size)++] =  getElement(liver_data, i)->data;
        }
    }

    *arr = tmp_arr;
    return SUCCESS;
}

void displayList(Node *node) {
    if (node == NULL) {
        printf("Список пуст.\n");
        return;
    }

    while (node != NULL) {
        printf("Фамилия: %s, Имя: %s, Отчество: %s, Дата рождения: %s, Пол: %c, Средний доход: %lf\n",
               node->data.surname, node->data.name, node->data.patronymic,
               node->data.date_of_birth, node->data.gender, node->data.avg_income);
        node = node->next;
    }
}

void print_stud(Liver liver){
    printf("Surname: %s\n", liver.surname);
    printf("Name: %s\n", liver.name);
    printf("Patronymic: %s\n", liver.patronymic);
    printf("Date of birth: %s\n", liver.date_of_birth);
    printf("Gender: %c\n", liver.gender);
    printf("Avg income: %lf\n", liver.avg_income);

}

callback search_by_name(Node *liver_data, Liver **arr, int *arr_size){
    if (!liver_data || !arr)
        return MEMORY_ERROR;

    *arr_size = 0;
    int size = getLength(liver_data);
    char surname[128];
    printf("Enter name: ");
    if (scanf("%127s", surname) != 1)
        return INPUT_ERROR;

    Liver *tmp_arr = (Liver *) malloc(sizeof(Liver) * size);
    if (!tmp_arr)
        return MEMORY_ERROR;

    for (int i = 0; i < size; ++i){
        if (!strcmp(surname, getElement(liver_data, i)->data.name)){
            tmp_arr[(*arr_size)++] =  getElement(liver_data, i)->data;
        }
    }

    *arr = tmp_arr;
    return SUCCESS;
}

callback search_by_patronymic(Node *liver_data, Liver **arr, int *arr_size){
    if (!liver_data || !arr)
        return MEMORY_ERROR;

    *arr_size = 0;
    int size = getLength(liver_data);
    char surname[128];
    printf("Enter patronymic: ");
    if (scanf("%127s", surname) != 1)
        return INPUT_ERROR;

    Liver *tmp_arr = (Liver *) malloc(sizeof(Liver) * size);
    if (!tmp_arr)
        return MEMORY_ERROR;

    for (int i = 0; i < size; ++i){
        if (!strcmp(surname, getElement(liver_data, i)->data.patronymic)){
            tmp_arr[(*arr_size)++] =  getElement(liver_data, i)->data;
        }
    }

    *arr = tmp_arr;
    return SUCCESS;
}

callback upd_liver(Node **liver_data){
    if (!liver_data )
        return MEMORY_ERROR;

    int size = getLength(*liver_data);
    char surname[128];
    printf("Enter surname for upd: ");
    if (scanf("%127s", surname) != 1)
        return INPUT_ERROR;

    for (int i = 0; i < size; ++i){
        if (!strcmp(surname, getElement(*liver_data, i)->data.surname)){
            Liver liver;
            if (get_liver(&liver)){
                return INPUT_ERROR;
            }
            getElement(*liver_data, i)->data = liver;
            return SUCCESS;
        }
    }

    return DONT_FIND;
}

callback get_liver(Liver *liver){
    char tmp[6][128];
    printf("Enter surname: ");
    if (scanf("%127s", tmp[0]) != 1)
        return INPUT_ERROR;

    printf("Enter name: ");
    if (scanf("%127s", tmp[1]) != 1)
        return INPUT_ERROR;

    printf("Enter patronymic: ");
    if (scanf("%127s", tmp[2]) != 1)
        return INPUT_ERROR;

    printf("Enter date of birth(dd:mm:yyyy): ");
    if (scanf("%11s", tmp[3]) != 1)
        return INPUT_ERROR;

    printf("Enter gender(M/W): ");
    if (scanf("%1s", tmp[4]) != 1)
        return INPUT_ERROR;

    printf("Enter avg_income: ");
    if (scanf("%127s", tmp[5]) != 1)
        return INPUT_ERROR;

    int r = validate_data(tmp);

    if (r)
        return r;

    strcpy((*liver).surname, tmp[0]);
    strcpy((*liver).name, tmp[1]);
    strcpy((*liver).patronymic, tmp[2]);
    strcpy((*liver).date_of_birth, tmp[3]);
    (*liver).gender = tmp[4][0];

    double tmp2 =  strtod(tmp[5], NULL);
    if (errno == ERANGE)
        return OVERFLOW;
    (*liver).avg_income = tmp2;

    return SUCCESS;
}

callback add_liver(Node **liver_data){
    if (!liver_data )
        return MEMORY_ERROR;

    Liver liver;
    int callback = get_liver(&liver);
    if (callback)
        return callback;

    if (append(liver_data, liver))
        return MEMORY_ERROR;

    return SUCCESS;
}

callback deleteNode(Node** head, const char* surname) {
    if (*head == NULL)
        return MEMORY_ERROR;

    Node* current = *head;
    Node* previous = NULL;

    while (current != NULL && strcmp(current->data.surname, surname) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
        return DONT_FIND;

    if (previous == NULL)
        *head = current->next;
    else
        previous->next = current->next;

    free(current);
    return SUCCESS;
}

callback del_liver(Node **liver_data){
    if (!liver_data )
        return MEMORY_ERROR;

    int size = getLength(*liver_data);
    char surname[128];
    printf("Enter surname for del: ");
    if (scanf("%127s", surname) != 1)
        return INPUT_ERROR;

    return deleteNode(liver_data, surname);
}

int compareDates(const char* date1, const char* date2) {
    int day1, month1, year1;
    int day2, month2, year2;

    sscanf(date1, "%d:%d:%d", &day1, &month1, &year1);
    sscanf(date2, "%d:%d:%d", &day2, &month2, &year2);

    if (year1 != year2) {
        return year1 - year2;
    }
    if (month1 != month2) {
        return month1 - month2;
    }
    return day1 - day2;
}

void sortByAge(Node** head) {
    if (*head == NULL) {
        return;
    }

    int swapped;
    Node *ptr1;
    Node *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;
        while (ptr1->next != lptr) {
            if (compareDates(ptr1->data.date_of_birth, ptr1->next->data.date_of_birth) > 0) {

                Liver temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

int is_same_file(char *input_file, char *output_file) {
    char buf1[PATH_MAX];
    char *res1 = realpath(input_file, buf1);
    char buf2[PATH_MAX];
    char *res2 = realpath(output_file, buf2);
    if (res1 != NULL && res2 != NULL){
        if (!strcmp(res1, res2)){
            return ERROR_THE_SAME_FILE;
        }
    }
    return SUCCESS;
}


callback write_to_file(Node *head, char *input_f){
    char filename[128];
    if (scanf("%127s", filename) != 1)
        return INPUT_ERROR;

    if (is_same_file(input_f, filename))
        return ERROR_THE_SAME_FILE;

    FILE* file = fopen(filename, "w");
    if (!file)
        return FILE_OPEN_ERROR;

    Node* current = head;
    while (current != NULL) {
        fprintf(file, "S: %s\n", current->data.surname);
        fprintf(file, "N: %s\n", current->data.name);
        fprintf(file, "P: %s\n", current->data.patronymic);
        fprintf(file, "D: %s\n", current->data.date_of_birth);
        fprintf(file, "G: %c\n", current->data.gender);
        fprintf(file, "A: %.2f\n\n", current->data.avg_income);
        current = current->next;
    }

    fclose(file);
    return SUCCESS;
}