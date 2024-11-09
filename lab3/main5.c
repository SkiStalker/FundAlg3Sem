#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum ret_t{
    MEMORY_ERROR = 2,
    FILE_ERROR,
    ARG_ERROR
};

typedef struct 
{
    char* data;
    int len;
    int _size;
}String;


int String_init(String* str, char* data){
    str->len = strlen(data);
    str->_size = strlen(data)+ 1;
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
    unsigned int id;
    String name;
    String surname;
    String group;
    char* marks;
}Student;

typedef struct 
{
    Student* students;
    int len;
    int _size;
}VectorS;

int Student_delete(Student* s){
    if(s->marks != NULL && s != NULL){
        free(s->marks);    
    }
    String_delete(&s->name);
    String_delete(&s->surname);
    String_delete(&s->group);
    s->id = 0;
    return 1;
}

int VectorS_init(VectorS* vs){
    vs->_size = 4;
    vs->students = (Student*)malloc(sizeof(Student) * vs->_size);
    if(vs->students == NULL){
        vs->_size = 0;
        vs->len = 0;
        return MEMORY_ERROR;
    }
    vs->len = 0;
    return 1;
}

int has_only_letters(char* str, int len){
    for (int i = 0; i < len; i++)
    {
        if(!isalpha(str[i])){
            return 0;
        }
    }
    return 1;
}

int VectorS_Add(VectorS* vs, Student* st){
    if(vs == NULL || vs->students == NULL || st == NULL){
        return MEMORY_ERROR;
    }
    else if(vs->_size == vs->len){
        Student* v = (Student*)realloc(vs->students, sizeof(Student)* (vs->_size * 2));
        if(v == NULL){
            for (int i = 0; i < vs->len; i++)
            {
                Student_delete(&vs->students[i]);
            }
            free(vs->students);
            return MEMORY_ERROR;
        }
        vs->students = v;
    }
    vs->students[vs->len++] = *st;
    return 1;
}

int VectorS_delete(VectorS* vs){
    if(vs == NULL || vs->students == NULL){
        vs->len = 0;
        vs->_size = 0;
        return MEMORY_ERROR;
    }
    for (int i = 0; i < vs->len; i++)
    {
        Student_delete(&vs->students[i]);
    }
    free(vs->students);
    return 1;
}

int Student_init(Student* stud, unsigned int id, char* name, char* surname, char* group, char* marks){
    if(marks == NULL){
        return MEMORY_ERROR;
    }
    stud->marks = marks;
    stud->id = id;
    String nameS, surnameS, groupS;
    String_init(&nameS, name);
    String_init(&surnameS, surname);
    String_init(&groupS, group);
    stud->name = nameS;
    stud->surname = surnameS;
    stud->group = groupS;
    return 1;
}



int has_only_numbers(char* str, int len){
    for (int i = 0; i < len; i++)
    {
        if(!isdigit(str[i])){
            return 0;
        }
    }
    return 1;
}

int Parse_file(VectorS* vector, char* inFile){
    FILE* fi = fopen(inFile, "r");
    if(fi == NULL){
        return FILE_ERROR;
    }
    char c;
    String s;
    String_init(&s, "");
    do
    {
        c = fgetc(fi);
         while (c != EOF && c != '\n')
        {
            String_concat_ch(&s, c);
            c = fgetc(fi);
        }
        Student stud;
        char name[128], surname[128], group[128], id[1024];
        sscanf(s.data, "%1024s %128s %128s %128s", id, name, surname, group);
        if(!has_only_numbers(id, strlen(id))){
            printf("Wrong format detected! Parsing stopped!\n");
            return ARG_ERROR;
        }
        unsigned int id_ui = (unsigned int)atoi(id);
        int space_cnt = 0;
        char* marks = (char*)malloc(sizeof(char) * 5);
        int len = 0;
        if(marks == NULL){
            if(vector->len > 0 || vector->_size > 0){
                VectorS_delete(vector);
            }
            fclose(fi);
            return MEMORY_ERROR;
        }
        for (int i = 0; i < s.len; i++)
        {
            if(space_cnt >= 4 && s.data[i] != ' '){
                marks[len++] = s.data[i] - '0';
            }
            if(s.data[i] == ' '){
                space_cnt++;
            }
        }
        Student_init(&stud, id_ui, name, surname, group, marks);
        VectorS_Add(vector, &stud);
        String_assignment(&s, "");
    } while (c != EOF);
    fclose(fi);
    return 1;
}

int search_by_id(VectorS* vector, unsigned int id, VectorS* stud){
    for (int i = 0; i < vector->len; i++)
    {
        if(vector->students[i].id == id){
            VectorS_Add(stud, &vector->students[i]);
        }
    }
    return !(stud->len == 0);
}

int search_by_name(VectorS* vector, char* name, VectorS* stud){
    for (int i = 0; i < vector->len; i++)
    {
        if(!strcmp(vector->students[i].name.data, name)){
            VectorS_Add(stud, &vector->students[i]);
        }
    }
    return !(stud->len == 0);
}

