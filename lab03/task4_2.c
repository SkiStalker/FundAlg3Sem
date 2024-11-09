#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>

typedef struct String {
    int len;
    char *ptr;
} String;

typedef struct Address {
    String city;
    String street;
    unsigned building;
    String block;
    unsigned flat;
    String id;
} Address;

typedef struct Mail {
    Address addr;
    float weight;
    String id;
    String create;
    String receive;
} Mail;

typedef struct Post {
    Address *current;
    size_t size;
    size_t capacity;
    Mail *mail;
} Post;

int create_string_from_str(String *s, const char *str) {
    if (s->len || s->ptr) {
        return -1;
    }
    s->len = strlen(str);
    s->ptr = malloc(s->len + 1);
    if (!s->ptr)
        return -1;
    memcpy(s->ptr, str, s->len + 1);
    return 0;
}

int string_free(String *s) {
    if (s->ptr)
        free(s->ptr);
    s->ptr = NULL;
    s->len = 0;
    return 0;
}

int compare(int *res, const String *a, const String *b) {
    if (a->len != b->len) {
        *res = a->len < b->len ? -1 : 1;
        return 0;
    }
    for (int i = 0; i < a->len; i++) {
        if (a->ptr[i] != b->ptr[i]) {
            *res = a->ptr[i] < b->ptr[i] ? -1 : 1;
            return 0;
        }
    }
    *res = 0;
    return 0;
}

int compare_str(int *res, const String *a, const char *b) {
    String bs = {0};
    create_string_from_str(&bs, b);
    int r = compare(res, a, &bs);
    string_free(&bs);
    return r;
}

int equal(bool *res, const String *a, const String *b) {
    int cmp;
    compare(&cmp, a, b);
    *res = cmp == 0;
    return 0;
}

int equal_str(bool *res, const String *a, const char *b) {
    int cmp;
    compare_str(&cmp, a, b);
    *res = cmp == 0;
    return 0;
}

int copy_strings(String *a, const String *b) {
    if (a->len || a->ptr)
        string_free(a);
    return create_string_from_str(a, b->ptr);
}


int string_concatenate(String *a, const String *b) {
    if (a->len == 0)
        return copy_strings(a, b);
    char *tmp = realloc(a->ptr, a->len + b->len + 1);
    if (tmp == NULL)
        return -1;
    a->ptr = tmp;
    memcpy(a->ptr + a->len, b->ptr, b->len + 1);
    a->len = a->len + b->len;
    return 0;
}

/*
    char* strchr(const char* string, int c) {
        int i = 0;
        while (string[i] & (string[i] != c)) {
            c++;
        }
        return c == string[i] ? (char*)string + i: NULL;
    }
*/

int for_parse_str_from_input(String *res, char **start, const char *sep) {
    char *end = strchr(*start, sep[0]);
    if (!end) {
        end = *start + strlen(*start);
    }
    char tmp = *end;
    *end = 0;
    int r = create_string_from_str(res, *start);
    *end = tmp;
    *start = end + 1;
    return r;
}

/*
    unsigned long strtoul(const char *nptr, char **endptr, int base) {
        //определение базы (в данном случае передается), скип пробелов, знак

        while (*nptr) {
            int digit = (isdigit((unsigned char)*prt) ? *ptr - '0' :
            (tolower((unsigned char)*prt) - 'a' + 10))
        }
    }
*/

int for_parse_uint_from_input(unsigned *res, char **start, const char *sep) {
    char *ptr;
    *res = strtoul(*start, &ptr, 10);
    bool valid = *ptr == 0;
    for (; !valid && *sep; sep++) {
        if (*sep == *ptr) {
            valid = true;
        }
    }
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        printf("I don`t know how to parse [%s] as unsigned\n", *start);
        *ptr = tmp;
        return -1;
    }

    *start = ptr + 1;
    return 0;
}

int parse_time(time_t *t, const String *a) {
    if (a == NULL || a->ptr == NULL) {
        printf("Error: input string is NULL\n");
        return -1;
    }

    struct tm t0 = {0};
    char *last = strptime(a->ptr, "%d:%m:%Y %H:%M:%S", &t0);
    
    
    // if (last == NULL || *last != 0) {
    //     printf("I can`t parse time from [%s]\n", a->ptr);
    //     return -1;
    // }
    
    *t = mktime(&t0);
    
    if (*t == (time_t)-1) {
        printf("Failed to convert parsed time to time_t\n");
        return -1;
    }

    return 0;
}


int for_weight_input(float *res, char **start, const char *sep) {
    char *ptr;
    *res = strtof(*start, &ptr);
    bool valid = *ptr == 0;
    for (; !valid && *sep; sep++) {
        if (*sep == *ptr) {
            valid = true;
        }
    }
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        printf("I don`t know how to parse [%s] as float\n", *start);
        *ptr = tmp;
        return -1;
    }
    *start = ptr + 1;
    return 0;
}

