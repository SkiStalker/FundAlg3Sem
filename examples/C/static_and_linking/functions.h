struct _my_struct;
typedef struct _my_struct my_struct;

extern int pub_sum_cnt;
my_struct* create_my_struct(double x1, double x2);
double sum_structs(const my_struct* first, const my_struct* second);
void free_my_struct(my_struct* s);
int get_created_structs_count();

// there we try to export static functions and vars
#ifdef MY_ERROR

// if we want to use this features, we will get a compiling error.
my_struct create_my_struct_inplace(double x1, double x2);


// if we want to use this features, we will get a linking error.
extern int create_free_cnt;
int __sum(const my_struct *first, const my_struct *second, double *sum_res); 

#endif