int search_by_surname(VectorS* vector, char* surname, VectorS* stud){
    for (int i = 0; i < vector->len; i++)
    {
        if(!strcmp(vector->students[i].surname.data, surname)){
            VectorS_Add(stud, &vector->students[i]);
        }
    }
    return !(stud->len == 0);
}

int search_by_group(VectorS* vector, char* group, VectorS* stud){
    for (int i = 0; i < vector->len; i++)
    {
        if(!strcmp(vector->students[i].group.data, group)){
            VectorS_Add(stud, &vector->students[i]);
        }
    }
    return !(stud->len == 0);
}

int comp_id(const void* ptr1, const void* ptr2){
    Student s1, s2;
    s1 = *(const Student*)ptr1;
    s2 = *(const Student*)ptr2;

    if(s1.id < s2.id){
        return -1;
    }
    else if(s1.id == s2.id){
        return 0;
    }
    else{
        return 1;
    }
}

int comp_surname(const void* ptr1, const void* ptr2){
    Student s1, s2;
    s1 = *(const Student*)ptr1;
    s2 = *(const Student*)ptr2;

    if(String_ltr(&s1.surname, &s2.surname)){
        return -1;
    }
    else if(String_isEqual(&s1.surname, &s2.surname)){
        return 0;
    }
    else{
        return 1;
    }
}

int comp_name(const void* ptr1, const void* ptr2){
    Student s1, s2;
    s1 = *(const Student*)ptr1;
    s2 = *(const Student*)ptr2;

    if(String_ltr(&s1.name, &s2.name)){
        return -1;
    }
    else if(String_isEqual(&s1.name, &s2.name)){
        return 0;
    }
    else{
        return 1;
    }
}

int comp_group(const void* ptr1, const void* ptr2){
    Student s1, s2;
    s1 = *(const Student*)ptr1;
    s2 = *(const Student*)ptr2;

    if(String_ltr(&s1.group, &s2.group)){
        return -1;
    }
    else if(String_isEqual(&s1.group, &s2.group)){
        return 0;
    }
    else{
        return 1;
    }
}

double srzn(char* marks){
    double sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += (double)marks[i];
    }
    return sum / 5.0;
}

int put_StudData_by_id(VectorS* vector, FILE* f){
    fputs("Found student:\n", f);
    fprintf(f, "%s %s %s %f\n--------------------\n", vector->students[0].name.data, vector->students[0].surname.data, 
        vector->students[0].group.data, srzn(vector->students[0].marks));
    return 1;
}

int put_StudData_UpperMarks(VectorS* found, VectorS* def, FILE* f){
    
    double Max_sr = -__DBL_MAX__;
    for (int i = 0; i < found->len; i++)
    {
        double sr = srzn(found->students[i].marks);
        if(Max_sr < sr){
            Max_sr = sr;
        }
    }

    fputs("Found students:\n", f);
    for (int i = 0; i < found->len; i++)
    {
        fprintf(f, "\t%s %s %s %f\n", found->students[i].name.data, found->students[i].surname.data, 
                    found->students[i].group.data, srzn(found->students[i].marks));
    }
    
    
    fputs("These are students with higher average marks than the highest of found:\n", f);
    int count = 0;
    for (int i = 0; i < def->len; i++)
    {
        int flag = 0;
        for (int j = 0; j < found->len; j++)
        {
            if(found->students[j].id == def->students[i].id){
                flag = 1;
                break;
            }
        }
        
        if(!flag){
            if(Max_sr < srzn(def->students[i].marks)){
                count++;
                fprintf(f, "\t%s %s %s %f\n", def->students[i].name.data, def->students[i].surname.data, 
                    def->students[i].group.data, srzn(def->students[i].marks));
            }
        }
    }
    if(!count){
        fputs("\tNo students found...\n--------------------\n", f);
        return 1;
    }
    fputs("--------------------\n", f);
    return 1;
}

int load_Data(VectorS* vector, FILE* f){
    for (int i = 0; i < vector->len; i++)
    {
        fprintf(f, "%d %s %s %s", vector->students[i].id, vector->students[i].name.data, vector->students[i].surname.data, vector->students[i].group.data);
        for (int j = 0; j < 5; j++)
        {
            fprintf(f, " %d", vector->students[i].marks[j]);
        }
        fputc('\n', f);
    }
    fputs("--------------------\n", f);
    return 1;
}

void errorHandler(int ret){
    switch (ret)
    {
    case MEMORY_ERROR:
        printf("Memory error!\n");
        break;
    case FILE_ERROR:
        printf("File error!\n");
        break;
    default:
        break;
    }
    return;
}

