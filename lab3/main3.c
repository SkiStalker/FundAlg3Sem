#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct 
{
    char* data;
    int len;
    int _size;
}String;


int String_init(String* str, char* data){
    str->len = strlen(data);
    str->_size = strlen(data) + 1;
    str->data = (char*)malloc(sizeof(char) * str->_size);
    if(str->data == NULL){
        str->len = 0;
        str->_size = 0;
        return 0;
    }
    strcpy(str->data, data);
    strcat(str->data, "\0"); 
    return 1;
}



int String_gtr(String* str1, String* str2){
    for (int i = 0; i < str1->len; i++)
    {
        if(str1->data[i] > str2->data[i]){
            return 1;
        }
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
    for (int i = 0; i < str1->len; i++)
    {
        if(str1->data[i] < str2->data[i]){
            return 1;
        }
    }
    return 0;
}

int String_realloc(String* str, int size){ 
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
        return 0;
    }
    strcat(str1->data, str2->data);
    strcat(str1->data, "\0"); 
    return 1;
}

int String_concat_ch(String* str1, char str2){
    int sum_len = strlen(str1->data) + 1;
    int ret = String_realloc(str1, sum_len + 1);
    if(!ret){
        return 0;
    }
    str1->data[str1->len] = str2;
    str1->data[++str1->len] = '\0';
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
    str->len = strlen(data);
    strcpy(str->data, data);
    strcat(str->data, "\0");
    return 1;
}

