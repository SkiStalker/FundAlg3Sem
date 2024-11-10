#ifndef UNIDIRECT_LIST_H
#define UNIDIRECT_LIST_H

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "return_code.h"

#define BUF_SIZE 256
#define EPS 1e-10

typedef struct Date
{
    int day, month, year;
} Date;

typedef struct Liver
{
    char surname[BUF_SIZE];
    char name[BUF_SIZE];
    char patronymic[BUF_SIZE];
    Date birth_date;
    char sex;
    double income;
} Liver;

typedef struct Node
{
    Liver data;
    struct Node *next;
} Node;

typedef struct UniList
{
    Node *head;
    size_t size;
} UniList;

enum return_code list_init(UniList *list);
size_t list_size(UniList *list);
int is_empty(UniList *list);
int date_cmp(const Date *date_1, const Date *date_2);
enum return_code list_insert(UniList *list, const Liver *liver);
enum return_code list_delete_node(UniList *list, const Liver *liver);
enum return_code list_find(UniList *list, const Liver *liver, Liver **found);
enum return_code liver_change(UniList *list, const Liver *to_change, const Liver *new_data);
enum return_code list_file_print(UniList *list, FILE *file);
enum return_code list_file_scan(UniList *list, FILE *file);
enum return_code liver_validate(const Liver *liver);
enum return_code list_push(UniList *list, const Liver *liver);
enum return_code list_pop(UniList *list, Liver *top_liver);
enum return_code list_destroy(UniList *list);

#endif