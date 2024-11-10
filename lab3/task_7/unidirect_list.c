#include "unidirect_list.h"

enum return_code list_init(UniList *list) {
    if (!list)
        return MEMORY_ERROR;
    list->head = NULL;
    list->size = 0;
    return OK;
}

size_t list_size(UniList *list) {
    return list->size;
}

int is_empty(UniList *list) {
    return !list->size;
}

int date_cmp(const Date *date_1, const Date *date_2) {
    if (date_1->year != date_2->year)
        return date_1->year - date_2->year;

    if (date_1->month != date_2->month) {
        return date_1->month - date_2->month;
    }
    return date_1->day - date_2->day;
}

enum return_code list_insert(UniList *list, const Liver *liver) {
    Node *new_node, *temp;
    size_t i;
    if (!list || !liver)
        return MEMORY_ERROR;

    new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
        return MEMORY_ERROR;

    new_node->data = *liver;
    new_node->next = NULL;

    if (list->size == 0) {
        list->head = new_node;
        list->size++;
        return OK;
    }
    if (date_cmp(&list->head->data.birth_date, &new_node->data.birth_date) <= 0) {
        new_node->next = list->head;
        list->head = new_node;
        list->size++;
        return OK;
    }
    temp = list->head;
    for (i = 0; i < list->size; ++i) {
        if (date_cmp(&temp->data.birth_date, &new_node->data.birth_date) >= 0 && (!temp->next || date_cmp(&temp->next->data.birth_date, &new_node->data.birth_date) <= 0)) {
            new_node->next = temp->next;
            temp->next = new_node;
            list->size++;
            return OK;
        }
        temp = temp->next;
    }
    return OK;
}

int liver_equal(const Liver *liver_1, const Liver *liver_2) {
    Date date_1, date_2;
    if (fabs(liver_1->income - liver_2->income) > EPS)
        return 0;
    date_1 = liver_1->birth_date;
    date_2 = liver_2->birth_date;

    if (date_1.day != date_2.day || date_1.month != date_2.month || date_1.year != date_2.year)
        return 0;
    if (liver_1->sex != liver_2->sex ||
        strcmp(liver_1->name, liver_2->name) ||
        strcmp(liver_1->patronymic, liver_2->patronymic) ||
        strcmp(liver_1->surname, liver_2->surname))
        return 0;
    return 1;
}

enum return_code list_delete_node(UniList *list, const Liver *liver) {
    size_t i;
    Node *temp, *cur_node;
    if (!list || !liver)
        return MEMORY_ERROR;

    if (is_empty(list))
        return MISSING_ERROR;

    if (liver_equal(liver, &list->head->data)) {
        temp = list->head;
        list->head = list->head->next;
        free(temp);
        list->size--;
        return OK;
    }
    cur_node = list->head;
    for (i = 0; i < list->size - 1; ++i) {
        if (liver_equal(&cur_node->next->data, liver)) {
            temp = cur_node->next;
            cur_node->next = cur_node->next->next;
            free(temp);
            list->size--;
            return OK;
        }
        cur_node = cur_node->next;
    }
    return MISSING_ERROR;
}

enum return_code list_find(UniList *list, const Liver *liver, Liver **found) {
    size_t i;
    Node *temp;
    *found = NULL;
    if (!list || !liver || !found)
        return MEMORY_ERROR;

    temp = list->head;
    for (i = 0; i < list->size; ++i) {
        if (liver_equal(liver, &temp->data)) {
            *found = &temp->data;
            return OK;
        }
        temp = temp->next;
    }
    return MISSING_ERROR;
}

enum return_code liver_change(UniList *list, const Liver *to_change, const Liver *new_data) {
    Liver *update_liver;
    if (!list || !to_change || !new_data)
        return MEMORY_ERROR;

    if (date_cmp(&to_change->birth_date, &new_data->birth_date) == 0) {
        if (list_find(list, to_change, &update_liver) == MISSING_ERROR)
            return MISSING_ERROR;

        *update_liver = *new_data;
    } else {
        if (list_delete_node(list, to_change) == MISSING_ERROR)
            return MISSING_ERROR;
        if (list_insert(list, new_data) == MEMORY_ERROR)
            return MEMORY_ERROR;
    }
    return OK;
}

