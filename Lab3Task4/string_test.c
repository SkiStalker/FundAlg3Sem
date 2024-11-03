#include "String.h"
#include <stdio.h>


int main(){
    String s1, s2;
    String_init(&s1, "123");
    String_init(&s2, "123");
    printf("%d\n", String_isEqual(&s1, &s2));
    String_concat(&s1, &s2);
    printf("%s\n", s1.data);
    String_assignment(&s2, "321");
    printf("%s\n", s2.data);
    printf("%d\n", String_gtr(&s2, &s1));
    String_delete(&s1);
    String_delete(&s2);
}