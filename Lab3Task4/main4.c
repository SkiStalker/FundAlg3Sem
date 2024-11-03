#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "String.h"



enum ret{
    MEMORY_ERROR = 2
};

typedef struct 
{
    String city;
    String street;
    int home_number;
    String building;
    int flat_number;
    String index;
    
}Address;



int Address_init(Address* address, char* city, char* street, int home_number, char* building, int flat_number, char* index){
    int res[4];
    String* ptrs[4];
    res[0] = String_init(&address->city, city);
    ptrs[0] = &address->city;
    res[1] = String_init(&address->street, street);
    ptrs[1] = &address->street;
    res[2] = String_init(&address->building, building);
    ptrs[2] = &address->building;
    res[3] = String_init(&address->index, index);
    ptrs[3] = &address->index;
    if(!(res[0] * res[1] * res[2] * res[3])){
        for (int i = 0; i < 3; i++)
        {
            if(res[i]){
                String_delete(ptrs[i]);
            }
        }
        return 0;
    }

    address->home_number = home_number;
    address->flat_number = flat_number;
    return 1;
}

int Address_copy(Address* address, Address* data){
    String_copy(&address->building, data->building);
    String_copy(&address->city, data->city);
    String_copy(&address->index, data->index);
    String_copy(&address->street, data->street);
    address->flat_number = data->flat_number;
    address->home_number = data->home_number;
}

int Address_copy_from_NULL(Address* address, Address* data){
    String_init(&address->building, " ");
    String_copy(&address->building, data->building);
    String_init(&address->city, " ");
    String_copy(&address->city, data->city);
    String_init(&address->index, " ");
    String_copy(&address->index, data->index);
    String_init(&address->street, " ");
    String_copy(&address->street, data->street);
    address->flat_number = data->flat_number;
    address->home_number = data->home_number;
    return 1;
}

int Address_delete(Address* address){
    String_delete(&address->building);
    String_delete(&address->city);
    String_delete(&address->index);
    String_delete(&address->street);
    return 1;
}

/* typedef struct 
{
    short year;
    unsigned char month, day, hour, minute, second;
}Date; */



typedef struct 
{
    Address receiver;
    double weight;
    String mail_id;
    String date_creation;
    String date_get;
}Mail;

int Mail_init(Mail* mail, Address* receiver, char* mail_id, char* date_creation, char* date_get, double weight){
    mail->receiver = *receiver;
    if(weight < 0){
        mail->weight = 0;
    }
    else{
        mail->weight = weight;
    }
    int ret[3];
    if(strlen(date_creation) != 19 && date_creation[2] != ':' && date_creation[5] != ':' && date_creation[13] != ':' && date_creation[16] != ':'){
        ret[0] = String_init(&mail->date_creation, "01:01:1970 00:00:00");    
    }
    else{
        ret[0] = String_init(&mail->date_creation, date_creation);
    }    
    ret[1] = String_init(&mail->mail_id, mail_id);
    if(strlen(date_get) != 19 && date_get[2] != ':' && date_get[5] != ':' && date_get[13] != ':' && date_get[16] != ':'){
        ret[2] = String_init(&mail->date_get, "01:01:1970 00:00:00");    
    }
    else{
        ret[2] = String_init(&mail->date_get, date_get);
    }    
    if(!(ret[0] && ret[1] && ret[2])){
        return 0;
    }
    return 1;
}

int Mail_delete(Mail* mail){
    Address_delete(&mail->receiver);
    String_delete(&mail->date_creation);
    String_delete(&mail->date_get);
    String_delete(&mail->mail_id);
    return 1;
}

int Mail_cpy_from_NULL(Mail* mail, Mail* src){
    Address_copy_from_NULL(&mail->receiver, &src->receiver);
    String_init(&mail->date_creation, " ");
    String_copy(&mail->date_creation, src->date_creation);
    String_init(&mail->date_get, " ");
    String_copy(&mail->date_get, src->date_get);
    String_init(&mail->mail_id, " ");
    String_copy(&mail->mail_id, src->mail_id);
    mail->weight = src->weight;
    return 1;
}


