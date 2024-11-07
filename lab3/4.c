#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


enum Errors {
    ERROR = -1,
    SUCCESS,
};

typedef struct String {
    char* str;
    int symbols;
} String;


typedef struct Address {
    String town;
    String street;
    int house;
    String corpus;
    int apartNumber;
    String index;

} Address;

typedef struct Mail {
    Address address;
    float weight;
    String mailID;
    String createdTime;
    String deliveryTime;
} Mail;


typedef struct Post {
    Address* addres;
    Mail* mails;
    int count;
    int capacity;
} Post;

//Вспомогательные функции
char* GetNowTime() {
    time_t now;
    time(&now);

    struct tm* local = localtime(&now);
    char* buffer = (char*)malloc(20 * sizeof(char)); 
    if (buffer == NULL) {
        return NULL;
    }

    strftime(buffer, 20, "%d:%m:%Y %H:%M:%S", local);
    return buffer;
}

int ValidateIndex(char* index) {
    if (strlen(index) == 6 || strlen(index) == 14) {
        for (int i = 0; i < (int)strlen(index); i++) {
            if (!isdigit(index[i])) {
                return ERROR;
            }
        }
        return SUCCESS;
    }
    return ERROR;
}

int stringToTm(char* dateStr, struct tm* timeStruct) {
    if (sscanf(dateStr, "%d:%d:%d %d:%d:%d", 
               &timeStruct->tm_mday, &timeStruct->tm_mon, &timeStruct->tm_year,
               &timeStruct->tm_hour, &timeStruct->tm_min, &timeStruct->tm_sec) != 6) {
        return ERROR;
    }

    timeStruct->tm_mon -= 1;
    
    timeStruct->tm_year -= 1900;

    return SUCCESS;
}

int CompareTime(const void* a, const void* b) {
    Mail* m1 = (Mail*)a;
    Mail* m2 = (Mail*)b;
    struct tm t1 = {0};
    struct tm t2 = {0};
    stringToTm(m1->createdTime.str, &t1);
    stringToTm(m2->createdTime.str, &t2);
    
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    
    if (time1 < time2) {
        return -1;
    } else if (time1 > time2) {
        return 1;
    }
    return 0;
}




//Функции String
String* CreateString(char* str) {
    String* s = (String*)malloc(sizeof(String));
    if (s == NULL) {
        return NULL;
    }
    char* ptr = (char*)malloc((strlen(str) + 1) * sizeof(char));
    if (ptr == NULL) {
        return NULL;
    }
    strcpy(ptr, str);
    s->str = ptr;
    s->symbols = strlen(str);
    return s;
}

int DeleteString(String* str) {
    str->str[0] = '\0';
    str->symbols = 0;
    return SUCCESS;
}

int CompareString(String* s1, String* s2) {
    if (s1->symbols > s2->symbols) {
        return 1;
    } else if (s1->symbols == s2->symbols) {
        return strcmp(s1->str, s2->str);
    } else {
        return 0;
    }
}

int CopyString(String* s1, String* s2) {
    if (s1->str == NULL || s2->str == NULL) {
        return ERROR;
    }

    strcpy(s2->str, s1->str);
    s2->symbols = s1->symbols;
    return SUCCESS;
}


String* CopyAndCreateString(String* s) {
    if (s == NULL) {
        return NULL;
    }
    String* new_s = CreateString(s->str);
    if (new_s == NULL) {
        return NULL;
    }
    return new_s;
}

int EquivalenceString(String* s1, String* s2) {
    return (s1->symbols == s2->symbols) && (strcmp(s1->str,s2->str) == 0);
}

int ConcatenationString(String* s1, String* s2) {
    if (s1->str == NULL || s2->str == NULL) {
        return ERROR;
    }
    strcat(s1->str, s2->str);
    s1->symbols += s2->symbols;
    return SUCCESS;
}

