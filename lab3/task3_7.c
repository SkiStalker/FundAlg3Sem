#include <stdio.h>
#include <math.h>
#include <float.h>
#include <ctype.h> 
#include <string.h> 
#include <stdlib.h> 

typedef struct {
    char surname[BUFSIZ];
    char name[BUFSIZ];
    char patronymic[BUFSIZ];
    int date;
    int month;
    int year;
    char gender;
    double profit;
}Liver;

int is_alleter(char* str) {
    while (*str) {
        if (!isalpha(*str))
            return 0;
        str++;
    }
    return 1;
}

void copy_liver(Liver* l1, Liver* l2){
    strcpy(l1->surname, l2->surname);
    strcpy(l1->name, l2->name);
    strcpy(l1->patronymic, l2->patronymic);
    l1->date = l2->date;
    l1->month = l2->month;
    l1->year = l2->year;
    l1->gender = l2->gender;
    l1->profit = l2->profit;
}

void init_liver(Liver* l, char* s, char* n, char* pa, int d, int m, int y, char g, double pr){
    strcpy(l->surname, n);
    strcpy(l->name, s);
    strcpy(l->patronymic, pa);
    l->date = d;
    l->month = m;
    l->year = y;
    l->gender = g;
    l->profit = pr;
}

int check_data(char* s, char* n, char* pa, char g, double pr){
    if (!is_alleter(s) || !is_alleter(n) || !is_alleter(pa)) 
        return 4;
    if (g != 'M' && g != 'W')
        return 6;
    if (pr < 0) 
        return 7;
    return 0;
}

int is_equal(Liver* l1, Liver* l2) {
    return !strcmp(l1->surname, l2->surname) &&
        !strcmp(l1->name, l2->name) &&
        !strcmp(l1->patronymic, l2->patronymic) &&
        l1->date == l2->date && l1->month == l2->month &&
        l1->year == l2->year && l1->gender == l2->gender &&
        fabs(l1->profit - l2->profit) < DBL_MIN;
}

void print_liver(Liver* l) {
    printf("%s %s %s %d %d %d %c %f\n",
        l->name, l->surname, l->patronymic,
        l->date, l->month, l->year, l->gender, l->profit);
}

void fprintf_liver(Liver* l, FILE* ofile) {
    fprintf(ofile, "%s %s %s %d %d %d %c %f\n",
        l->name, l->surname, l->patronymic,
        l->date, l->month, l->year, l->gender, l->profit);
}

int is_older(Liver* l1, Liver* l2) {
    if (l1->year != l2->year)
        return l1->year < l2->year;
    if (l1->month != l2->month)
        return l1->month < l2->month;
    return l1->date <= l2->date;
}

int is_num(char* str) {
    while (*str) {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int read_liver(char* str, Liver** new_liver) {
    int cnt_fields = 0;
    *new_liver = (Liver*)malloc(sizeof(Liver));
    if (!new_liver)
        return 3;
    int last = strlen(str) - 1;
    if (str[last] == '\n')
        str[last] = '\0';
    char* buf = strtok(str, " ,");
    while (buf && cnt_fields < 8) {
        //printf("%d: %s\n", cnt_fields, buf);
        double cur_profit = 0;
        switch (cnt_fields) {
        case 0:
            if (!is_alleter(buf)) {
                free(*new_liver);
                return 4;
            }
            strcpy((*new_liver)->name, buf);
            break;
        case 1:
            if (!is_alleter(buf)) {
                free(*new_liver);
                return 4;
            }
            strcpy((*new_liver)->surname, buf);
            break;
        case 2:
            if (is_num(buf)) {
                strcpy((*new_liver)->patronymic, "");
                (*new_liver)->date = atoi(buf);
                cnt_fields++;
                break;
            }
            else if (!is_alleter(buf)) {
                free(*new_liver);
                return 4;
            }
            strcpy((*new_liver)->patronymic, buf);
            break;
        case 3:
            if (!is_num(buf)) {
                free(*new_liver);
                return 5;
            }
            (*new_liver)->date = atoi(buf);
            break;
        case 4:
            if (!is_num(buf)) {
                free(*new_liver);
                return 5;
            }
            (*new_liver)->month = atoi(buf);
            break;
        case 5:
            if (!is_num(buf)) {
                free(*new_liver);
                return 5;
            }
            (*new_liver)->year = atoi(buf);
            break;
        case 6:
            if (strlen(buf) != 1 || (buf[0] != 'M' && buf[0] != 'W')) {
                free(*new_liver);
                return 6;
            }
            (*new_liver)->gender = buf[0];
            break;
        case 7:
            cur_profit = atof(buf);
            if (cur_profit < 0) {
                return 7;
            }
            (*new_liver)->profit = cur_profit;
            break;
        default:
            return 8;
        }
        buf = strtok(NULL, " ,");
        cnt_fields++;
    }
    return 0;
}

void print_error(int res) {
    if (res == 3)
        printf("Memory allocation error");
    if (res == 4)
        printf("Name, surname and patronymic must contain only letters");
    if (res == 5)
        printf("Date, month and year must be numbers");
    if (res == 6)
        printf("Incorrect gender");
    if (res == 7)
        printf("Profit must be unsigned");
    if (res == 8)
        printf("Incorrect number of fields");
}

typedef struct Node {
    Liver* val;
    struct Node* next;
}Node;

int append(Liver* val, Node** head) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node)
        return 0;
    new_node->val = val;
    if (!(*head) || is_older((*head)->val, val)) {
        new_node->next = *head;
        *head = new_node;
        return 1;
    }
    Node* ptn = *head;
    while (ptn->next) {
        if (is_older(val, ptn->val) && is_older(ptn->next->val, val))
            break;
        ptn = ptn->next;
    }
    new_node->next = ptn->next;
    ptn->next = new_node;
    return 1;
}