int VectorS_clear(VectorS* v){
    v->len = 0; //так как то, что находится в этом массиве есть ссылки на студентов (оригинал), то удалать их не надо
    return 0;   //в результате работы программы все и так будет удалено (другой функцией)
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Wrong amount of arguments!\nRight format: ./<command> <inFile> <outFile>\n");
        return 0;
    }
    char* inFile = (char*)malloc(sizeof(char) * (strlen(argv[1] + 1)));
    char* outFile = (char*)malloc(sizeof(char) * (strlen(argv[2] + 1)));
    strcpy(inFile, argv[1]);
    strcpy(outFile, argv[2]);
    FILE* f = fopen(outFile, "w");
    if(f == NULL){
        printf("Error with file occured!\n");
        return -1;
    }
    VectorS vector;
    VectorS_init(&vector);
    int ret = Parse_file(&vector, inFile);
    if(ret != 1){
        free(inFile);
        free(outFile);
        errorHandler(ret);
        return -1;
    }
    

    char c;
    ret = 0;
    printf("0 - load data to file\n1 - sort by id\n2 - sort by name\n3 - sort by surname\n4 - sort by group\n5 - find by id\n6 - find by name\n7 - find by surname\n8 - find by group\nH - help\nS - Stop\n");
    VectorS found;
    VectorS_init(&found);
    do
    {
        //scanf("%c", &c);
        c = (char)getchar();
        switch (c)
        {
        case '0':
            ret = load_Data(&vector, f);
            printf("Loaded\n");
            break;
        case '1':{
            qsort(vector.students, vector.len, sizeof(Student), comp_id);
            printf("Sorted\n");
            break;}
        case '2':{
            qsort(vector.students, vector.len, sizeof(Student), comp_name);
            printf("Sorted\n");
            break;}
        case '3':{
            qsort(vector.students, vector.len, sizeof(Student), comp_surname);
            printf("Sorted\n");
            break;}
        case '4':{
            qsort(vector.students, vector.len, sizeof(Student), comp_group);
            printf("Sorted\n");
            break;}
        case '5':{
            printf("Input id... ");
            char id_ch[100];
            int flag = 1;
            while (flag)
            {
                scanf(" %100s", id_ch);
                if(!has_only_numbers(id_ch, strlen(id_ch))){
                    printf("Id must be unsigned integer!\nTry again...");
                    continue;
                }
                else if(strlen(id_ch) > 9){
                    printf("Id must be shorter than 10!\nTry again...");
                    continue;
                }
                flag = 0;
            }
            unsigned int id = (unsigned int)atoi(id_ch);
            int res = search_by_id(&vector, id, &found);
            if(!res){
                printf("Nothing was found\n");
            }
            else{
                printf("What do you want:\n0 - add to the file data about the student\n1 - add to the file info about higher marked students\n");
                char md;
                scanf(" %c", &md);
                switch (md)
                {
                case '0':
                    put_StudData_by_id(&found, f);
                    printf("Found students were added to the file\n");
                    break;
                case '1':
                    put_StudData_UpperMarks(&found, &vector, f);
                    printf("Found students were added to the file\n");
                    break;
                default:
                    printf("Unknown command!\n");
                    break;
                }
            }
            break;}
        case '6':{
            printf("Input name... ");
            char Name[100];
            int flag = 1;
            while (flag)
            {
                scanf(" %100s", Name);
                if(!has_only_letters(Name, strlen(Name))){
                    printf("Name must consist only from letters\nTry again...");
                }
                flag = 0;
            }
            int res = search_by_name(&vector, Name, &found);
            if(!res){
                printf("Nothing was found\n");
            }
            else{
                put_StudData_UpperMarks(&found, &vector, f);
                printf("Found students were added to the file\n");
            }
            break;}
        case '7':{
            printf("Input surname... ");
            char Name[100];
            int flag = 1;
            while (flag)
            {
                scanf("%100s", Name);
                if(!has_only_letters(Name, strlen(Name))){
                    printf("Surname must consist only from letters\nTry again...");
                }
                flag = 0;
            }
            int res = search_by_surname(&vector, Name, &found);
            if(!res){
                printf("Nothing was found\n");
            }
            else{
                put_StudData_UpperMarks(&found, &vector, f);
                printf("Found students were added to the file\n");
            }
            break;}
        case '8':{
            printf("Input group... ");
            char Name[100];
            int flag = 1;
            while (flag)
            {
                scanf("%100s", Name);
                flag = 0;
            }
            int res = search_by_group(&vector, Name, &found);
            if(!res){
                printf("Nothing was found\n");
            }
            else{
                put_StudData_UpperMarks(&found, &vector, f);
                printf("Found students were added to the file\n");
            }
            break;}
        case 'H':{
            printf("0 - load data to file\n1 - sort by id\n2 - sort by name\n3 - sort by surname\n4 - sort by group\n5 - find by id\n6 - find by name\n7 - find by surname\n8 - find by group\nH - help\nS - Stop\n");
        }
        default:
            break;
        }
        VectorS_clear(&found); 
    } while (c != 'S');
    
    fclose(f);
    free(inFile);
    free(outFile);
    return 0;
}