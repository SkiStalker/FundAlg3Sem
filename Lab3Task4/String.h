#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
    char* data;
    int len;
    int _size;
}String;

/* int init(String* str){
    str->len = 0;
    str->_size = 8;
    str->data = (char*)malloc(sizeof(char) * str->_size);
    if(str->data == NULL){
        str->len = 0;
        str->_size = 0;
        return 0;
    }
    return 1;
} */

int String_init(String* str, char* data/* , int len */){
    str->len = strlen(data);//len;
    str->_size = strlen(data)/* len */ + 1;
    str->data = (char*)malloc(sizeof(char) * str->_size);
    if(str->data == NULL){
        str->len = 0;
        str->_size = 0;
        return 0;
    }
    strcpy(str->data, data);
    strcat(str->data, "\0"); //из-за этого могут быть ошибки
    return 1;
}



int String_gtr(String* str1, String* str2){
    if(str1->len > str2->len){
        return 1;
    }
    else if(str1->len == str2->len){
        for (int i = 0; i < str1->len; i++)
        {
            if(str1->data[i] > str2->data[i]){
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

int String_isEqual(String* str1, String* str2){
    if(strcmp(str1->data, str2->data)){
        return 0;
    }
    return 1;
}

int String_ltr(String* str1, String* str2){
    if(str1->len < str2->len){
        return 1;
    }
    else if(str1->len == str2->len){
        for (int i = 0; i < str1->len; i++)
        {
            if(str1->data[i] < str2->data[i]){
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

int String_realloc(String* str, int size){ //мб надо двойной указатель
    if(str->data == NULL){
        str->data = (char*)malloc(sizeof(char) * (size + 1));
        if(str->data == NULL){
            str->len = 0;
            str->_size = 0;
            return 0;
        }
        str->_size = size + 1;
        return 1;
    }
    char* tmp;
    tmp = (char*)realloc(str->data, size + 1);
    if(tmp == NULL){
        str->len = 0;
        str->_size = 0;
        return 0;
    }
    str->data = tmp;
    str->_size = size;
    return 1;
}



int String_concat(String* str1, String* str2){
    int sum_len = strlen(str1->data) + strlen(str2->data);
    int ret = String_realloc(str1, sum_len + 1);
    if(!ret){
        //мб надо поменять исходные данные
        return 0;
    }
    strcat(str1->data, str2->data);
    strcat(str1->data, "\0"); //из-за этого могут быть ошибки
    return 1;
}

int String_assignment(String* str, char* data){
    if(str->data == NULL){
        str->_size = strlen(data) + 1;
        str->len = str->_size - 1;
        str->data = (char*)malloc(sizeof(char) * str->_size);
        if(str->data == NULL){
            str->len = 0;
            str->_size = 0;
            return 0;
        }
    }
    int ret = String_realloc(str, strlen(data));
    if(!ret){
        str->len = 0;
        str->_size = 0;
        return 0;
    }
    strcpy(str->data, data);
    strcat(str->data, "\0");
    return 1;
}

int String_copy(String* str1, String source){
    if(str1->_size < source._size){
        int ret = String_realloc(str1, source._size);
        if(!ret){
            //мб надо изменить поля исходных данных
            return 0;
        }
    }
    strcpy(str1->data, source.data);
    str1->len = source.len;
    str1->_size = source._size;
    return 1;
}



int String_delete(String* str){
    if(str == NULL || str->data == NULL){
        return 0;
    }
    str->len = 0;
    str->_size = 0;
    free(str->data);
    return 1;
}