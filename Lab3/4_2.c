#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_MAIL 100
#define MAX_STRING_LENGTH 100
#define POSTAL_ID_LENGTH 14
#define CREATION_TIME_LENGTH 20
#define DELIVERY_TIME_LENGTH 20

typedef struct {
    char *data;
    int length;
} String;

String createString(const char *str) {
    String newString;
    newString.length = strlen(str);
    newString.data = (char *)malloc((newString.length + 1) * sizeof(char));
    strcpy(newString.data, str);
    return newString;
}

void freeString(String *str) {
    if (str->data) {
        free(str->data);
        str->data = NULL;
        str->length = 0;
    }
}

int isNonEmptyString(const char *str) {
    if (strlen(str) == 0 || strspn(str, " ") == strlen(str)) return 0;
    return 1;
}

int isValidNaturalNumber(int number) {
    return number > 0;
}

int isValidNonNegative(double number) {
    return number >= 0;
}

int isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int isValidDate(const char *dateStr) {
    int day, month, year, hour, minute, second;

    if (sscanf(dateStr, "%2d:%2d:%4d %2d:%2d:%2d", &day, &month, &year, &hour, &minute, &second) != 6) {
        return 0;
    }

    if (month < 1 || month > 12) {
        return 0;
    }

    int daysInMonth[] = {31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // Дни в месяцах
    if (day < 1 || day > daysInMonth[month - 1]) {
        return 0;
    }

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0;
    }

    return 1;
}

typedef struct {
    String city;
    String street;
    int houseNumber;
    String building;
    int apartmentNumber;
    String postalCode;
} Address;

Address createAddress(const char *city, const char *street, int houseNumber, const char *building, int apartmentNumber, const char *postalCode) {
    Address addr;
    addr.city = createString(city);
    addr.street = createString(street);
    addr.houseNumber = houseNumber;
    addr.building = createString(building);
    addr.apartmentNumber = apartmentNumber;
    addr.postalCode = createString(postalCode);
    return addr;
}

typedef struct {
    Address recipientAddress;
    double weight;
    String postalId;
    String creationTime;
    String deliveryTime;
} Mail;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

Mail createMail(Address recipientAddress, double weight, const char *postalId, const char *creationTime, const char *deliveryTime) {
    Mail mail;
    mail.recipientAddress = recipientAddress;
    mail.weight = weight;
    mail.postalId = createString(postalId);
    mail.creationTime = createString(creationTime);
    mail.deliveryTime = createString(deliveryTime);
    return mail;
}

typedef struct {
    Address currentAddress;
    Mail *mails;
    int mailCount;
} Post;

Post createPost(Address currentAddress) {
    Post post;
    post.currentAddress = currentAddress;
    post.mails = (Mail *)malloc(MAX_MAIL * sizeof(Mail));
    post.mailCount = 0;
    return post;
}

void addMail(Post *post, Mail mail) {
    if (post->mailCount < MAX_MAIL) {
        post->mails[post->mailCount++] = mail;
        printf("Письмо с ID %s добавлено успешно!\n", mail.postalId.data);
    } else {
        printf("Массив писем полон!\n");
    }
}

void removeMail(Post *post, const char *postalId) {
    for (int i = 0; i < post->mailCount; i++) {
        if (strcmp(post->mails[i].postalId.data, postalId) == 0) {
            freeString(&post->mails[i].postalId);
            freeString(&post->mails[i].creationTime);
            freeString(&post->mails[i].deliveryTime);
            freeString(&post->mails[i].recipientAddress.city);
            freeString(&post->mails[i].recipientAddress.street);
            freeString(&post->mails[i].recipientAddress.building);
            freeString(&post->mails[i].recipientAddress.postalCode);

            for (int j = i; j < post->mailCount - 1; j++) {
                post->mails[j] = post->mails[j + 1];
            }
            post->mailCount--;
            printf("Письмо с ID %s удалено успешно!\n", postalId);
            return;
        }
    }
    printf("Письмо с ID %s не найдено!\n", postalId);
}

void sortMails(Post *post) {
    for (int i = 0; i < post->mailCount - 1; i++) {
        for (int j = i + 1; j < post->mailCount; j++) {
            if (strcmp(post->mails[i].recipientAddress.postalCode.data, post->mails[j].recipientAddress.postalCode.data) > 0 ||
                (strcmp(post->mails[i].recipientAddress.postalCode.data, post->mails[j].recipientAddress.postalCode.data) == 0 &&
                 strcmp(post->mails[i].postalId.data, post->mails[j].postalId.data) > 0)) {
                Mail temp = post->mails[i];
                post->mails[i] = post->mails[j];
                post->mails[j] = temp;
            }
        }
    }
}