int free_addr(Address *a) {
    string_free(&a->id);
    string_free(&a->city);
    string_free(&a->street);
    string_free(&a->block);
    return 0;
}

int mail_free(Mail *m) {
    string_free(&m->receive);
    string_free(&m->create);
    string_free(&m->id);
    free_addr(&m->addr);
    return 0;
}

int for_string_input(Address *a, char **s) {
    memset(a, 0, sizeof(Address));
    for_parse_str_from_input(&a->city, s, ";");
    for_parse_str_from_input(&a->street, s, ";");
    for_parse_uint_from_input(&a->building, s, ";");
    for_parse_str_from_input(&a->block, s, ";");
    for_parse_uint_from_input(&a->flat, s, ";");
    for_parse_str_from_input(&a->id, s, ";");
    if (a->city.len == 0) {
        printf("Please, enter correct city, example: Kyiv;\n");
        free_addr(a);
        return -1;
    }
    if (a->street.len == 0) {
        printf("Please, enter correct street, example: Lenina;\n");
        free_addr(a);
        return -1;
    }
    if (a->building == 0) {
        printf("Please, enter correct building number, example: 1;\n");
        free_addr(a);
        return -1;
    }
    if (a->flat == 0) {
        printf("Please, enter correct flat number, example: 5;\n");
        free_addr(a);
        return -1;
    }
    if (a->id.len != 6) {
        printf("mail index must be 6 symbols, got [%d] [%s]\n", a->id.len, a->id.ptr);
        free_addr(a);
        return -1;
    }
    return 0;
}

int mail_from_string(Mail *m, char **s) {

    memset(m, 0, sizeof(Mail));
    for_string_input(&m->addr, s);
    for_weight_input(&m->weight, s, ";");
    for_parse_str_from_input(&m->id, s, ";");
    for_parse_str_from_input(&m->create, s, ";");
    for_parse_str_from_input(&m->receive, s, ";");
    int read = 0;
    time_t t;
    sscanf(m->create.ptr, "%*2d:%*2d:%*4d %*2d:%*2d:%*2d%n", &read);
    if (read != 19 || parse_time(&t, &m->create) != 0) {
        printf("I don`t know how to parse [%s] as date\n", m->create.ptr);
        mail_free(m);
        return -1;
    }
    sscanf(m->receive.ptr, "%*2d:%*2d:%*4d %*2d:%*2d:%*2d%n", &read);
    if (read != 19 || parse_time(&t, &m->receive) != 0) {
        printf("I don`t know how to parse [%s] as date\n", m->receive.ptr);
        mail_free(m);
        return -1;
    }
    if (m->weight < 0) {
        mail_free(m);
        printf("weight must be >= 0\n");
        return -1;
    }
    if (m->id.len != 14) {
        mail_free(m);
        printf("I can`t parse this count of symbols as index, expected 14,1 got [%d] [%s]\n",
                m->id.len, m->id.ptr);
        return -1;
    }
    printf("New data added\n");
    return 0;
}

int mail_print(const Mail *m) {
    printf("City: %s Street: %s Building: %u Block: %s Flat: %u ID_addr: %s] ", m->addr.city.ptr, m->addr.street.ptr,
           m->addr.building, m->addr.block.ptr, m->addr.flat, m->addr.id.ptr);
    printf("Weight: %f ID_post: %s Created: %s Received: %s\n", m->weight, m->id.ptr,
           m->create.ptr, m->receive.ptr);

    return 0;
}

int compare_id(const void *a0, const void *b0) {
    const Mail *a = a0;
    const Mail *b = b0;
    int res;
    compare(&res, &a->addr.id, &b->addr.id);
    if (res != 0)
        return res;
    compare(&res, &a->id, &b->id);
    return res;
}

int compare_date(const void *a0, const void *b0) {
    const Mail *a = a0;
    const Mail *b = b0;
    time_t ta;
    time_t tb;
    parse_time(&ta, &a->receive);
    parse_time(&tb, &b->receive);
    int diff = difftime(ta, tb);
    if (diff == 0)
        return 0;
    if (diff < 0)
        return -1;
    return 1;
}


int post_init(Post *p) {
    p->current = NULL;
    p->capacity = 16;
    p->size = 0;
    p->mail = malloc(p->capacity * sizeof(Mail));
    if (p->mail == NULL)
        return -1;
    return 0;
}

int post_print(const Post *p) {
    for (size_t i = 0; i < p->size; i++) {
        printf("№%zu\t", i);
        mail_print(&p->mail[i]);
    }
    printf("\n");
    return 0;
}
int post_add(Post *p, Mail m) {
    if (p->size >= p->capacity) {
        size_t new_capacity = p->capacity * 2;
        Mail *tmp = realloc(p->mail, new_capacity);
        if (!tmp)
            return -1;
        p->mail = tmp;
        p->capacity = new_capacity;
    }
    p->mail[p->size] = m;
    p->size++;
    return 0;
}
int post_remove(Post *p, size_t i) {
    if (i >= p->size)
        return -1;
    mail_free(&p->mail[i]);
    for (size_t j = i; j < p->size - 1; j++) {
        p->mail[j] = p->mail[j + 1];
    }
    p->size--;
    return 0;
}

