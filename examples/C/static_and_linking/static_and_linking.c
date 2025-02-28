#include "functions.h"
#include <stdio.h>

/*  
    The main idea of this example is that the .h file is just 
    a assumption that the functions, variables, and types described 
    in it will eventually appear somewhere later.

    The actual substitution of functions, vars and types occurs 
    only at the linking stage.
*/

int main()
{

    my_struct *s1 = create_my_struct(1, 2);
    my_struct *s2 = create_my_struct(2, 3);

    printf("%f\n", sum_structs(s1, s2));

#ifdef MY_ERROR
    create_free_cnt++;
    create_free_cnt--;
    double res = 0;
    __sum(s1, s2, &res);
    printf("%f\n", res);
#endif

    free_my_struct(s1);
    free_my_struct(s2);
    return 0;
}