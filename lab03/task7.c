#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct tm tm;

typedef struct Human {
    char *last_name;
    char *first_name;
    char *patronymic;
    tm date_of_birth;
    char gender;
    float income;
} Human;


typedef int (*for_check_symbol)(int);

typedef enum OpType { OP_INSERT, OP_REMOVE, OP_UPDATE } OpType;

typedef struct Op {
    OpType type;
    size_t pos;
    Human l;
} Op;

typedef struct Db {
    size_t size;
    size_t cap;
    Human *ptr;

    size_t op_size;
    size_t op_cap;
    Op *ops;
} Db;


int func_for_string_parsing(char **res, char const **start, for_check_symbol m) {
    const char *end = *start;
    while (*end && !m(*end)) {
        end++;
    }
    int n = end - *start;
    *res = malloc(n + 1);
    if (!res)
        return -1;
    memcpy(*res, *start, n);
    (*res)[n] = 0;
    *start = end + 1;
    return 0;
}

int parse_field_uint(unsigned long *res, char const **start, for_check_symbol m) {
    char *ptr;
    *res = strtoul(*start, &ptr, 10);
    bool valid = *ptr == 0;
    valid = valid | m(*ptr);
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        fprintf(stderr, "ERROR: failed to parse [%s] as unsigned\n", *start);
        *ptr = tmp;
        return -1;
    }

    *start = ptr + 1;
    return 0;
}

int function_for_parse_float(float *res, char const **start, for_check_symbol m) {
    char *ptr;
    *res = strtof(*start, &ptr);
    bool valid = *ptr == 0;
    valid = valid | m(*ptr);
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        fprintf(stderr, "ERROR: failed to parse [%s] as float\n", *start);
        *ptr = tmp;
        return -1;
    }

    *start = ptr + 1;
    return 0;
}
int liver_free(Human *l) {
    if (l->last_name)
        free(l->last_name);
    if (l->first_name)
        free(l->first_name);
    if (l->patronymic)
        free(l->patronymic);
    return 0;
}

int parse_field_time(tm *res, const char **start, for_check_symbol m) {
    char *str;
    func_for_string_parsing(&str, start, m);
    char *last = strptime(str, "%d.%m.%Y", res);
    // if (last == NULL || *last != 0) {
    //     fprintf(stderr, "ERROR: failed to parse [%s] as date\n", str);
    //     free(str);
    //     return S_PARSE_ERROR;
    // }
    res->tm_isdst = -1;
    free(str);
    return 0;
}

int function_for_parse_char(char *res, const char **start, for_check_symbol m) {
    char *str;
    func_for_string_parsing(&str, start, m);
    if (strlen(str) != 1) {
        fprintf(stderr, "ERROR: expected char, got string [%s]\n", str);
        free(str);
        return -1;
    }
    *res = *str;
    free(str);
    return 0;
}

int validate_name(bool *res, const char *s) {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (!isalpha(s[i])) {
            *res = false;
            return 0;
        }
    }
    *res = true;
    return 0;
}

int parsing(Human *l, const char *str) {
    const char *s = str;
    int check_error = func_for_string_parsing(&l->last_name, &s, isspace);
    if (check_error) {
        printf("ERROR: failed to parse last name\n");
        liver_free(l);
        return -1;
    }
    check_error = func_for_string_parsing(&l->first_name, &s, isspace);
    if (check_error) {
        printf("ERROR: failed to parse first name\n");
        liver_free(l);
        return -1;
    }
    check_error = func_for_string_parsing(&l->patronymic, &s, isspace);
    if (check_error) {
        printf("ERROR: failed to parse patronymic\n");
        liver_free(l);
        return -1;
    }
    check_error = parse_field_time(&l->date_of_birth, &s, isspace);
    if (check_error) {
        printf("ERROR: failed to parse data of birth\n");
        liver_free(l);
        return -1;
    }
    check_error = function_for_parse_char(&l->gender, &s, isspace);
    if (check_error) {
        printf("ERROR: failed to parse GENDER\n");
        liver_free(l);
        return -1;
    }
    check_error = function_for_parse_float(&l->income, &s, isspace);
    if (check_error) {
        printf("ERROR: failed to parse last name\n");
        liver_free(l);
        return -1;
    }

    bool valid;
    validate_name(&valid, l->last_name);
    if (!valid || l->last_name[0] == 0) {
        printf("ERROR: last name not empty! and only latin: [%s]\n", l->last_name);
        liver_free(l);
        return -1;
    }
    validate_name(&valid, l->first_name);
    if (!valid || l->first_name[0] == 0) {
        printf("ERROR: first name not empty! and only latin: [%s]\n", l->first_name);
        liver_free(l);
        return -1;
    }
    validate_name(&valid, l->patronymic);
    if (!valid) {
    printf("patronymic not empty! and only latin [%s]\n", l->patronymic);
        liver_free(l);
        return -1;
    }
    if (l->gender != 'M' && l->gender != 'W') {
        printf("In Russia we have only 2 gender [%d]\n", l->gender);
        liver_free(l);
        return -1;
    }
    if (l->income < 0) {
        fprintf(stderr, "ERROR: income must be positive: [%f]\n", l->income);
        liver_free(l);
        return -1;
    }

    return 0;
}

