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

typedef struct String
{
    char *arr;
    size_t capacity;
    size_t len;
} String;

typedef struct Node
{
    String data;
    size_t count_repeats;
    struct Node *left, *right;
} Node;

typedef struct Tree
{
    Node *root;
    int (*comparator)(const String *, const String *);
} Tree;

typedef struct ListNode
{
    Node *data;
    struct ListNode *next;
} ListNode;

typedef struct List
{
    ListNode *head;
    size_t size;
} List;

enum return_code init_tree(Tree *tree);
enum return_code delete_tree(Tree *tree);
enum return_code build_tree_from_file(Tree *tree, const char *separators, FILE *input);
enum return_code find_string(const String *str, Tree *tree, Node **found);
enum return_code find_longest_word(Tree *tree, String *str);
enum return_code find_shortest_word(Tree *tree, String *str);
int max_depth(Node *root);
enum return_code print_n_often(Tree *tree, int n);
enum return_code upload_tree_to_file(Tree *tree, FILE *file, char separator);
enum return_code list_destroy(List *list);
void print_menu();
void free_buf();
enum return_code string_scan(String *str);
enum return_code delete_string_content(String *string);
#endif