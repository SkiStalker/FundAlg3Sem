#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
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
         if(str1->data[i] == '\0' && str2->data[i] != '\0'){
            return 0;
        }
        if(str2->data[i] == '\0' && str1->data[i] != '\0'){
            return 1;
        }
        if(toupper(str1->data[i]) > toupper(str2->data[i])){
            return 1;
        }
        else if(toupper(str1->data[i]) < toupper(str2->data[i])){
            return 0;
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
        if(str1->data[i] == '\0' && str2->data[i] != '\0'){
            return 1;
        }
        if(str2->data[i] == '\0' && str1->data[i] != '\0'){
            return 0;
        }
        if(toupper(str1->data[i]) < toupper(str2->data[i])){
            return 1;
        }
        else if(toupper(str1->data[i]) > toupper(str2->data[i])){
            return 0;
        }
    }
    return 0;
}

int String_realloc(String* str, int size){
    if(str->_size == 0){
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
    int ret = String_realloc(str1, source._size);
    if(!ret){
        return 0;
    }
    for (int i = 0; i < source.len; i++)
    {
        str1->data[i] = source.data[i];
    }
    str1->data[source.len] = '\0';
    str1->len = source.len;
    str1->_size = source._size;
    return 1;
}



int String_delete(String* str){
    if(str == NULL || str->data == NULL || str->_size == 0){
        return 0;
    }
    str->len = 0;
    str->_size = 0;
    free(str->data);
    return 1;
}

typedef struct 
{
    char date[11];
    int year;
    char month;
    char day;
}Date;

int has_only_date_char(char* str, int len){
    for (int i = 0; i < len; i++)
    {
        if(!isdigit(str[i]) && str[i] != '.' && str[i] != EOF){
            return 0;
        }
    }
    return 1;
}

int Date_init(Date* date, char* dt){
    if(strlen(dt) != 10){
        strcpy(date->date, "01.01.1970\0");
        date->year = 1970;
        date->month = 1;
        date->day = 1;
        return 0;
    }
    if(has_only_date_char(dt, 10)){
        strcpy(date->date, dt);
        sscanf(dt, "%hhd.%hhd.%d", &date->day, &date->month, &date->year);
    }
    else{
        strcpy(date->date, "01.01.1970\0");
        date->year = 1970;
        date->month = 1;
        date->day = 1;
        return 0;
    }
    return 1;    
}

typedef struct 
{
    String name;
    String surname;
    String fathername;
    Date birthDate;
    char gender;
    long double income;
}Cityzen;

typedef struct Node
{
    Cityzen data;
    struct Node* next;
}Node;

typedef struct
{
    Node* head;
    int len;
}Liver;

typedef int (*Operation)(Liver*, Cityzen*, ...);

typedef struct
{
    Operation* func;
    Cityzen* livers;
    int used;
    int _size;
    int len;
}VectorCO;

int VectorCO_init(VectorCO* vector){
    vector->len = 0;
    vector->_size = 4;
    vector->used = 0;
    vector->func = (Operation*)malloc(sizeof(Operation) * vector->_size);
    if(vector->func == NULL){
        vector->_size = 0;
        return 0;
    }
    vector->livers = (Cityzen*)malloc(sizeof(Cityzen) * vector->_size);
    if(vector->func == NULL){
        free(vector->func);
        vector->_size = 0;
        return 0;
    }
    return 1;
}

int Cityzen_delete(Cityzen* liver){
    if(liver->gender != '0'){
        int ret = 1;
        ret *= String_delete(&liver->name);
        ret *= String_delete(&liver->surname);
        ret *= String_delete(&liver->fathername);
        liver->income = 0;
        liver->gender = '0';
        return ret;
    }
    return 0;
}


int VectorCO_realloc(VectorCO* vector){
    vector->_size *= 2;
    Operation* op = (Operation*)realloc(vector->func, sizeof(Operation) * vector->_size);
    if(op == NULL){
        for (int i = 0; i < vector->len; i++)
        {
            Cityzen_delete(&vector->livers[i]);
        }
        free(vector->livers);
        free(vector->func);
        vector->_size =0 ;
        vector->len =0 ;
        return 0;
    }
    Cityzen* cit = (Cityzen*)realloc(vector->livers, sizeof(Cityzen) * vector->_size);
    if(cit == NULL){
        vector->_size =0 ;
        vector->len =0 ;
        for (int i = 0; i < vector->len; i++)
        {
            Cityzen_delete(&vector->livers[i]);
        }
        free(vector->func);
        free(vector->livers);
        return 0;
    }
    vector->func = op;
    vector->livers = cit;
    return 1;
}

int VectorCO_append(VectorCO* vector, Operation* op, Cityzen* cit){
    if(vector->len == vector->_size){
        int ret = VectorCO_realloc(vector);
        if(!ret){
            return 0;
        }
    }
    vector->func[vector->len] = *op;
    vector->livers[vector->len++] = *cit;
    vector->used++;
    return 1;
}

int VectorCO_delete(VectorCO* VectorCO){
    if(VectorCO == NULL){
        return 0;
    }
    for (int i = 0; i < VectorCO->_size; i++)
    {
        Cityzen_delete(&VectorCO->livers[i]);
    }
    free(VectorCO->livers);
    free(VectorCO->func);
    VectorCO->_size = 0;
    VectorCO->len = 0;
    return 1;
}


int Cityzen_init(Cityzen* liver, char* name, char* surname, char* fathername, char* birthdate, char gender, long double income){
    if(strlen(name) == 0){
        return 0;
    }
    String_init(&liver->name, "");
    int flag = 0;
    for (int i = 0; i < (int)strlen(name); i++)
    {
        if(isalpha(name[i])){
            String_concat_ch(&liver->name, name[i]);
            flag = 1;
        }
    }
    if(strlen(surname) == 0){
        String_delete(&liver->name);
        return 0;
    }
    String_init(&liver->surname, "");
    for (int i = 0; i < (int)strlen(surname); i++)
    {
        if(isalpha(surname[i])){
            String_concat_ch(&liver->surname, surname[i]);
            flag = 1;
        }
    }
    if(strlen(fathername) == 0 || !flag){
        String_delete(&liver->name);
        String_delete(&liver->surname);
        return 0;
    }
    String_init(&liver->fathername, "");
    for (int i = 0; i < (int)strlen(fathername); i++)
    {
        if(isalpha(fathername[i]) || fathername[i] == '-'){
            String_concat_ch(&liver->fathername, fathername[i]);
        }
    }
    Date dateD;
    int ret = Date_init(&dateD, birthdate);
    liver->birthDate = dateD;
    if(gender != 'W' && gender != 'M'){
        gender = 'N';
        ret = 0;
    }
    liver->gender = gender;
    if(income < 0){
        liver->income = 0;
        ret = 0;
    }
    else{
        liver->income = income;
    }
    return ret;
}

int Cityzen_isEqual(Cityzen* s1, Cityzen* s2){
    if(String_isEqual(&s1->fathername, &s2->fathername) && String_isEqual(&s1->name, &s2->name) 
    && String_isEqual(&s1->surname, &s2->surname) && s1->gender == s2->gender && !strcmp(s2->birthDate.date, s1->birthDate.date)){
        return 1;
    }
    return 0;
}

int Cityzen_cpy(Cityzen* to, Cityzen* from){
    strcpy(to->birthDate.date, from->birthDate.date);
    to->birthDate.day = from->birthDate.day;
    to->birthDate.month = from->birthDate.month;
    to->birthDate.year = from->birthDate.year;
    String_copy(&to->fathername, from->fathername);
    String_copy(&to->name, from->name);
    String_copy(&to->surname, from->surname);
    to->gender = from->gender;
    to->income = from->income;
    return 1;
}





int Node_init(Node** node){
    *node = (Node*)malloc(sizeof(Node));
    if(*node == NULL){
        return 0;
    }
    (*node)->next = NULL;
    return 1;
}

int Node_delete(Node* node){
    if(node != NULL){
        if(node->data.name._size != 0){
            Cityzen_delete(&node->data);
        }
        free(node);
        return 1;
    }
    return 0;
}

int Liver_init(Liver* list){
    int ret = Node_init(&list->head);
    list->len = 0;
    return ret;
}

int Liver_cut(Liver* list, Cityzen* liver, ...){
    Node* tmp = list->head;
    Node* prev = tmp;
    int cnt = 0;
    while(tmp != NULL && !Cityzen_isEqual(&tmp->data, liver)){
        prev = tmp;
        tmp = tmp->next;
        cnt++;
    }
    if(tmp == NULL){
        return 0;
    }
    if(cnt != 0){
        prev->next = tmp->next; 
        Cityzen_delete(&tmp->data);
        Node_delete(tmp);
    }
    else{
        list->head = list->head->next;
        Cityzen_delete(&tmp->data);
        Node_delete(tmp);
    }
    list->len--;
    return 1;
}

int Liver_insert(Liver* list, Cityzen* liver, ...){
    Node* new_node;
    Node_init(&new_node);
    new_node->data = *liver;
    Node* tmp = list->head;
    if(list->len == 0){
        list->head->data = new_node->data;
        list->len = 1;
        return 1;
    }
    if((tmp->data.birthDate.year - liver->birthDate.year < 0) 
    || ((tmp->data.birthDate.year - liver->birthDate.year == 0) && tmp->data.birthDate.month - liver->birthDate.month < 0)
    || (tmp->data.birthDate.year - liver->birthDate.year == 0 && tmp->data.birthDate.month - liver->birthDate.month == 0 && tmp->data.birthDate.day - liver->birthDate.month < 0) 
    ||(tmp->data.birthDate.year - liver->birthDate.year == 0 && tmp->data.birthDate.month - liver->birthDate.month == 0 && tmp->data.birthDate.day - liver->birthDate.month == 0)){
        new_node->next = tmp;
        list->head = new_node;
        list->len++;
        return 1;
    }
    Node* prev = tmp;
    while (tmp->next != NULL)
    {
        if((tmp->data.birthDate.year - liver->birthDate.year < 0) 
        || ((tmp->data.birthDate.year - liver->birthDate.year == 0) && tmp->data.birthDate.month - liver->birthDate.month < 0)
        || (tmp->data.birthDate.year - liver->birthDate.year == 0 && tmp->data.birthDate.month - liver->birthDate.month == 0 && tmp->data.birthDate.day - liver->birthDate.month < 0) 
        ||(tmp->data.birthDate.year - liver->birthDate.year == 0 && tmp->data.birthDate.month - liver->birthDate.month == 0 && tmp->data.birthDate.day - liver->birthDate.month == 0)){
            new_node->next = tmp;
            prev->next = new_node;
            list->len++;
            return 1;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    tmp->next = new_node;
    list->len++;
    return 1;
}


int Liver_overwrite(Liver* list, Cityzen* liver, ...){
    Node* tmp = list->head;
    va_list args;
    va_start(args, liver);
    Cityzen* overwrite = va_arg(args, Cityzen*);
    va_end(args);
    while(tmp != NULL && !Cityzen_isEqual(&tmp->data, liver)){
        tmp = tmp->next;
    }
    if(tmp == NULL){
        return 0;
    }
    Liver_cut(list, &tmp->data);
    Cityzen_delete(&tmp->data);
    Liver_insert(list, overwrite);
    
    return 1;
}


int Liver_delete(Liver* list){
    while(list->head != NULL){
        Node* tmp = list->head;
        list->head = list->head->next;
        Node_delete(tmp);
    }
    return 1;
}

int Parse_file(Liver* list, char* fileName){
    FILE* f = fopen(fileName, "r");
    if(f == NULL){
        return 2;
    }
    String s1;
    String_init(&s1, "");
    char c;
    do
    {
        int spaces = 0, flag = 0;
        do
        {
            c = getc(f); 
            if(isalnum(c)){
                flag = 1;
            }
            if(c == ' ' && flag){
                spaces++;
                flag = 0;
            }
            String_concat_ch(&s1, c);
        } while (c != '\n' && c != EOF);
        Cityzen ctz;
        char name[128], surname[128], fathername[128], date[128], gender, salary[128];
        if(spaces == 5){
            sscanf(s1.data, "%s %s %s %s %c %s", name, surname, fathername, date, &gender, salary);
            if(!has_only_date_char(salary, strlen(salary))){
                strcpy(salary, "0\0");
            }
            long double incomeDBL = atof(salary);
            Cityzen_init(&ctz, name, surname, fathername, date, gender, incomeDBL);
            Liver_insert(list, &ctz);
            String_assignment(&s1, "");
        }
        else if(spaces == 4){
            sscanf(s1.data, "%s %s %s %c %s", name, surname, date, &gender, salary);
            strcpy(fathername, "-");
            if(!has_only_date_char(salary, strlen(salary))){
                strcpy(salary, "0\0");
            }
            long double incomeDBL = atof(salary);
            Cityzen_init(&ctz, name, surname, fathername, date, gender, incomeDBL);
            Liver_insert(list, &ctz);
            String_assignment(&s1, "");
        }
        
    } while (c != EOF);
    String_delete(&s1);
    fclose(f);
    return 1;
}


int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Wrong amount of arguments!\n");
        return -1;
    }
    Liver livers;
    Liver_init(&livers);
    Parse_file(&livers, argv[1]);
    VectorCO vco;
    VectorCO_init(&vco);
    
    char md = ' ';
    printf("What do you want to do?\n0 - print all data\n1 - add cityzen\n2 - delete cityzen\n3 - overwrite info about the cityzen\nU - undo\nH - help\nS - Stop\n");
    while(md != 'S'){
        scanf(" %c", &md);
        switch (md)
        {
            case '0':
            {
                Node* tmp = livers.head;
                printf("________________________________\n");
                for (int i = 0; i < livers.len; i++)
                {
                    printf("%s %s %s %s %c %Lf\n", tmp->data.name.data, tmp->data.surname.data, tmp->data.fathername.data, tmp->data.birthDate.date, tmp->data.gender, tmp->data.income);
                    tmp = tmp->next;
                }           
                printf("________________________________\n");
            }
            break;
            case '1':
            {
                printf("Write all data about the cityzen you want to add\nRight format:\n<Name> <Surname> <Fathername (if no fathername then '-')> <Birth date (day.month.year)> <Gender (M or W)> <salary>\n");
                char name[128], surname[128], fathername[128], date[128], gender, salary[128];
                Cityzen ctz;
                scanf("%128s %128s %128s %128s %c %15s", name, surname, fathername, date, &gender, salary);
                if(!has_only_date_char(salary, strlen(salary))){
                    strcpy(salary, "0\0");
                }
                long double incomeDBL = atof(salary);
                Cityzen_init(&ctz, name, surname, fathername, date, gender, incomeDBL);
                Liver_insert(&livers, &ctz);
                Cityzen ctz_copy;
                Cityzen_init(&ctz_copy, "a", "a", "a", "01.01.0101", 'M', 0.0);
                Cityzen_cpy(&ctz_copy, &ctz);
                Operation op = Liver_cut;
                VectorCO_append(&vco, &op, &ctz_copy);
                break;
            }
            break;
            case '2':
            {
                printf("Write all data about the cityzen you want to delete\nRight format:\n<Name> <Surname> <Fathername (if no fathername then '-')> <Birth date (day.month.year)> <Gender (M or W)> <salary>\n");
                char name[128], surname[128], fathername[128], date[128], gender, salary[128];
                Cityzen ctz;
                scanf("%128s %128s %128s %128s %c %15s", name, surname, fathername, date, &gender, salary);
                if(!has_only_date_char(salary, strlen(salary))){
                    strcpy(salary, "0\0");
                }
                long double incomeDBL = atof(salary);
                Cityzen ctz_copy;
                Cityzen_init(&ctz, name, surname, fathername, date, gender, incomeDBL);
                Cityzen_init(&ctz_copy, "a", "a", "a", "01.01.0101", 'M', 0.0);
                Cityzen_cpy(&ctz_copy, &ctz);
                
                int ret = Liver_cut(&livers, &ctz);
                if(ret == 0){
                    printf("The cityzen wasn't found\n");
                    Cityzen_delete(&ctz);
                    Cityzen_delete(&ctz_copy);
                    break;
                }
                Cityzen_delete(&ctz);
                
                Operation op = Liver_insert;
                VectorCO_append(&vco, &op, &ctz_copy);
            }
            break;
            case '3':
            {
                printf("Write all data about the cityzen you want to change\nRight format:\n<Name> <Surname> <Fathername (if no fathername then '-')> <Birth date (day.month.year)> <Gender (M or W)> <salary>\n");
                char name[128], surname[128], fathername[128], date[128], gender, salary[128];
                scanf("%128s %128s %128s %128s %c %15s", name, surname, fathername, date, &gender, salary);
                if(!has_only_date_char(salary, strlen(salary))){
                    strcpy(salary, "0\0");
                }

                int flag = 0;
                Node* tmp = livers.head;
                for (int i = 0; i < livers.len; i++)
                {
                    if(!strcmp(tmp->data.fathername.data, fathername) && !strcmp(tmp->data.name.data, name) 
                    && !strcmp(tmp->data.surname.data, surname) && !strcmp(tmp->data.birthDate.date, date) 
                    && tmp->data.gender == gender){
                        flag = 1;
                        printf("Write all changed data about the selected cityzen\nRight format:\n<Name> <Surname> <Fathername (if no fathername then '-')> <Birth date (day.month.year)> <Gender (M or W)> <salary>\n");
                        char namen[128], surnamen[128], fathernamen[128], daten[128], gendern, salaryn[128];
                        Cityzen ctz;
                        scanf("%128s %128s %128s %128s %c %15s", namen, surnamen, fathernamen, daten, &gendern, salaryn);
                        if(!has_only_date_char(salaryn, strlen(salaryn))){
                            strcpy(salaryn, "0\0");
                        }
                        long double incomeDBL = atof(salaryn);
                        Cityzen_init(&ctz, namen, surnamen, fathernamen, daten, gendern, incomeDBL);
                        Cityzen new_data, old_data;
                        Cityzen_init(&new_data, "a", "a", "a", "01.01.0101", 'M', 0.0);
                        Cityzen_init(&old_data, "a", "a", "a", "01.01.0101", 'M', 0.0);
                        Cityzen_cpy(&new_data, &ctz);
                        Cityzen_cpy(&old_data, &tmp->data);
                        Liver_overwrite(&livers, &tmp->data, &ctz);
                        Operation op = Liver_overwrite;


                        VectorCO_append(&vco, &op, &new_data);
                        VectorCO_append(&vco, &op, &old_data);
                        break;
                    }
                    tmp = tmp->next;
                }
                if(!flag){
                    printf("Cityzen wasn't found\n");
                }
                
                
            }
            break;
            case 'U':
            {
                if(vco.len == 0){
                    printf("No ability to undo\n");
                    break;
                }
                if(vco.func[vco.len - 1] == Liver_overwrite) {
                    Cityzen from = vco.livers[vco.len - 1];
                    Cityzen to = vco.livers[vco.len - 2];
                    Operation func = vco.func[vco.len - 1];
                    vco.len -= 2;
                    func(&livers, &to, &from);

                    Cityzen_delete(&to);
                }
                else{
                    Cityzen ctz = vco.livers[vco.len - 1];
                    Operation func = vco.func[vco.len - 1];
                    vco.len -= 1;
                    func(&livers, &ctz);
                }
            }
            break;
            case 'S':{
                printf("Exit\n");
            }
            break;
            case 'H':{
                printf("What do you want to do?\n0 - print all data\n1 - add cityzen\n2 - delete cityzen\n3 - overwrite info about the cityzen\nU - undo\nH - help\nS - Stop\n");
            }
            break;
            default:
                printf("Unknown key %c\n", md);
                break;
        }
    }
    if(livers.head != NULL && livers.len != 0){
        Liver_delete(&livers);
    }
    if(vco.used != 0){
        VectorCO_delete(&vco);
    }
    
    return 0;
}