typedef struct 
{
    Mail* mail;
    int _mail_size, _mail_len;
    Address* address;
}Post;

int Post_delete(Post*);

int Post_init(Post* post, Address* address){
    post->_mail_size = 4;
    post->_mail_len = 0;
    post->mail = (Mail*)malloc(sizeof(Mail) * (post->_mail_size));
    if(post->mail == NULL){
        post->_mail_size = 0;
        return 0;
    }
    post->address = address;
    return 1;
}

int Post_realloc(Post* post){
    Mail* new = (Mail*)realloc(post->mail, post->_mail_size * 2);
    if(new == NULL){
        Post_delete(post);
        return 0;
    }
    post->mail = new;
    post->_mail_size *= 2;
    return 1;
}

int Post_add(Post* post, Mail* mail){
    Mail tmp;
    for (int i = 0; i < post->_mail_len; i++)
    {
        tmp = post->mail[i];
        if(String_ltr(&mail->receiver.index, &tmp.receiver.index)){
            post->mail[i] = *mail;
            Mail next;
            int ret;
            if(post->_mail_len + 1 > post->_mail_size){
                ret = Post_realloc(post);
                if(!ret){
                    Post_delete(post);
                    return MEMORY_ERROR;
                }
            }
            for (int j = i+1; j < post->_mail_len + 1; j++)
            {
                next = post->mail[j];
                post->mail[j] = tmp;
                tmp = next;
            }
            post->_mail_len++;
            return 1;
        }
        else if(String_isEqual(&mail->receiver.index, &tmp.receiver.index)){
            if(String_ltr(&mail->mail_id, &tmp.mail_id) || String_isEqual(&mail->mail_id, &tmp.mail_id)){
                post->mail[i] = *mail;
                Mail next;
                int ret;
                if(post->_mail_len + 1 > post->_mail_size){
                    ret = Post_realloc(post);
                    if(!ret){
                        Post_delete(post);
                        return MEMORY_ERROR;
                    }
                }
                for (int j = i+1; j < post->_mail_len + 1; j++)
                {
                    next = post->mail[j];
                    post->mail[j] = tmp;
                    tmp = next;
                }
                post->_mail_len++;
                return 1;
            }
        }
        //if()здесь добавление с сортировкой
    }
    if(post->_mail_len + 1 > post->_mail_size){
        int ret = Post_realloc(post);
        if(!ret){
            Post_delete(post);
            return MEMORY_ERROR;
        }
    }
    post->mail[post->_mail_len++] = *mail;
    return 1;
}

int Post_delete(Post* post){
    
    if(post->mail == NULL){
        Address_delete(post->address);
        post->_mail_size = 0;
        return 0;
    }
    for (int i = 0; i < post->_mail_len; i++)
    {
        Mail_delete(&post->mail[i]);
    }
    
    free(post->mail);
    post->_mail_size = 0;

    Address_delete(post->address);
    return 1;
}


int has_only_numbers(char* str, int len){
    for (int i = 0; i < len; i++)
    {
        if(!isdigit(str[i]) && str[i] != '.'){
            return 0;
        }
    }
    return 1;
}