enum return_code list_file_print(UniList *list, FILE *file) {
    Node *temp;
    size_t i;
    if (!list || !file)
        return MEMORY_ERROR;

    temp = list->head;
    for (i = 0; i < list->size; ++i) {
        fputs("_______________________________________________\n", file);
        fprintf(file, "Surname: %s;\nName: %s;\nPatronymic: %s;\nBirth date: %02d.%02d.%04d;\nSex: %c;\nIncome: %.3lf;\n",
                temp->data.surname, temp->data.name, temp->data.patronymic, temp->data.birth_date.day, temp->data.birth_date.month, temp->data.birth_date.year, temp->data.sex, temp->data.income);
        temp = temp->next;
        fputs("_______________________________________________\n", file);
    }

    return OK;
}

enum return_code validate_names(const char *name) {
    size_t i, len = strlen(name);
    if (!len)
        return BAD_INPUT_ERROR;

    for (i = 0; i < len; ++i) {
        if (!isalpha(name[i]))
            return BAD_INPUT_ERROR;
    }
    return OK;
}

enum return_code validate_date(const Date *date) {
    int month = date->month, day = date->day, year = date->year;
    
    if (month < 1 || month > 12 || day < 1 || year < 1900 || year > 2025) {
        return BAD_INPUT_ERROR;
    }

    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        if (day > 31) {
            return BAD_INPUT_ERROR;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return BAD_INPUT_ERROR;
        }
    }
    else if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) {
                return BAD_INPUT_ERROR;
            }
        } else {
            if (day > 28) {
                return BAD_INPUT_ERROR;
            }
        }
    }
    return OK;
}

enum return_code liver_validate(const Liver *liver) {
    if (validate_names(liver->name) || validate_names(liver->surname))
        return BAD_INPUT_ERROR;
    if (strlen(liver->patronymic) && validate_names(liver->patronymic))
        return BAD_INPUT_ERROR;

    if (validate_date(&liver->birth_date)) {
        return BAD_INPUT_ERROR;
    }
    if (liver->income < 0) {
        return BAD_INPUT_ERROR;
    }
    if (liver->sex != 'M' && liver->sex != 'W')
        return BAD_INPUT_ERROR;

    return OK;
}

enum return_code list_file_scan(UniList *list, FILE *file) {
    int count_read, patron_read;
    Liver temp;
    if (!list || !file)
        return MEMORY_ERROR;

    do {
        count_read = 0;
        count_read += fscanf(file, "Surname: %[^;]255s", temp.surname);
        getc(file);
        getc(file);
        count_read += fscanf(file, "Name: %[^;]255s", temp.name);
        getc(file);
        getc(file);
        patron_read = fscanf(file, "Patronymic: %[^;]255s", temp.patronymic);
        if (patron_read == 0) {
            temp.patronymic[0] = '\0';
        }
        count_read++;
        getc(file);
        getc(file);
        count_read += fscanf(file, "Birth date: %d.%d.%d;\n", &temp.birth_date.day, &temp.birth_date.month, &temp.birth_date.year);
        count_read += fscanf(file, "Sex: %c;\n", &temp.sex);
        count_read += fscanf(file, "Income: %lf;\n", &temp.income);
        if (count_read != 8 || liver_validate(&temp))
            continue;
        if (count_read == 8)
            list_insert(list, &temp);
    } while (count_read == 8);
    if (count_read > 0 && count_read < 8)
        return BAD_INPUT_ERROR;
    return OK;
}

enum return_code list_push(UniList *list, const Liver *liver) {
    Node *new_node;
    if (!list || !liver)
        return MEMORY_ERROR;

    new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
        return MEMORY_ERROR;

    new_node->data = *liver;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
    return OK;
}

enum return_code list_pop(UniList *list, Liver *top_liver) {
    Node *temp;
    if (!list || !top_liver)
        return MEMORY_ERROR;

    *top_liver = list->head->data;
    temp = list->head;
    list->head = temp->next;
    free(temp);
    list->size--;
    return OK;
}

enum return_code list_destroy(UniList *list) {
    Node *temp, *next;
    if (!list)
        return MEMORY_ERROR;

    temp = list->head;
    while (temp) {
        next = temp->next;
        free(temp);
        temp = next;
    }
    list->head = NULL;
    list->size = 0;
    return OK;
}