int post_sort(Post *p) {
    qsort(p->mail, p->size, sizeof(Mail), compare_id);
    return 0;
}

int post_filter_by_delivery_status(Post *p, const Post *p0, bool delivered) {
    time_t now = time(NULL);
    time_t t;
    for (size_t i = 0; i < p0->size; i++) {
        Mail m = p0->mail[i];
        if (m.receive.len == 0) {
            if (!delivered) {
                post_add(p, m);
            }
            continue;
        }
        if (parse_time(&t, &m.receive) == 0) {
            if ((difftime(t, now) <= 0) ^ !delivered) {
                post_add(p, m);
            }
        }
    }
    qsort(p->mail, p->size, sizeof(Mail), compare_date);
    return 0;
}

int post_free(Post *p) {
    for (size_t i = 0; i < p->size; i++) {
        mail_free(&p->mail[i]);
    }
    free(p->mail);
    return 0;
}

int for_add(Post *p, char *s) {
    int r;
    Mail m;
    char *start = s;
    if ((r = mail_from_string(&m, &s)) != 0) {
        printf("I don't know how to parse [%s] as mail\n", start);
        return r;
    }
    if ((r = post_add(p, m)) != 0) {
        printf("Error adding mail\n");
        return r;
    }
    return 0;
}

int for_remove(Post *p, char *s) {
    int r;
    char *start = s;
    unsigned i;
    if ((r = for_parse_uint_from_input(&i, &s, " ")) != 0) {
        fprintf(stderr, "ERROR: failed to parse [%s] as unsigned\n", start);
        return r;
    }
    if ((r = post_remove(p, i)) != 0) {
        fprintf(stderr, "ERROR: failed to remove mail to post\n");
        return r;
    }
    return 0;
}

int for_sort(Post *p, char *s) {
    return post_sort(p);
}

int for_print(Post *p, char *s) {
    return post_print(p);
}

int handle_delivered_status(Post *p, bool delivered) {
    Post deliv = {0};
    post_init(&deliv);
    post_filter_by_delivery_status(&deliv, p, delivered);
    post_print(&deliv);
    free(deliv.mail);
    return 0;
}

int for_delivered(Post *p, char *s) {
    (void)s;
    return handle_delivered_status(p, true);
}

int for_error(Post *p, char *s) {
    (void)s;
    return handle_delivered_status(p, false);
}

int for_id(Post *p, char *s) {
    String id = {0};
    int r;
    if ((r = for_parse_str_from_input(&id, &s, " ")) != 0) {
        fprintf(stderr, "ERROR: failed to parse [%s] as unsigned\n", s);
        return r;
    }
    bool eq = false;
    for (size_t i = 0; i < p->size; i++) {
        equal(&eq, &p->mail[i].id, &id);
        if (!eq)
            continue;
        time_t now = time(NULL);
        time_t t;
        parse_time(&t, &p->mail[i].receive);

        if (difftime(t, now) <= 0)
            printf("mail was delivered\n");
        else
            printf("mail was not delivered\n");
        break;
    }
    string_free(&id);
    if (!eq) {
        printf("mail not found\n");
    }

    return 0;
}

typedef int (*handle)(Post *p, char *s);

int main() {
    const char *commands[] = {"Add", "Remove", "Sort", "Print", "Id",
    "Delivered", "NotDelivered"};
    handle functions[] = {for_add, for_remove, for_sort, for_print,
    for_id, for_delivered, for_error};

    size_t line_len = 0;
    char *line = NULL;
    Post p = {0};
    if (post_init(&p)) {
        printf("I don`t have memory");
        return 1;
    }
    printf("Hello, I can use next operations: \n");
    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        printf("%s ", commands[i]);
    }
    printf("\n");
    printf("Format for add data: city;street;building;block;flat;id_addr;weight;id_post;create;receive\n");
    printf("Example: City;Street;1;one;1;123456;1;12345678912345;01:01:2024 01:01:01;02:02:2024 02:02:02;\n");
    printf("\n");
    printf("\n");
    while (1) {
        String op = {0};
        int n = getline(&line, &line_len, stdin);
        if (n <= 0) {
            break;
        }
        n--;
        line[n] = 0;
        char *s = line;
        for_parse_str_from_input(&op, &s, " ");
        bool res = false;
        for (size_t i = 0; i < (sizeof(functions) / sizeof(functions[0])); i++) {
            equal_str(&res, &op, commands[i]);
            if (res) {
                functions[i](&p, s);
                break;
            }
        }
        if (!res) {
            printf("I don`t know this operation\n");
            printf("I know only next operations: ");
            for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
                printf("%s ", commands[i]);
            }
            printf("\n");
        }
        string_free(&op);
    }
    if (line)
        free(line);
    post_free(&p);
    return 0;
}