int Mail_cmp_byDateGet(Mail* mail1, Mail* mail2){
    if(mail1 == NULL || mail2 == NULL){
        return MEMORY_ERROR;
    }
    else if(mail1->date_get.data == NULL || mail2->date_get.data == NULL){
        return MEMORY_ERROR;
    }
    else if(mail1->date_get.len != 19 || mail2->date_get.len != 19){
        return -1; //Другую ошибку
    }
    else if(mail1 == mail2){
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        if(mail1->date_get.data[6 + i] < mail2->date_get.data[6 + i]){
            return 1;
        }
        else if(mail1->date_get.data[6 + i] > mail2->date_get.data[6 + i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_get.data[3 + i] < mail2->date_get.data[3 + i]){
            return 1;
        }
        else if(mail1->date_get.data[3 + i] > mail2->date_get.data[3 + i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_get.data[i] < mail2->date_get.data[i]){
            return 1;
        }
        else if(mail1->date_get.data[i] > mail2->date_get.data[i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_get.data[i + 11] < mail2->date_get.data[i + 11]){
            return 1;
        }
        else if(mail1->date_get.data[i + 11] > mail2->date_get.data[i + 11]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_get.data[i + 14] < mail2->date_get.data[i + 14]){
            return 1;
        }
        else if(mail1->date_get.data[i + 14] > mail2->date_get.data[i + 14]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_get.data[i + 17] < mail2->date_get.data[i + 17]){
            return 1;
        }
        else if(mail1->date_get.data[i + 17] > mail2->date_get.data[i + 17]){
            return 2;
        }
    }
    return 0;
}


int Mail_cmp_byDateCreation(Mail* mail1, Mail* mail2){
    if(mail1 == NULL || mail2 == NULL){
        return MEMORY_ERROR;
    }
    else if(mail1->date_creation.data == NULL || mail2->date_creation.data == NULL){
        return MEMORY_ERROR;
    }
    else if(mail1->date_creation.len != 19 || mail2->date_creation.len != 19){
        return -1; //Другую ошибку
    }
    else if(mail1 == mail2){
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        if(mail1->date_creation.data[6 + i] < mail2->date_creation.data[6 + i]){
            return 1;
        }
        else if(mail1->date_creation.data[6 + i] > mail2->date_creation.data[6 + i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_creation.data[3 + i] < mail2->date_creation.data[3 + i]){
            return 1;
        }
        else if(mail1->date_creation.data[3 + i] > mail2->date_creation.data[3 + i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_creation.data[i] < mail2->date_creation.data[i]){
            return 1;
        }
        else if(mail1->date_creation.data[i] > mail2->date_creation.data[i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_creation.data[i + 11] < mail2->date_creation.data[i + 11]){
            return 1;
        }
        else if(mail1->date_creation.data[i + 11] > mail2->date_creation.data[i + 11]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_creation.data[i + 14] < mail2->date_creation.data[i + 14]){
            return 1;
        }
        else if(mail1->date_creation.data[i + 14] > mail2->date_creation.data[i + 14]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(mail1->date_creation.data[i + 17] < mail2->date_creation.data[i + 17]){
            return 1;
        }
        else if(mail1->date_creation.data[i + 17] > mail2->date_creation.data[i + 17]){
            return 2;
        }
    }
    return 0;
}

int Print_Sorted_byDateGet(Post* post){
    if(post == NULL || post->mail == NULL){
        return MEMORY_ERROR;
    }
    int* flags = (int*)calloc(post->_mail_len, sizeof(int));
    if(flags == NULL){
        return MEMORY_ERROR;
    }
    int min_notmin = 0;
    printf("Index          Mail_id\tDataCreation\t\t\tDataGet\n");
    for (int i = 0; i < post->_mail_len; i++)
    {
        while(flags[min_notmin] != 0)
        {
            min_notmin++;
        }
        /* Mail minimal = post->mail[min_notmin]; */
        int min_by_j = min_notmin;
        for (int j = 0; j < post->_mail_len; j++)
        {
            //printf("%d\n", flags[min_by_j]);
            if(flags[j] == 0 && Mail_cmp_byDateGet(&post->mail[min_by_j], &post->mail[j]) == 2){
                min_by_j = j;
            }
        }
        flags[min_by_j] = 1;
        printf("%s\t%s\t%s\t%s\n", post->mail[min_by_j].receiver.index.data, post->mail[min_by_j].mail_id.data, post->mail[min_by_j].date_creation.data, post->mail[min_by_j].date_get.data);
    }
    free(flags);
    return 0;
}

int Print_Sorted_byDateCreation(Post* post){
    if(post == NULL || post->mail == NULL){
        return MEMORY_ERROR;
    }
    int* flags = (int*)calloc(post->_mail_len, sizeof(int));
    if(flags == NULL){
        return MEMORY_ERROR;
    }
    int min_notmin = 0;
    printf("Index          Mail_id\tDataCreation\t\t\tDataGet\n");
    for (int i = 0; i < post->_mail_len; i++)
    {
        while(flags[min_notmin] != 0)
        {
            min_notmin++;
        }
        /* Mail minimal = post->mail[min_notmin]; */
        int min_by_j = min_notmin;
        for (int j = 0; j < post->_mail_len; j++)
        {
            //printf("%d\n", flags[min_by_j]);
            if(flags[j] == 0 && Mail_cmp_byDateCreation(&post->mail[min_by_j], &post->mail[j]) == 2){
                min_by_j = j;
            }
        }
        flags[min_by_j] = 1;
        printf("%s\t%s\t%s\t%s\n", post->mail[min_by_j].receiver.index.data, post->mail[min_by_j].mail_id.data, post->mail[min_by_j].date_creation.data, post->mail[min_by_j].date_get.data);
    }
    free(flags);
    return 0;
}

int isDate(char* date, int len){
    if(len != 19 && date[2] != ':' && date[5] != ':' && date[13] != ':' && date[16] != ':'){
        return 0;
    }
    for (int i = 0; i < len; i++)
    {
        if(!isdigit(date[i]) && date[i] != ':' && date[i] != ' '){
            return 0;
        }
    }
    int day, month, year, hour, minute, second;
    sscanf(date, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second);
    if(day > 31){
        return 0;
    }
    else if(month > 12){
        return 0;
    }
    else if(hour > 24){
        return 0;
    }
    else if(minute > 59){
        return 0;
    }
    else if(second > 59){
        return 0;
    }
    return 1;
}


int Print_delivered(Post* post){
    time_t tm = time(NULL);
    struct tm *now = localtime(&tm);

    Post post_new;
    Address address_new;
    Address_init(&address_new, "No", "No", 1, "No", 123, "No");
    Post_init(&post_new, &address_new);


    now->tm_mon += 1;
    now->tm_year += 1900;
    printf("Cur date: %d:%d:%d %d:%d:%d\n", now->tm_mday, now->tm_mon, now->tm_year, now->tm_hour, now->tm_min, now->tm_sec);
    for (int i = 0; i < post->_mail_len; i++)
    {
        Mail tmp;
        int year, month, day, hour, minute, second;
        sscanf(post->mail[i].date_get.data, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second);
        
        if(now->tm_year > year){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //Post_add(&post_new, &post->mail[i]);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
        }
        else if(now->tm_year < year){
            continue;
        }
        else if(now->tm_year == year && now->tm_mon > month){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_mon < month){
            continue;
        }
        else if(now->tm_mon == month && now->tm_mday > day){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_mday < day){
            continue;
        }
        else if(now->tm_mday == day && now->tm_hour > hour){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_hour < hour){
            continue;
        }
        else if(now->tm_hour == hour && now->tm_min > minute){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_min < minute){
            continue;
        }
        else if(now->tm_min == minute && now->tm_sec > second){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_sec < second){
            continue;
        }
    
    }
    int ret = Print_Sorted_byDateGet(&post_new);
    Post_delete(&post_new);
    return ret;
}

int Print_undelivered(Post* post){
    time_t tm = time(NULL);
    struct tm *now = localtime(&tm);

    Post post_new;
    Address address_new;
    Address_init(&address_new, "No", "No", 1, "No", 123, "No");
    Post_init(&post_new, &address_new);

    now->tm_mon += 1;
    now->tm_year += 1900;
    printf("Cur date: %d:%d:%d %d:%d:%d\n", now->tm_mday, now->tm_mon, now->tm_year, now->tm_hour, now->tm_min, now->tm_sec);
    for (int i = 0; i < post->_mail_len; i++)
    {
        Mail tmp;
        int year, month, day, hour, minute, second;
        sscanf(post->mail[i].date_get.data, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second);
        if(now->tm_year < year){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //Post_add(&post_new, &post->mail[i]);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
        }
        else if(now->tm_year > year){
            continue;
        }
        else if(now->tm_year == year && now->tm_mon < month){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_mon > month){
            continue;
        }
        else if(now->tm_mon == month && now->tm_mday < day){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_mday > day){
            continue;
        }
        else if(now->tm_mday == day && now->tm_hour < hour){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_hour > hour){
            continue;
        }
        else if(now->tm_hour == hour && now->tm_min < minute){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_min > minute){
            continue;
        }
        else if(now->tm_min == minute && now->tm_sec < second){
            Mail_cpy_from_NULL(&tmp, &post->mail[i]);
            Post_add(&post_new, &tmp);
            //printf("%s\t%s\n", post->mail[i].mail_id.data, post->mail[i].date_get.data);
            //Post_add(&post_new, &post->mail[i]);
        }
        else if(now->tm_sec > second){
            continue;
        }
    }
    int ret = Print_Sorted_byDateGet(&post_new);
    Post_delete(&post_new);
    return ret;
}

int date_cmp(char* d1, int len_d1, char* d2, int len_d2){
    if(len_d1 != 19 || len_d2 != 19){
        return -1;
    }
    for (int i = 0; i < 4; i++)
    {
        if(d1[6 + i] < d2[6 + i]){
            return 1;
        }
        else if(d1[6 + i] > d2[6 + i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(d1[3 + i] < d2[3 + i]){
            return 1;
        }
        else if(d1[3 + i] > d2[3 + i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(d1[i] < d2[i]){
            return 1;
        }
        else if(d1[i] > d2[i]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(d1[i + 11] < d2[i + 11]){
            return 1;
        }
        else if(d1[i + 11] > d2[i + 11]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(d1[i + 14] < d2[i + 14]){
            return 1;
        }
        else if(d1[i + 14] > d2[i + 14]){
            return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(d1[i + 17] < d2[i + 17]){
            return 1;
        }
        else if(d1[i + 17] > d2[i + 17]){
            return 2;
        }
    }
    return 0;
}

int print_info_about_mail(Post* post, char* index){
    for (int i = 0; i < post->_mail_len; i++)
    {
        if(!strcmp(index, post->mail[i].receiver.index.data)){
            printf("Index: %s\nCreation date: %s\nGet date: %s\nWeight: %f\n", post->mail[i].receiver.index.data, post->mail[i].date_creation.data, post->mail[i].date_get.data, post->mail[i].weight);
            return 1;
        }
    }
    return 0;
}

int main(){
    //For dialog
    Post post;
    Address post_address;
    Address_init(&post_address, "SPB", "HZ", 1, "1A", 123, "index");
    Post_init(&post, &post_address);

    printf("Menu of the dialog:\n0 - add mail\n1 - print list sorted by creation date\n2 - print list sorted by get date\n3 - print list\n4 - Print all delivere\n5 - Print all undelivered\n6 - to search for mail by index\nH - help\nS - to stop input\n");
    char mode = ' ';
    while(mode != 'S'){
        scanf("%c", &mode);
        switch (mode)
        {
        case '0':
            {
                Mail mail;
                Address address;
                printf("Write the address in next order (without staples): (City) (Street) (House number) (Building number) (flat number) (index)\n");
                int flag1 = 1;
                char City[1024], Street[1024], House_num[1024], Building_num[1024], flat_num[1024], index[1024];
                while(flag1){
                    
                    scanf("%s %s %s %s %s %s", City, Street, House_num, Building_num, flat_num, index);
                    if((has_only_numbers(House_num, strlen(House_num)) && has_only_numbers(flat_num, strlen(flat_num)))){ //не забыть проверить индекс
                        flag1 = 0;
                    }
                    else if(strlen(index) != 14 && !has_only_numbers(index, strlen(index))){
                        printf("Wrong index. The length must be equal to 14 and index must consist of numbers!\n Try again...\n");
                        continue;
                    }
                    else{
                        printf("Wrong data type. House number and flat number must be integers! Try again...\n");
                    }
                }
                int housenum_int = atoi(House_num), flatnum_int = atoi(flat_num);
                Address_init(&address, City, Street, housenum_int, Building_num, flatnum_int, index);
                printf("Write the address in next order (without staples): (mail id) (date creation) (date get) (weight)\n");
                flag1 = 1;
                char mail_id[1024], date_cr[1024], date_get[1024], weight[1024];
                while(flag1){
                    scanf("%s %s %s %s %s %s", mail_id, date_cr, date_cr + 11, date_get, date_get + 11, weight);
                    date_cr[10] = ' ', date_get[10] = ' ';
                    if(!has_only_numbers(weight, strlen(weight)) || !isDate(date_cr, strlen(date_cr) 
                    || !isDate(date_get, strlen(date_get)))){
                        printf("Wrong data type. Weight must be a number. Date must be in format 'dd:MM:yyyy hh:mm:ss'. Try again...\n");
                    }
                    else if(date_cmp(date_cr, strlen(date_cr), date_get, strlen(date_get)) == 2){
                        printf("Get date must not be earlier than creation date! Try again...\n");
                    }
                    else{
                        flag1--;
                    }
                }
                double weight_d = atof(weight);
                Mail_init(&mail, &address, mail_id, date_cr, date_get, weight_d);
                Post_add(&post, &mail);
            }
            printf("Mail added\n");
            break;
        case '1':
            printf("Printing sorted by creation date...\n");
            {
                int ret = Print_Sorted_byDateCreation(&post);
                if(ret == MEMORY_ERROR){
                    printf("Memory error!\n");
                    Post_delete(&post);
                    return -1;
                }
            }
            break;
        case '2':
            printf("Printing sorted by get date...\n");
            {
                int ret = Print_Sorted_byDateGet(&post);
                if(ret == MEMORY_ERROR){
                    printf("Memory error!\n");
                    Post_delete(&post);
                    return -1;
                }
            }
            break;
        case '3':
            {
                printf("Printing list...\n");
                for (int i = 0; i < post._mail_len; i++)
                {
                    printf("%s\t%s\t%s\t%s\n", post.mail[i].receiver.index.data, post.mail[i].mail_id.data, post.mail[i].date_creation.data, post.mail[i].date_get.data);
                }
            }
            break;
        case '4':
            {
                int ret = Print_delivered(&post);
                if(ret == MEMORY_ERROR){
                    printf("Memory error!\n");
                    Post_delete(&post);
                    return -1;
                }
            }
            break;
        case '5':
            {
                int ret = Print_undelivered(&post);
                if(ret == MEMORY_ERROR){
                    printf("Memory error!\n");
                    Post_delete(&post);
                    return -1;
                }
            }
            break;
        case '6':
            {
                char index[100];
                printf("Write index...");
                int flag = 1;
                while(flag){
                    scanf("%s", index);
                    if(strlen(index) != 14 && !has_only_numbers(index, strlen(index))){
                        printf("Wrong index. The length must be equal to 14 and index must consist of numbers!\n Try again...\n");
                        continue;
                    }
                    flag = 0;
                    int ret = print_info_about_mail(&post, index);
                    if(ret == 0){
                        printf("No such mail.\n");
                    }
                }
                
            }
            break;
        case 'H':
        {
            printf("Menu of the dialog:\n0 - add mail\n1 - print list sorted by creation date\n2 - print list sorted by get date\n3 - print list\n4 - Print all delivere\n5 - Print all undelivered\n6 - to search for mail by index\nH - help\nS - to stop input\n");
        }
        default:
            break;
        }
    }
    Post_delete(&post);
    return 0;
}