void printMailInfo(const Mail *mail) {
    printf("Почтовое ID: %s\n", mail->postalId.data);
    printf("Вес: %.2f\n", mail->weight);

    printf("Время создания: %s\n", mail->creationTime.data);
    printf("Время вручения: %s\n", mail->deliveryTime.data);

    printf("Адрес получателя: %s, %s, %d, %s, %d, %s\n", 
           mail->recipientAddress.city.data, 
           mail->recipientAddress.street.data,
           mail->recipientAddress.houseNumber, 
           mail->recipientAddress.building.data,
           mail->recipientAddress.apartmentNumber, 
           mail->recipientAddress.postalCode.data);
}


int isMailDelivered(const Mail *mail) {
    if (mail->deliveryTime.length == 0) {
        return 0;
    }

    time_t t = time(NULL);
    struct tm *currentTime = localtime(&t);
    char currentTimeStr[DELIVERY_TIME_LENGTH];
    strftime(currentTimeStr, sizeof(currentTimeStr), "%d:%m:%Y %H:%M:%S", currentTime);

    return strcmp(mail->deliveryTime.data, currentTimeStr) <= 0;
}

void searchMailById(Post *post, const char *postalId) {
    for (int i = 0; i < post->mailCount; i++) {
        if (strcmp(post->mails[i].postalId.data, postalId) == 0) {
            printMailInfo(&post->mails[i]);
            if (isMailDelivered(&post->mails[i])) {
                printf("Статус: доставлено\n");
            } else {
                printf("Статус: недоставленно\n");
            }
            return;
        }
    }
    printf("Письмо с ID %s не найдено!\n", postalId);
}

void findDeliveredMails(Post *post) {
    printf("Доставленные письма:\n");
    for (int i = 0; i < post->mailCount; i++) {
        if (isMailDelivered(&post->mails[i])) {
            printMailInfo(&post->mails[i]);
            printf("\n");
        }
    }
}

void findUndeliveredMails(Post *post) {
    printf("Недоставленные письма:\n");
    for (int i = 0; i < post->mailCount; i++) {
        if (!isMailDelivered(&post->mails[i])) {
            printMailInfo(&post->mails[i]);
            printf("\n");
        }
    }
}

void freeMail(Mail *mail) {
    freeString(&mail->postalId);
    freeString(&mail->creationTime);
    freeString(&mail->deliveryTime);
    freeString(&mail->recipientAddress.city);
    freeString(&mail->recipientAddress.street);
    freeString(&mail->recipientAddress.building);
    freeString(&mail->recipientAddress.postalCode);
}

void freePost(Post *post) {
    for (int i = 0; i < post->mailCount; i++) {
        freeMail(&post->mails[i]);
    }
    free(post->mails);
}

int parseDate(const char *dateStr, struct tm *tmDate) {
    return sscanf(dateStr, "%d:%d:%d %d:%d:%d", 
                   &tmDate->tm_mday, &tmDate->tm_mon, &tmDate->tm_year, 
                   &tmDate->tm_hour, &tmDate->tm_min, &tmDate->tm_sec) == 6;
}

int compareTimes(const char *creationTimeStr, const char *deliveryTimeStr) {
    struct tm creationTime = {0};
    struct tm deliveryTime = {0};
    
    if (!parseDate(creationTimeStr, &creationTime) || !parseDate(deliveryTimeStr, &deliveryTime)) {
        return -1;
    }

    creationTime.tm_year -= 1900;
    creationTime.tm_mon -= 1;
    deliveryTime.tm_year -= 1900;
    deliveryTime.tm_mon -= 1;

    time_t creationTimeEpoch = mktime(&creationTime);
    time_t deliveryTimeEpoch = mktime(&deliveryTime);

    if (creationTimeEpoch == (time_t)-1 || deliveryTimeEpoch == (time_t)-1) {
        return -1;
    }

    if (difftime(deliveryTimeEpoch, creationTimeEpoch) < 0) {
        return 1;
    }

    return 0;
}