int del(Liver* val, Node** head){
    Node* ptn;
    if (is_equal(val, (*head)->val)){
        free((*head)->val);
        ptn = (*head)->next;
        free(*head);
        (*head) = ptn;
        return 1;
    }
    int flag = 0;
    ptn = *head;
    while (ptn->next){
        if (is_equal(val, ptn->next->val)){
            flag = 1;
            break;
        }
        ptn = ptn->next;
    }
    if (!flag)
        return 0;
    Node* ptn1 = ptn->next;
    free(ptn1->val);
    ptn->next = ptn1->next;
    free(ptn1);
    return 1;
}

void print_lst(Node* head) {
    while (head) {
        print_liver(head->val);
        head = head->next;
    }
    printf("\n");
}

int fprintf_lst(Node* head, char* filename) {
    FILE* ofile = fopen(filename, "w");
    if (!ofile)
        return 0;
    while (head) {
        fprintf_liver(head->val, ofile);
        head = head->next;
    }
    fprintf(ofile, "\n");
    fclose(ofile);
    return 1;
}

void destroy(Node** head) {
    Node* ptn;
    while (*head) {
        free((*head)->val);
        ptn = (*head)->next;
        free(*head);
        *head = ptn;
    }
}

Node* copy_lst(Node* head){
    Node* new_head = NULL;
    while (head){
        Liver* new_liver = (Liver*)malloc(sizeof(Liver));
        if (!new_liver){
            destroy(&new_head);
            return NULL;
        }
        copy_liver(new_liver, head->val);
        append(new_liver, &new_head);
        head = head->next;
    }
    return new_head;
}

int scanf_liver(Liver* l){
    char name[BUFSIZ], sname[BUFSIZ], patr[BUFSIZ];
    int date, month, year;
    char gender; 
    double profit;
    printf("Input liver's data\n");
    scanf("%s %s %s %d %d %d %c %lf", sname, name, patr, &date, &month, &year, &gender, &profit);
    if(!check_data(sname, name, patr, gender, profit))
        return 0;
    init_liver(l, sname, name, patr, date, month, year, gender, profit);
    return 1;
}

int main(int argc, char* argv[])
{

    if (argc != 2) {
        printf("Incorrect number of arguments");
        return 1;
    }

    FILE* ifile = fopen(argv[1], "r");
    if (!ifile) {
        printf("File %s opening error", argv[1]);
        return 2;
    }

    char cur_string[BUFSIZ];
    Node* head = NULL;
    int size = 0;
    while (fgets(cur_string, BUFSIZ, ifile)) {
        Liver* l = NULL;
        int res = read_liver(cur_string, &l);
        if (res) {
            print_error(res);
            return res;
        }
        if (!append(l, &head)) {
            printf("Memory allocation error");
            destroy(&head);
            free(l);
            return 3;
        }
        size++;
    }
    
    
    
    printf("Data was successfully read.\n");
    printf("Input 1 - to find and change liver, 2 - to append liver, 3 - to delete liver,\n");
    printf("4 - to upload data in file, 5 - to stop program.\n");
    
    int n = 0, command = 0;
    

    while (1) {
        
        
        scanf("%d", &command);
        if (command < 1 || command > 5)
            printf("Incorrect command.\n");
        else if (command == 5)
            break;
        else if (command == 4) {
            char filename[BUFSIZ];
            printf("Input filename\n");
            scanf("%s", filename);
            if (!fprintf_lst(head, filename)) {
                printf("File %s opening error", filename);
                return 2;
            }
        }
        else if (command == 3) {
            if (!size)
                printf("List is empty\n");
            else{
                Liver l;
                if (!scanf_liver(&l))
                    printf("Incorrect input format\n");
                //print_liver(&l);
                else if (!del(&l, &head))
                    printf("No such liver\n");
                else{
                    size--;
                    n++;
                }
            }
        }
        else if (command == 2) {
            Liver* l = (Liver*)malloc(sizeof(Liver));
            if (!l){
                printf("Memory allocation error\n");
                destroy(&head);
                return 3;
            }
            if (!scanf_liver(l)){
                printf("Incorrect input format\n");
                free(l);
            }
            else if (!append(l, &head)){
                free(l);
                printf("Memory allocation error\n");
                destroy(&head);
                return 3;
            }
            else{
                size++;
                n++;
            }
        }
        else if (command == 1) {
            if (!size)
                printf("List is empty\n");
            else{
                Liver* l = (Liver*)malloc(sizeof(Liver));
                if (!l){
                    printf("Memory allocation error\n");
                    destroy(&head);
                    return 3;
                }
                if (!scanf_liver(l)){
                    printf("Incorrect input format\n");
                    free(l);
                }
                else if (!del(l, &head)){
                    printf("No such liver\n");
                    free(l);
                }
                else if (!scanf_liver(l)){
                    printf("Incorrect input format\n");
                    free(l);
                }
                else if (!append(l, &head)){
                    free(l);
                    printf("Memory allocation error\n");
                    destroy(&head);
                    return 3;
                }
                else{
                    n++;
                }
            }
        }
        
    }
    
    //print_lst(head);
    printf("Program successfully finished");
    destroy(&head);
    
    return 0;
}
