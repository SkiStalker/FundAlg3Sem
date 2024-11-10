#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "return_code.h"
#include "unidirect_list.h"

typedef struct SNode
{
    char c;
    struct SNode *next;
} SNode;

typedef struct Stack
{
    SNode *head;
    size_t size;
} Stack;

void print_menu();
enum return_code get_liver(Liver *liver);
enum return_code stack_init(Stack *stack);
enum return_code stack_push(Stack *stack, char c);
enum return_code stack_pop(Stack *stack, char *top);
enum return_code stack_destroy(Stack *stack);
void free_buf();

#endif