void userInterface() {
    char city[MAX_STRING_LENGTH], street[MAX_STRING_LENGTH], building[MAX_STRING_LENGTH], postalId[POSTAL_ID_LENGTH + 1], creationTime[CREATION_TIME_LENGTH], deliveryTime[DELIVERY_TIME_LENGTH];
    int houseNumber, apartmentNumber, choice;
    double weight;
    
    Address postAddress = createAddress("Город Почты", "Улица Почты", 1, "Здание Почты", 1, "123456");

    Post post = createPost(postAddress);

    do {
        printf("Меню:\n");
        printf("1. Добавить почту\n");
        printf("2. Удалить почту\n");
        printf("3. Найти почту по ID\n");
        printf("4. Найти доставленные письма\n");
        printf("5. Найти недоставленные письма\n");
        printf("6. Выйти\n");
        printf("Введите ваш выбор: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                printf("Введите город: ");
                fgets(city, sizeof(city), stdin);
                city[strcspn(city, "\n")] = 0;
                if (!isNonEmptyString(city)) {
                    printf("Неверный город! Город не может быть пустым\n");
                    break;
                }

                printf("Введите улицу: ");
                fgets(street, sizeof(street), stdin);
                street[strcspn(street, "\n")] = 0;
                if (!isNonEmptyString(street)) {
                    printf("Неверная улица! Улица не может быть пустой.\n");
                    break;
                }

                printf("Введите номер дома: ");
                scanf("%d", &houseNumber);
                if (!isValidNaturalNumber(houseNumber)) {
                    printf("Неверный номер дома! Пожалуйста, введите натуральное число.\n");
                    break;
                }

                printf("Введите здание: ");
                getchar();
                fgets(building, sizeof(building), stdin);
                building[strcspn(building, "\n")] = 0;

                printf("Введите номер квартиры: ");
                scanf("%d", &apartmentNumber);
                if (!isValidNaturalNumber(apartmentNumber)) {
                    printf("Неверный номер квартиры! Пожалуйста, введите натуральное число.\n");
                    break;
                }

                printf("Введите почтовый индекс (14 цифр): ");
                scanf("%s", postalId);
                clearInputBuffer();
                if (strlen(postalId) != POSTAL_ID_LENGTH || !isdigit(postalId[0])) {
                    printf("Неверный почтовый индекс! Он должен содержать 14 цифр.\n");
                    break;
                }

                printf("Введите вес: ");
                scanf("%lf", &weight);
                clearInputBuffer();
                if (!isValidNonNegative(weight)) {
                    printf("Неверный вес! Пожалуйста, введите неотрицательное число.\n");
                    break;
                }

                printf("Введите время создания (dd:MM:yyyy hh:mm:ss): ");
                fgets(creationTime, sizeof(creationTime), stdin);
                creationTime[strcspn(creationTime, "\n")] = 0;

                clearInputBuffer();

                printf("Введите время доставки (dd:MM:yyyy hh:mm:ss): ");
                fgets(deliveryTime, sizeof(deliveryTime), stdin);
                deliveryTime[strcspn(deliveryTime, "\n")] = 0;

                if (!isValidDate(creationTime)) {
                    printf("Неверный формат времени создания или дата!\n");
                    break;
                }

                if (!isValidDate(deliveryTime)) {
                    printf("Неверный формат времени доставки или дата!\n");
                    break;
                }

                if (compareTimes(creationTime, deliveryTime) == 1) {
                    printf("Ошибка: Время доставки не может быть раньше времени создания.\n");
                    break;
                } else {
                    printf("Времена корректны.\n");
                }

                Address recipientAddress = createAddress(city, street, houseNumber, building, apartmentNumber, postalId);
                Mail mail = createMail(recipientAddress, weight, postalId, creationTime, deliveryTime);
                addMail(&post, mail);
                sortMails(&post);
                break;
            }
            case 2: {
                printf("Введите почтовый индекс для удаления: ");
                scanf("%s", postalId);
                removeMail(&post, postalId);
                break;
            }
            case 3: {
                printf("Введите почтовый индекс для поиска: ");
                scanf("%s", postalId);
                searchMailById(&post, postalId);
                break;
            }
            case 4:
                findDeliveredMails(&post);
                break;
            case 5:
                findUndeliveredMails(&post);
                break;
            case 6:
                printf("Выход...\n");
                break;
            default:
                printf("Неверная опция! Попробуйте снова.\n");
        }
    } while (choice != 6);

    freePost(&post); 
}

int main() {
    userInterface();
    return 0;
}