//Функции Mail
Mail* CreateMail(Address* address, float weight, String* mailId, String* createdTime, String* deliveryTime) {
    Mail* mail = (Mail*)malloc(sizeof(Mail));
    if (mail == NULL) {
        return NULL;
    }
    mail->address = *address;
    mail->weight = weight;
    mail->mailID = *mailId;
    mail->createdTime = *createdTime;
    mail->deliveryTime = *deliveryTime;
    return mail;
}

int PrintMail(Mail* m) {
    Mail mail = *m;
    printf("------------------\n");
    printf("Town: %s\n", mail.address.town.str);
    printf("Street: %s\n", mail.address.street.str);
    printf("House: %d\n", mail.address.house);
    printf("Corpus: %s\n", mail.address.corpus.str);
    printf("ApartNumber: %d\n", mail.address.apartNumber);
    printf("Index: %s\n", mail.address.index.str);
    printf("Weight: %f\n", mail.weight);
    printf("MailID: %s\n", mail.mailID.str);
    printf("Create: %s\n", mail.createdTime.str);
    printf("Delivery: %s\n", mail.deliveryTime.str);
    printf("----------------------------------\n");
    return SUCCESS;
}

Mail* SearchMail(Post* p, char* id) {
    for (int i = 0; i < p->count; i++) {
        if (strcmp(p->mails[i].mailID.str, id) == 0) {
            return &p->mails[i];
        }
    }
    return NULL;
}

int CompareMail(const void* a, const void* b) {
    Mail* m1 = (Mail*)a;
    Mail* m2 = (Mail*)b;
    if (CompareString(&m1->address.index, &m2->address.index) != 0) {
        return CompareString(&m1->address.index, &m2->address.index);
    }

    return CompareString(&m1->mailID, &m2->mailID);
}

int SortMails(Mail* mails, int size) {
    qsort(mails, size, sizeof(Mail), CompareMail);
    return SUCCESS;
}


//Функции Address
Address* CreateAddress(String* town, String* street, int house, String* corpus, int apartNumber, String* index) {
    Address* a = (Address*)malloc(sizeof(Address));
    if (a == NULL) {
        return NULL;
    }
    a->town = *town;
    a->street = *street;
    a->house = house;
    a->corpus = *corpus;
    a->apartNumber = apartNumber;
    a->index = *index;
    return a;
}

//Функции Post
Post* CreatePost(Address* address) {
    Post* p = (Post*)malloc(sizeof(Post));
    if (p == NULL) {
        return NULL;
    }
    p->addres = address;
    Mail* mails = (Mail*)malloc(10 * sizeof(Mail));
    if (mails == NULL) {
        return NULL;
    }
    p->mails = mails;
    p->count = 0;
    p->capacity = 10;
    return p;
}

int AddMail(Post* p, Mail* m) {
    if (p->count >= p->capacity) {
        p->capacity *= 2;
        p->mails = (Mail*)realloc(p->mails, p->capacity * sizeof(Mail));
        if (p->mails == NULL) {
            return ERROR;
        }
    }
    p->mails[p->count++] = *m;
    SortMails(p->mails, p->count);
    return SUCCESS;
}

int DeleteMail(Post* p, Mail* m) {
    int index = -1;

    for (int i = 0; i < p->count; i++) {
        if (EquivalenceString(&p->mails[i].mailID, &m->mailID)) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return ERROR;
    }

    for (int i = index; i < p->count - 1; i++) {
        p->mails[i] = p->mails[i + 1];
    }

    p->count--;
    SortMails(p->mails, p->count);

    return SUCCESS;
}

int DeliveryMail(Mail* m) {
    m->deliveryTime = *CreateString(GetNowTime());
    return SUCCESS;
}

