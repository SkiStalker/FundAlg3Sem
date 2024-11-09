#ifndef UNTITLED1_LIVER_H
#define UNTITLED1_LIVER_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"
#include "stdbool.h"
#include "ctype.h"
#include <limits.h>

#define MAX_INPUT_SIZE 100
#define MAX_UNDO 100

typedef enum Callback {
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    OVERFLOW,
    DONT_FIND,
    FILE_OPEN_ERROR,
    ERROR_THE_SAME_FILE
} callback;

typedef struct Liver{
    char surname[128];
    char name[128];
    char patronymic[128];
    char date_of_birth[12];
    char gender;
    double avg_income;
} Liver;

typedef struct Node {
    Liver data;
    struct Node *next;
} Node;

typedef struct UndoStack {
    Node* states[MAX_UNDO];
    int top;
} UndoStack;

callback read_liver(Node **liver_data, FILE *in);
bool is_number(const char *str);
callback validate_date(const char *date);
callback validate_data(char tmp[6][128]);
Node* createNode(Liver liver);
callback append(Node **head, Liver liver);
void displayList(Node *node);
void menu();
callback search_by_surname(Node *liver_data, Liver **arr, int *arr_size);
void print_stud(Liver liver);
callback search_by_name(Node *liver_data, Liver **arr, int *arr_size);
callback search_by_patronymic(Node *liver_data, Liver **arr, int *arr_size);
callback upd_liver(Node **liver_data);
callback get_liver(Liver *liver);
callback add_liver(Node **liver_data);
callback del_liver(Node **liver_data);
void sortByAge(Node** head);
callback write_to_file(Node *head, char *input_f);
void initUndoStack(UndoStack* stack);
callback pushUndoState(UndoStack* stack, Node* head);
Node* popUndoState(UndoStack* stack);
void freeUndoStack(UndoStack* stack);
void freeList(Node* head);


#endif
