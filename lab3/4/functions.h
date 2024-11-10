#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "my_string.h"

typedef struct address
{
    String city, street, building, index;
    unsigned int house_number, apartment_number;
} Address;

typedef struct mail
{
    Address address;
    double weight;
    String postal_index;
    String create_time, getting_time;
    char delivered;
} Mail;

typedef struct post
{
    Mail* mail_arr;
    Address* address_ptr;
    size_t mail_capacity;
} Post;

return_code get_address(Address* address);
return_code post_init(Post* post);
return_code add_mail(Post* post, int* count_mails);
return_code delete_mail(Post* post, int* count_mails);
return_code search_mail(Post* post, int* count_mails);
int compare_mails(const void* a, const void* b);
void print_mail_info(Mail* mail);
return_code get_postal_index(String* str);
return_code set_delivered(Post* post, int* count_mails);
return_code search_delivered(Post* post, int count_mails, int* count_delivered, Mail* *arr_delivered);
void clear_address(Address* address);
void clear_mail(Mail* mail);
return_code search_overdue(Post* post, int count_mails, int* count_overdue, Mail* *arr_overdue);
void clear_post(Post* post, int count_mails);
int compare_dates(const char* time_1, const char* time_2);
void interactive_dialog(Post *post, int *count_mails);

#endif