int liver_write(const Human *l, FILE *f) {
    char t[20];
    strftime(t, 20, "%d.%m.%Y", &l->date_of_birth);
    fprintf(f, "%s %s %s %s %c %f\n", l->last_name, l->first_name,
            l->patronymic, t, l->gender, l->income);
    return 0;
}

int liver_print(const Human *l) {
    return liver_write(l, stdout);
}

int liver_copy(Human *t, const Human *l) {
    int n;
    n = strlen(l->first_name) + 1;
    t->first_name = malloc(n);
    if (t == NULL)
        return -1;
    memcpy(t->first_name, l->first_name, n);

    n = strlen(l->last_name) + 1;
    t->last_name = malloc(n);
    if (t == NULL)
        return -1;
    memcpy(t->last_name, l->last_name, n);

    n = strlen(l->patronymic) + 1;
    t->patronymic = malloc(n);
    if (t == NULL)
        return -1;
    memcpy(t->patronymic, l->patronymic, n);

    t->date_of_birth = l->date_of_birth;
    t->gender = l->gender;
    t->income = l->income;
    return 0;
}



int db_init(Db *d) {
    d->cap = 16;
    d->size = 0;
    d->ptr = malloc(d->cap * sizeof(Human));
    if (d->ptr == NULL)
        return -1;

    d->op_cap = 16;
    d->op_size = 0;
    d->ops = malloc(d->op_cap * sizeof(Op));
    if (d->ptr == NULL) {
        free(d->ptr);
        return -1;
    }
    return 0;
}

int db_push_op(Db *d, Op op) {
    if (d->op_size >= d->op_cap) {
        size_t new_cap = d->op_cap * 2;
        Op *t = realloc(d->ops, new_cap * sizeof(Op));
        if (t == NULL)
            return -1;
        d->op_cap = new_cap;
        d->ops = t;
    }
    d->ops[d->op_size] = op;
    d->op_size++;
    return 0;
}

int db_pop_op(Db *d, Op *op) {
    if (d->op_size == 0)
        return -1;
    d->op_size--;
    *op = d->ops[d->op_size];
    return 0;
}

int db_insert_data(Db *d, size_t i, Human l) {
    if (i > d->size)
        return -1;
    if (d->size >= d->cap) {
        size_t new_cap = d->cap * 2;
        Human *t = realloc(d->ptr, new_cap * sizeof(Human));
        if (t == NULL)
            return -1;
        d->cap = new_cap;
        d->ptr = t;
    }
    for (size_t j = d->size; j > i; j--)
        d->ptr[j] = d->ptr[j - 1];
    d->ptr[i] = l;
    d->size++;
    return 0;
}

int db_insert(Db *d, size_t i, Human l) {
    Human l2 = {0};
    Op op = {.type = OP_INSERT, .pos = i, .l = l2};
    int check_error = db_insert_data(d, i, l);
    if (check_error) {
        printf("I can`t insert data\n");
        return -1;
    }
    check_error = db_push_op(d, op);
    if (check_error) {
        printf("I can`t push op\n");
    }
    return 0;
}


int db_push(Db *d, Human l) {
    return db_insert(d, d->size, l);
}

int db_remove_data(Db *d, size_t i) {
    if (i >= d->size)
        return -1;
    for (size_t j = i; j < d->size - 1; j++) {
        d->ptr[j] = d->ptr[j + 1];
    }
    d->size--;
    return 0;
}

int db_remove(Db *d, size_t i) {
    if (i >= d->size)
        return -1;
    Op op = {.type = OP_REMOVE, .pos = i, .l = d->ptr[i]};
    int check_error = db_remove_data(d, i);
    if (check_error) {
        printf("I can`t remove data\n");
        return -1;
    }
    check_error = db_push_op(d, op);
    if (check_error) {
        printf("I can`t remove data\n");
        return -1;
    }
    return 0;
}

