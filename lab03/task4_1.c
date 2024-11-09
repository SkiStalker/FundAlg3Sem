#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct String {
    int len;
    char *ptr;
} String;

int create_str(String *s, const char *str) {
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

int delete_str(String *s) {
    if (s->ptr)
        free(s->ptr);
    s->ptr = NULL;
    s->len = 0;
    return 0;
}

int comparable_string(const String *a, const String *b) {
    if (a->len != b->len) {
        if (a->len < b->len) {
            return -1;
        } else {
            return 1;
        }
    }
    for (int i = 0; i < a->len; i++) {
        if (a->ptr[i] != b->ptr[i]) {
            if (a->ptr[i] < b->ptr[i]) {
                return -1;
            } else {
                return 1;
            }
        }
    }
    return 0;
}

/*
    int strcmp(const void *a, const void *b) {
        while (*a & (*a == *b)) {
            *a++;
            *b++;
        }

        return *(const unsigned char*)a - *(const unsigned char*)b
    }
*/

int string_equal(const String *a, const String *b) {
    int res = comparable_string(a, b);
    if (res == 0) {
        printf("Strings is equal\n");
    } else {
        printf("Strings is not equal\n");
    } 
    return 0;
}

int copy_strings(String *a, const String *b) {
    if (a->len || a->ptr)
        delete_str(a);
    return create_str(a, b->ptr);
}

int string_create_copy(String **a, const String *b) {
    *a = malloc(sizeof(String));
    if (*a == NULL)
        return -1;
    return copy_strings(*a, b);
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

int comparable(const String *a, const String *b) {
    if (comparable_string(a, b) < 0) {
        printf("Second String is greater\n");
        return 0;
    } else if (comparable_string(a, b) > 0) {
        printf("First String is greater\n");
        return 0;
    }
    printf("Strings are equal\n");
    return 0;
}


int main(void) {
    String a = {0};
    String b = {0};
    String c = {0};
    create_str(&a, "First");
    create_str(&b, "Second");
    create_str(&c, "Third");
    
    String d = {0};
    copy_strings(&d, &a);
    printf("copy: [%s]\n", d.ptr);
    delete_str(&d);

    String *e;
    string_create_copy(&e, &a);
    printf("copy into malloc: [%s]\n", e->ptr);
    delete_str(e);
    free(e);

    string_concatenate(&a, &b);
    string_concatenate(&a, &c);
    printf("concatenated [%s]\n", a.ptr);

    delete_str(&a);
    delete_str(&b);
    delete_str(&c);

    create_str(&a, "12345679");
    create_str(&b, "12345678");
    create_str(&c, "123456780");
    create_str(&d, "123456780");
    printf("\n");
    printf("a = [%s]\n", a.ptr);
    printf("b = [%s]\n", b.ptr);
    printf("c = [%s]\n", c.ptr);
    printf("d = [%s]\n", c.ptr);
    
    printf("cmp(a, b)\n");
    comparable(&a, &b);
    printf("cmp(a, c)\n");
    comparable(&a, &c);
    printf("cmp(c, d)\n");
    comparable(&c, &d);
    printf("\n");

    printf("equal(a, b)\n");
    string_equal(&a, &b);
    printf("equal(c, d)\n");
    string_equal(&c, &d);

    return 0;
}