int PrintPost(Post* p) {
    for (int i = 0; i < p->count; i++) {
        Mail mail = p->mails[i];
        printf("Town: %s\n", mail.address.town.str);
        printf("Street: %s\n", mail.address.street.str);
        printf("House: %d\n", mail.address.house);
        printf("Corpus: %s\n", mail.address.corpus.str);
        printf("ApartNumber: %d\n", mail.address.apartNumber);
        printf("Index: %s\n", mail.address.index.str);
        printf("Weight: %f\n", mail.weight);
        printf("MailID: %s\n", mail.mailID.str);
        printf("Create: %s\n", mail.createdTime.str);
        printf("Delivery: %s\n", mail.deliveryTime.str);
        printf("----------------------------------\n");
    }
    return SUCCESS;
}


int PrintDelivery(Post* p) {
    Mail* m = (Mail*)malloc(p->count * sizeof(Mail));
    int n = 0;
    for (int i = 0; i < p->count; i++) {
        Mail mail = p->mails[i];
        if (strlen(mail.deliveryTime.str) == 19) {
            m[n] = mail;
            n++;
        }
    }
    
    qsort(m, n, sizeof(Mail), CompareTime);
    for (int i = 0; i < n; i++) {
        Mail mail = m[i];
        printf("Town: %s\n", mail.address.town.str);
        printf("Street: %s\n", mail.address.street.str);
        printf("House: %d\n", mail.address.house);
        printf("Corpus: %s\n", mail.address.corpus.str);
        printf("ApartNumber: %d\n", mail.address.apartNumber);
        printf("Index: %s\n", mail.address.index.str);
        printf("Weight: %f\n", mail.weight);
        printf("MailID: %s\n", mail.mailID.str);
        printf("Create: %s\n", mail.createdTime.str);
        printf("Delivery: %s\n", mail.deliveryTime.str);
        printf("----------------------------------\n");
    }
    free(m);
    return SUCCESS;
}

int PrintNotDelivery(Post* p) {
    Mail* m = (Mail*)malloc(p->count * sizeof(Mail));
    int n = 0;
    for (int i = 0; i < p->count; i++) {
        Mail mail = p->mails[i];
        if (strlen(mail.deliveryTime.str) == 0) {
            m[n] = mail;
            n++;
        }
    }
    
    qsort(m, n, sizeof(Mail), CompareTime);
    for (int i = 0; i < n; i++) {
        Mail mail = m[i];
        printf("Town: %s\n", mail.address.town.str);
        printf("Street: %s\n", mail.address.street.str);
        printf("House: %d\n", mail.address.house);
        printf("Corpus: %s\n", mail.address.corpus.str);
        printf("ApartNumber: %d\n", mail.address.apartNumber);
        printf("Index: %s\n", mail.address.index.str);
        printf("Weight: %f\n", mail.weight);
        printf("MailID: %s\n", mail.mailID.str);
        printf("Create: %s\n", mail.createdTime.str);
        printf("Delivery: %s\n", mail.deliveryTime.str);
        printf("----------------------------------\n");
    }
    free(m);
    return SUCCESS;
}

int FreePost(Post* p) {
    for (int i = 0; i < p->count; i++) {

        free(p->mails[i].address.town.str);
        free(p->mails[i].address.street.str);
        free(p->mails[i].address.corpus.str);
        free(p->mails[i].address.index.str);

        free(p->mails[i].mailID.str);
        free(p->mails[i].createdTime.str);
        free(p->mails[i].deliveryTime.str);
    }

    free(p->mails);

    free(p->addres->town.str);
    free(p->addres->street.str);
    free(p->addres->corpus.str);
    free(p->addres->index.str);

    free(p->addres);

    free(p);

    return SUCCESS;
}