int db_update_data(Db *d, size_t i, Human l) {
    if (i >= d->size)
        return -1;
    d->ptr[i] = l;
    return 0;
}

int db_update(Db *d, size_t i, Human l) {
    if (i >= d->size)
        return -1;
    Human l_old = {0};
    liver_copy(&l_old, &d->ptr[i]);
    Op op = {.type = OP_UPDATE, .pos = i, .l = l_old};
    liver_free(&d->ptr[i]);
    int check_error = db_update_data(d, i, l);
    if (check_error) {
        printf("I can`t update data\n");
        return -1;
    }
    check_error = db_push_op(d, op);
    if (check_error) {
        printf("I can`t push data\n");
        return -1;
    }
    return 0;
}

int db_undo(Db *d) {
    Op op;
    int check_error = db_pop_op(d, &op);
    if (check_error) {
        printf("ERROR: impossible to undo, not actions were performed\n");
        return -1;
    }
    switch (op.type) {
    case OP_INSERT:
        check_error = db_remove_data(d, op.pos);
        if (check_error) {
            printf("I can`t remove data\n");
        }
        liver_free(&op.l);
        break;
    case OP_UPDATE:
        liver_free(&d->ptr[op.pos]);
        check_error = db_update_data(d, op.pos, op.l);
        if (check_error) {
            printf("I can`t update data\n");
        }
        break;
    case OP_REMOVE:
        check_error = db_insert_data(d, op.pos, op.l);
        if (check_error) {
            printf("I can`t insert data\n");
        }
        break;
    default:
        fprintf(stderr, "ERROR: unreachable\n");
    }
    return 0;
}

int db_read_file(Db *d, FILE *f) {
    char *line = NULL;
    size_t line_len = 0;
    while (1) {
        int n = getline(&line, &line_len, f);
        if (n <= 0) {
            break;
        }
            
        n--;
        line[n] = 0;
        const char *s = line;
        Human l = {0};
        int check_error = parsing(&l, s);
        if (check_error) {
            printf("I can`t parse line\n");
            free(line);
            return -1;
        }
        check_error = db_push(d, l);
        if (check_error) {
            printf("I can`t push line\n");
            free(line);
            return -1;
        }
    }
    free(line);
    return 0;
}

int db_free(Db *d) {
    for (size_t i = 0; i < d->size; i++) {
        liver_free(&d->ptr[i]);
    }
    for (size_t i = 0; i < d->op_size; i++) {
        liver_free(&d->ops[i].l);
    }
    free(d->ptr);
    free(d->ops);
    return 0;
}
int db_write(const Db *d, FILE *f) {
    for (size_t i = 0; i < d->size; i++) {
        liver_write(&d->ptr[i], f);
    }
    return 0;
}
int db_print(const Db *d) {
    for (size_t i = 0; i < d->size; i++) {
        printf("[%zu]:\t", i);
        liver_print(&d->ptr[i]);
    }
    printf("\n");
    return 0;
}


int print_output(Db *d, const char *s) {
    (void)s;
    return db_print(d);
}
int push(Db *d, const char *s) {
    Human l = {0};
    int check_error = parsing(&l, s);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    return db_push(d, l);
}