int String_copy(String* str1, String source){
    if(str1->_size < source._size){
        int ret = String_realloc(str1, source._size);
        if(!ret){
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


typedef struct
{
    unsigned long int id;
    String name;
    String surname;
    long double salary;
}Employee;

enum ret_t{
    MEMORY_ERROR = -2,
    FILE_ERROR = -1
};

void error_handler(int* ret){
    switch (*ret)
    {
    case MEMORY_ERROR:
        printf("Memory error!\n");
        *ret = -1;
        break;
    case FILE_ERROR:
        printf("File error!\n");
        *ret = -1;
        break;
    default:
        *ret = 0;
        break;
    }
}

int Employee_init(Employee* emp, unsigned long int id, char* name, char* surname, long double salary){
    emp->id = id;
    int flag;
    flag = String_init(&emp->name, name);
    if(flag == 0){
        return 0;
    }
    flag = String_init(&emp->surname, surname);
    if(flag == 0){
        String_delete(&emp->name);
        return 0;
    }
    if(salary < 0){
        salary = 0;
    }
    emp->salary = salary;
    return 1;
}

int Employee_delete(Employee* emp){
    if(emp == NULL || emp->salary < 0){
        return 0;
    }
    if(emp->name.data != NULL){
        String_delete(&emp->name);
    }
    if(emp->surname.data != NULL){
        String_delete(&emp->surname);
    }
    emp->salary = -1;
    return 1;
}

int Employee_array_realloc(Employee** emp, int* _size){
    Employee* tmp;
    if(emp == NULL){
        *_size = 4;
        tmp = (Employee*)malloc(sizeof(Employee) * (*_size));
        if(tmp != NULL){
            emp = &tmp;
            return 1;
        }
        return MEMORY_ERROR;
    }
    *_size *= 2;
    tmp = (Employee*)realloc(*emp, sizeof(Employee) * (*_size));
    if(tmp == NULL){
        for (int i = 0; i < *_size / 2; i++)
        {
            Employee_delete(emp[i]);
        }
        return MEMORY_ERROR;
    }
    *emp = tmp;
    return 1;
}

int Parse_File(FILE* f, Employee** empl_empty, int* lenn, int* siz){
    Employee* emp = *empl_empty;
    int _size = 4, len = 0;
    emp = (Employee*)malloc(sizeof(Employee) * _size);
    if(emp == NULL){
        return MEMORY_ERROR;
    }
    
    
    //char buf[BUFSIZ];
    String s;
    String_init(&s, "");
    char c;
    do
    {
        c = fgetc(f);
        while(c != '\n' && c != EOF){
            String_concat_ch(&s, c);
            c = fgetc(f);
        }

        unsigned long int id;
        char name[128], surname[128];
        long double salary;
        sscanf(s.data, "%lu %s %s %Lf", &id, name, surname, &salary);
        Employee tmp;
        int ret = Employee_init(&tmp, id, name, surname, salary);
        if(ret != 1){
            for (int i = 0; i < len; i++)
            {
                Employee_delete(&emp[i]);
            }
            free(emp);
            return 0;
        }
        if(len == _size){
            int ret = Employee_array_realloc(&emp, &_size);
            if(ret != 1){
                return 0;
            }
        }
        emp[len++] = tmp;
        String_assignment(&s, "");
    } while (c != EOF);
    
    *siz = _size;
    *lenn = len;
    *empl_empty = emp;
    return 1;
}

int print_to_file(char* ofile, Employee* emp, int len){
    FILE* f = fopen(ofile, "w");
    if(!f){
        return FILE_ERROR;
    }
    /* for (int i = 0; i < len; i++)
    {
        printf("%s\n", emp[i].name.data);
    }
    


    fclose(f);
    return 1; */



    for (int i = 0; i < len; i++)
    {
        fprintf(f, "%lu %s %s %Lf\n", emp[i].id, emp[i].name.data, emp[i].surname.data, emp[i].salary);
    }
    fclose(f);
    return 1;
}

int compare_lt(const void* p1, const void* p2){
    Employee l = *(const Employee*)p1;
    Employee r = *(const Employee*)p2;

    if(l.salary > r.salary){
        return -1;
    }
    else if(l.salary < r.salary){
        return 1;
    }
    else if(String_gtr(&l.surname, &r.surname)){
        return -1;
    }
    else if(String_ltr(&l.surname, &r.surname)){
        return 1;
    }
    else if(String_gtr(&l.name, &r.name)){
        return -1;
    }
    else if(String_ltr(&l.name, &r.name)){
        return 1;
    }
    else if(l.id > r.id){
        return -1;
    }
    else if(l.id < r.id){
        return 1;
    }
    else{
        return 0;
    }
}

int compare_gr(const void* p1, const void* p2){
    Employee l = *(const Employee*)p1;
    Employee r = *(const Employee*)p2;

    if(l.salary > r.salary){
        return 1;
    }
    else if(l.salary < r.salary){
        return -1;
    }
    else if(String_gtr(&l.surname, &r.surname)){
        return 1;
    }
    else if(String_ltr(&l.surname, &r.surname)){
        return -1;
    }
    else if(String_gtr(&l.name, &r.name)){
        return 1;
    }
    else if(String_ltr(&l.name, &r.name)){
        return -1;
    }
    else if(l.id > r.id){
        return 1;
    }
    else if(l.id < r.id){
        return -1;
    }
    else{
        return 0;
    }
}

int main(int argc, char* argv[]){
    if(argc != 4){
        printf("Wrong amount of arguments!\n");
        return -1;
    }
    else if(strlen(argv[2]) != 2 || (argv[2][0] != '-' && argv[2][0] != '/')){
        printf("Unknown key %s!\n", argv[2]);
        return -1;
    }
    Employee* emp;
    int size = 0, len = 0;
    FILE* f = fopen(argv[1], "r");
    if(f == NULL){
        printf("File error!\n");
        return -1;
    }
    int ret = Parse_File(f, &emp, &len, &size);
    if(f != NULL && ret){
        fclose(f);
    }
    if(ret == 1){
        switch (argv[2][1])
        {
        case 'd':
            qsort(emp, len, sizeof(Employee), compare_lt);
            ret = print_to_file(argv[3], emp, len);
            break;
        case 'a':
            qsort(emp, len, sizeof(Employee), compare_gr);
            ret = print_to_file(argv[3], emp, len);
            break;
        default:
            printf("Unknown key %s!", argv[2]);
            break;
        }
    }
    else{
        error_handler(&ret);
        return ret;
    }
    for (int i = 0; i < len; i++)
    {
        Employee_delete(&emp[i]);
    }
    free(emp);
    error_handler(&ret);
    return ret;
}