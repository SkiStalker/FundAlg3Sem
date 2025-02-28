
#include <stdlib.h>
#include <assert.h>

// !!!! note that #include "test_functions.h" there is no !!!!

typedef struct _my_struct
{
    double x1;
    double x2;
} my_struct;

static int create_free_cnt = 0;

int pub_sum_cnt = 0;

static int __sum(const my_struct *first, const my_struct *second, double *sum_res)
{
    *sum_res = first->x1 + first->x2 + second->x1 + second->x2;
    return 0;
}

my_struct* create_my_struct(double x1, double x2)
{
    create_free_cnt++;

    my_struct* m = (my_struct*)malloc(sizeof( my_struct));
    m->x1 = x1;
    m->x2 = x2;
    return m;
}

#ifdef MY_ERROR
// compiling error: incomplete type isn't allowed
my_struct create_my_struct_inplace(double x1, double x2)
{
    create_free_cnt++;
    my_struct m = {.x1 = x1, .x2 = x2};
    return m;
}
#endif

double sum_structs(const my_struct *first, const my_struct *second)
{
    static int sum_cnt = 0;
    pub_sum_cnt++;
    double res = 0;
    if (__sum(first, second, &res))
    {
        return 0;
    }

    return res + sum_cnt++;
}

void free_my_struct(my_struct* s)
{
    create_free_cnt--;
    free(s);
    assert(create_free_cnt >= 0);
}

int get_created_structs_count()
{
    return create_free_cnt;
}