//remove index
int remove_data(Db *d, const char *s) {
    size_t i;
    int check_error = parse_field_uint(&i, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    if (i >= d->size) {
        fprintf(stderr,
                "ERROR: out of bounds: tried to remove index [%zu] in an array "
                "of length [%zu]\n",
                i, d->size);
        return -1;
    }
    return db_remove(d, i);
}

int handle_update_first_name(Human *l, const char *s) {
    free(l->first_name);
    int check_error = func_for_string_parsing(&l->first_name, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        liver_free(l);
        return -1;
    }
    bool valid;
    validate_name(&valid, l->first_name);
    if (!valid || l->first_name[0] == 0) {
        fprintf(stderr,
                "ERROR: first name must not be empty and can only "
                "contain latin symbols: [%s]\n",
                l->first_name);
        liver_free(l);
        return -1;
    }
    return 0;
}

int handle_update_last_name(Human *l, const char *s) {
    free(l->last_name);
    int check_error = func_for_string_parsing(&l->last_name, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        liver_free(l);
        return -1;
    }
    bool valid;
    validate_name(&valid, l->last_name);
    if (!valid || l->last_name[0] == 0) {
        fprintf(stderr,
                "ERROR: last name must not be empty and can only "
                "contain latin symbols: [%s]\n",
                l->last_name);
        liver_free(l);
        return -1;
    }
    return 0;
}

int handle_update_patronymic(Human *l, const char *s) {
    free(l->patronymic);
    int check_error = func_for_string_parsing(&l->patronymic, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        liver_free(l);
        return -1;
    }
    bool valid;
    validate_name(&valid, l->patronymic);
    if (!valid) {
        fprintf(stderr,
                "ERROR: patronymic can only "
                "contain latin symbols: [%s]\n",
                l->patronymic);
        liver_free(l);
        return -1;
    }
    return 0;
}

int handle_update_date_of_birth(Human *l, const char *s) {
    int check_error = parse_field_time(&l->date_of_birth, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        liver_free(l);
        return -1;
    }
    return 0;
}

int handle_update_gender(Human *l, const char *s) {
    int check_error = function_for_parse_char(&l->gender, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        liver_free(l);
        return -1;
    }
    if (l->gender != 'M' && l->gender != 'W') {
        fprintf(stderr, "ERROR: gender can be either 'M' or 'W': [%c]\n",
                l->gender);
        return -1;
    }
    return 0;
}

int handle_update_income(Human *l, const char *s) {
    int check_error = function_for_parse_float(&l->income, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        liver_free(l);
        return -1;
    }
    if (l->income < 0) {
        fprintf(stderr, "ERROR: income must be positive: [%f]\n", l->income);
        return -1;
    }
    return 0;
}

typedef int (*handle_liver_update)(Human *l, const char *s);

//update index ops znach
int update(Db *d, const char *s) {
    size_t i;
    char *op;
    int check_error = parse_field_uint(&i, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    check_error = func_for_string_parsing(&op, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    if (i >= d->size) {
        fprintf(stderr,
                "ERROR: out of bounds: tried to access index [%zu] in an array "
                "of length [%zu]\n",
                i, d->size);
        return -1;
    }
    Human l = {0};
    liver_copy(&l, &d->ptr[i]);
    const char *ops[] = {"first_name",    "last_name", "patronymic",
                         "date_of_birth", "gender",    "income"};
    handle_liver_update handles[] = {
        handle_update_first_name, handle_update_last_name,
        handle_update_patronymic, handle_update_date_of_birth,
        handle_update_gender,     handle_update_income};

    bool found = false;
    for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
        if (strcmp(op, ops[i]) == 0) {
            check_error = handles[i](&l, s);
            if (check_error) {
                fprintf(stderr, "ERROR: parsing failed\n");
                // liver_free(&l);
                // free(op);
                return -1;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "ERROR: unknown field [%s]\n", op);
        fprintf(stderr, "supported fields: {");
        for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
            fprintf(stderr, "%s, ", ops[i]);
        }
        fprintf(stderr, "}\n");
        // free(op);
        return -1;
    }

    free(op);
    return db_update(d, i, l);
}

//write filename
int write(Db *d, const char *s) {
    char *fname;
    int check_error = func_for_string_parsing(&fname, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    if (fname[0] == 0) {
        fprintf(stderr, "ERROR: filename not provided\n");
        free(fname);
        return -1;
    }
    FILE *f = fopen(fname, "w");
    if (!f) {
        fprintf(stderr, "ERROR: failed to open file [%s]\n", fname);
        free(fname);
        return -1;
    }
    db_write(d, f);
    fclose(f);
    free(fname);
    return 0;
}
int undo(Db *d, const char *s) {
    (void)s;
    return db_undo(d);
}

typedef int (*handle)(Db *f, const char *s);

// #define tm_compare(a, b) !(a.tm_year == b.tm_year && a.tm_yday == b.tm_yday)
// #define num_compare(a, b) a != b

int tm_compare(tm a, tm b) {
    return !(a.tm_year == b.tm_year && a.tm_yday == b.tm_yday);
}

int num_compare(char a, float b) {
    return a != b;
}


int handler_find_last_name(Db *d, const char *s) {                            
    char * a;                                                                
    int check_error = func_for_string_parsing(&a, &s, isspace);                             
    if (check_error) {                                                     
        fprintf(stderr, "ERROR: parsing failed\n");                        
        return -1;                                                         
    }                                                                      
    for (size_t i = 0; i < d->size; i++) {                                 
        Human l = d->ptr[i];                                               
        if (strcmp(a, l.last_name) == 0) {                                         
             printf("[%zu]:\t", i);                                         
            liver_print(&d->ptr[i]);                                       
        }                                                                 
    }                                                                      
    printf("\n");                                                          
    return 0;                                                              
}

int handler_find_first_name(Db *d, const char *s) {
    char * a;
    int check_error = func_for_string_parsing(&a, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    for (size_t i = 0; i < d->size; i++) {
        Human l = d->ptr[i];
        if (strcmp(a, l.first_name) == 0) {
            printf("[%zu]:\t", i);
            liver_print(&d->ptr[i]);
        }
    }
    printf("\n");
    return 0;
}

int handler_find_patronymic(Db *d, const char *s) {
    char * a;
    int check_error = func_for_string_parsing(&a, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    for (size_t i = 0; i < d->size; i++) {
        Human l = d->ptr[i];
        if (strcmp(a, l.patronymic) == 0) {
            printf("[%zu]:\t", i);
            liver_print(&d->ptr[i]);
        }
    }
    printf("\n");
    return 0;
}

int handler_find_date_of_birth(Db *d, const char *s) {
    tm a;
    int check_error = parse_field_time(&a, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    for (size_t i = 0; i < d->size; i++) {
        Human l = d->ptr[i];
        if (tm_compare(a, l.date_of_birth) == 0) {
            printf("[%zu]:\t", i);
            liver_print(&d->ptr[i]);
        }
    }
    printf("\n");
    return 0;
}

int handler_find_gender(Db *d, const char *s) {
    char a;
    int check_error = func_for_string_parsing(&a, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    for (size_t i = 0; i < d->size; i++) {
        Human l = d->ptr[i];
        if (num_compare(a, l.gender) == 0) {
            printf("[%zu]:\t", i);
            liver_print(&d->ptr[i]);
        }
    }
    printf("\n");
    return 0;
}

int handler_find_income(Db *d, const char *s) {
    float a;
    int check_error = function_for_parse_float(&a, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    for (size_t i = 0; i < d->size; i++) {
        Human l = d->ptr[i];
        if (num_compare(a, l.income) == 0) {
            printf("[%zu]:\t", i);
            liver_print(&d->ptr[i]);
        }
    }
    printf("\n");
    return 0;
}


//find ops znach
int find(Db *d, const char *s) {
    char *op;
    int check_error = func_for_string_parsing(&op, &s, isspace);
    if (check_error) {
        fprintf(stderr, "ERROR: parsing failed\n");
        return -1;
    }
    const char *ops[] = {"first_name",    "last_name", "patronymic",
                         "date_of_birth", "gender",    "income"};
    handle handles[] = {handler_find_first_name, handler_find_last_name,
                        handler_find_patronymic, handler_find_date_of_birth,
                        handler_find_gender,     handler_find_income};
    bool found = false;
    for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
        if (strcmp(op, ops[i]) == 0) {
            int check_error = handles[i](d, s);
            if (check_error) {
                fprintf(stderr, "ERROR: parsing failed\n");
                free(op);
                return -1;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "ERROR: unknown field [%s]\n", op);
        fprintf(stderr, "supported fields: {");
        for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
            fprintf(stderr, "%s, ", ops[i]);
        }
        fprintf(stderr, "}\n");
        free(op);
        return -1;
    }
    free(op);
    return 0;
}

int main(int argc, const char *argw[]) {
    if (argc != 2) {
        printf("You need to provide only input file\n");
        return 1;
    }
    FILE *file = fopen(argw[1], "r");
    if (!file) {
        printf("I can`t open file: [%s]\n", argw[1]);
        return 1;
    }
    const char *ops[] = {"Print",  "Write", "Push", "Remove", "Update", "Find",  "Undo"};
    handle handles[] = {print_output,  write, push, remove_data, update, find,  undo};
    
    printf("Hello, I can use next operations: \n");
    for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
        printf("%s ", ops[i]);
    }
    printf("\n");
    printf("\n");


    Db d = {0};
    int check_error = db_init(&d);
    if (check_error) {
        printf("I can`t init db\n");
        return 1;
    }
    db_read_file(&d, file);

    char *line = NULL;
    size_t line_len = 0;
    while (true) {
        int n = getline(&line, &line_len, stdin);
        if (n <= 0)
            break;
        n--;
        line[n] = 0;

        const char *s = line;
        char *op;
        check_error = func_for_string_parsing(&op, &s, isspace);
        if (check_error) {
            fprintf(stderr, "ERROR: parsing failed\n");
            free(op);
            return -1;
        }
        bool found = false;
        for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
            if (strcmp(op, ops[i]) == 0) {
                handles[i](&d, s);
                found = true;
                break;
            }
        }
        if (!found) {
            printf("I don`t understand operation [%s]\n", line);
            printf("I supported operations only next operations: ");
            for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
                printf("%s, ", ops[i]);
            }
            printf("\n");
        }
        free(op);
    }
    free(line);

    db_free(&d);

    fclose(file);
    return 0;
}