int main() {



    //Почта России
    Post* post = CreatePost(CreateAddress(CreateString("Москва"), CreateString("Панфилова"), 20, CreateString("4"), 7204, CreateString("734123")));
    Address* a = CreateAddress(CreateString("Москва"), CreateString("Ленина"), 12, CreateString("3"), 724, CreateString("000000"));
    AddMail(post, CreateMail(a, 32.4, CreateString("00000000000000"), CreateString("10:11:2006 10:22:32"), CreateString("01:11:2005 10:02:32")));
    a = CreateAddress(CreateString("Москва"), CreateString("Пушкина"), 3, CreateString("11"), 24, CreateString("000000"));
    AddMail(post, CreateMail(a, 3.2, CreateString("00000000000000"), CreateString("12:10:2005 09:22:32"), CreateString("01:11:2006 10:02:32")));

    
    char addMail[15] = "add";
    char printPost[15] = "print";
    char stop[15] = "stop";
    char search[15] = "search";
    char delete[15] = "delete";
    char delivery[15] = "delivery";
    char printDelivery[15] = "printDelivery";
    char printNotdelivery[20] = "printNotDelivery";
    printf("----------------------------------\n");
    printf("add - добавить новую посылку\n");
    printf("print - вывести список всех посылок\n");
    printf("stop - завершить программу\n");
    printf("search - поиск посылки по Id\n");
    printf("delete - удаление посылки по Id\n");
    printf("delivery - доставка посылки по Id\n");
    printf("printDelivery - cписок всех доставленных поссылок\n");
    printf("printNotDelivery - cписок всех не доставленных поссылок\n");
    printf("----------------------------------\n");

    while (1) {
        
        char command[20];
        scanf("%s", command);

        if (strcmp(command, addMail) == 0) {
            printf("Введите данные: <Город> <Улица> <Дом> <Корпус> <Номер квартиры> <Индекс> <Вес посылки> <Почтовый идентификатор>\n");
            char town[50], street[50], corpus[10], index[10], mailid[20];
            int house, apart;
            float weight;

            scanf("%s %s %d %s %d %s %f %s", town, street, &house, corpus, &apart, index, &weight, mailid);
            if ((ValidateIndex(index) == ERROR) || (ValidateIndex(mailid) == ERROR)) {
                printf("Ошибка в индексе или почтовом индетификаторе\n");
                return ERROR;
            }
            Address* a = CreateAddress(CreateString(town), CreateString(street), house, CreateString(corpus), apart, CreateString(index));
            int res = AddMail(post, CreateMail(a, weight, CreateString(mailid), CreateString(GetNowTime()), CreateString("")));
            if (res == ERROR) {
                printf("Ошибка при добавлении\n");
            } else {
                printf("Посылка добавлена\n");
            }
        } else if (strcmp(command, printPost) == 0) {
            PrintPost(post);
        } else if (strcmp(command, stop) == 0) {
            FreePost(post);
            break;
        } else if (strcmp(command, printDelivery) == 0) {
            PrintDelivery(post);
        } else if (strcmp(command, printNotdelivery) == 0) {
            PrintNotDelivery(post);
        }else if (strcmp(command, search) == 0) {
            printf("Введите ID посылки\n");
            char id[14];
            scanf("%s", id);
            Mail* m = SearchMail(post, id);
            if (m == NULL) {
                printf("Такой посылки не существует\n");
                return ERROR;
            } else {
                PrintMail(m);
            }
        } else if (strcmp(command, delete) == 0) {
            printf("Введите ID посылки\n");
            char id[14];
            scanf("%s", id);
            Mail* m = SearchMail(post, id);
            if (m == NULL) {
                printf("Такой посылки не существует\n");
                return ERROR;
            } else {
                int res = DeleteMail(post, m);
                if (res == ERROR) {
                    printf("Посылка не существует или неправильный id\n");
                    return ERROR;
                } else {
                    printf("Посылка удалена\n");
                }
            }

        } else if (strcmp(command, delivery) == 0) {
            printf("Введите ID посылки\n");
            char id[14];
            scanf("%s", id);
            Mail* m = SearchMail(post, id);
            int res = DeliveryMail(m);
            if (res == ERROR) {
                printf("Посылка не существует или неправильный id\n");
            } else {
                printf("Посылка доставлена\n");
            }
        } else {
            printf("Неизвестная команда. Пожалуйста, попробуйте снова.\n");
            return ERROR;
        }
    }


}
