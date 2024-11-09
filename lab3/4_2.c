#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    char *data;
    int length;
} String;

typedef struct {
    String city;
    String street;
    unsigned int house_number;
    String building;
    unsigned int apartment_number;
    String index;
} Address;

typedef struct {
    Address recipient_address;
    double weight;
    String postal_id;
    String creation_time;
    String delivery_time;
} Mail;

typedef struct {
    Address *post_address;
    Mail *mails;
    size_t mail_count;
} Post;

int createString(String *str, const char *input) {
    str->length = strlen(input);
    str->data = (char *)malloc((str->length + 1) * sizeof(char));
    if (str->data == NULL) {
        return 1;
    }
    strcpy(str->data, input);
    return 0;
}

int deleteString(String *str) {
    free(str->data);
    str->data = NULL;
    str->length = 0;
    return 0;
}

int initializeAddress(Address *address) {
    if (createString(&address->city, "") || createString(&address->street, "") || 
        createString(&address->building, "") || createString(&address->index, "")) {
        return 1;
    }
    address->house_number = 0;
    address->apartment_number = 0;
    return 0;
}

int freeAddress(Address *address) {
    deleteString(&address->city);
    deleteString(&address->street);
    deleteString(&address->building);
    deleteString(&address->index);
    return 0;
}

int initializeMail(Mail *mail) {
    if (initializeAddress(&mail->recipient_address) || 
        createString(&mail->postal_id, "") || createString(&mail->creation_time, "") || 
        createString(&mail->delivery_time, "")) {
        return 1;
    }
    mail->weight = 0.0;
    return 0;
}

int freeMail(Mail *mail) {
    freeAddress(&mail->recipient_address);
    deleteString(&mail->postal_id);
    deleteString(&mail->creation_time);
    deleteString(&mail->delivery_time);
    return 0;
}

int initializePost(Post *post) {
    post->post_address = (Address *)malloc(sizeof(Address));
    if (post->post_address == NULL || initializeAddress(post->post_address)) {
        return 1;
    }
    post->mails = NULL;
    post->mail_count = 0;
    return 0;
}

int freePost(Post *post) {
    freeAddress(post->post_address);
    for (size_t i = 0; i < post->mail_count; i++) {
        freeMail(&post->mails[i]);
    }
    free(post->mails);
    return 0;
}

int addMail(Post *post, Mail mail) {
    post->mails = (Mail *)realloc(post->mails, (post->mail_count + 1) * sizeof(Mail));
    if (post->mails == NULL) {
        return 1;
    }
    post->mails[post->mail_count] = mail;
    post->mail_count++;
    return 0;
}

int printMail(const Mail *mail) {
    printf("Postal ID: %s\n", mail->postal_id.data);
    printf("Recipient Address: %s, %s, %u, %s, %u, %s\n",
           mail->recipient_address.city.data,
           mail->recipient_address.street.data,
           mail->recipient_address.house_number,
           mail->recipient_address.building.data,
           mail->recipient_address.apartment_number,
           mail->recipient_address.index.data);
    printf("Weight: %.2f\n", mail->weight);
    printf("Creation Time: %s\n", mail->creation_time.data);
    printf("Delivery Time: %s\n", mail->delivery_time.data);
    printf("---------------------------------\n");
    return 0;
}

int printAllMails(Post *post) {
    printf("All Mails:\n");
    for (size_t i = 0; i < post->mail_count; i++) {
        printMail(&post->mails[i]);
    }
    return 0;
}

int searchMailById(Post *post, String *id) {
    for (size_t i = 0; i < post->mail_count; i++) {
        if (strcmp(post->mails[i].postal_id.data, id->data) == 0) {
            printMail(&post->mails[i]);
            return 0;
        }
    }
    printf("Mail with ID %s not found.\n", id->data);
    return 1;
}

int sortMails(Post *post) {
    for (size_t i = 0; i < post->mail_count - 1; i++) {
        for (size_t j = i + 1; j < post->mail_count; j++) {
            if (strcmp(post->mails[i].recipient_address.index.data, post->mails[j].recipient_address.index.data) > 0 ||
                (strcmp(post->mails[i].recipient_address.index.data, post->mails[j].recipient_address.index.data) == 0 &&
                 strcmp(post->mails[i].postal_id.data, post->mails[j].postal_id.data) > 0)) {
                Mail temp = post->mails[i];
                post->mails[i] = post->mails[j];
                post->mails[j] = temp;
            }
        }
    }
    return 0;
}

int printDeliveredMails(Post *post) {
    for (size_t i = 0; i < post->mail_count; i++) {
        struct tm creation_time = {0};
        struct tm delivery_time = {0};
        time_t now = time(NULL);

        sscanf(post->mails[i].creation_time.data, "%d:%d:%d %d:%d:%d",
               &creation_time.tm_mday, &creation_time.tm_mon, &creation_time.tm_year,
               &creation_time.tm_hour, &creation_time.tm_min, &creation_time.tm_sec);
        creation_time.tm_mon -= 1;
        creation_time.tm_year -= 1900;

        sscanf(post->mails[i].delivery_time.data, "%d:%d:%d %d:%d:%d",
               &delivery_time.tm_mday, &delivery_time.tm_mon, &delivery_time.tm_year,
               &delivery_time.tm_hour, &delivery_time.tm_min, &delivery_time.tm_sec);
        delivery_time.tm_mon -= 1;
        delivery_time.tm_year -= 1900;

        if (difftime(now, mktime(&delivery_time)) > 0) {
            printMail(&post->mails[i]);
        }
    }
    return 0;
}


int isValidPostalID(const char *postal_id) {
    if (strlen(postal_id) != 14) return 0;
    for (int i = 0; i < 14; i++) {
        if (!isdigit(postal_id[i])) return 0;
    }
    return 1;
}

int isValidIndex(const char *index) {
    if (strlen(index) != 6) return 0;
    for (int i = 0; i < 6; i++) {
        if (!isdigit(index[i])) return 0;
    }
    return 1;
}

int strptime(const char* datetime, const char* format, struct tm* tm) {
    int ret;
    if (strcmp(format, "%d:%m:%Y %H:%M:%S") == 0) {
        ret = sscanf(datetime, "%d:%d:%d %d:%d:%d", 
            &tm->tm_mday, &tm->tm_mon, &tm->tm_year, 
            &tm->tm_hour, &tm->tm_min, &tm->tm_sec);
        if (ret == 6) {
            tm->tm_mon -= 1;  // Месяцы начинаются с 0
            tm->tm_year -= 1900;  // Год с 1900
            return 1;
        }
    }
    return 0;
}


int isValidDateTime(const char *datetime) {
    struct tm tm;
    return strptime(datetime, "%d:%m:%Y %H:%M:%S", &tm) == 1;
}


int interactiveMenu(Post *post) {
    int choice;
    while (1) {
        printf("1. Add Mail\n");
        printf("2. Search Mail by ID\n");
        printf("3. Print Delivered Mails\n");
        printf("4. Sort Mails\n");
        printf("5. Print All Mails\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                Mail mail;
                initializeMail(&mail);

                char buffer[256];

                // Ввод и проверка ID почты
                printf("Enter postal ID (14 digits): ");
                fgets(buffer, sizeof(buffer), stdin);
                strtok(buffer, "\n");
                while (!isValidPostalID(buffer)) {
                    printf("Invalid postal ID. It must be exactly 14 digits.\n");
                    printf("Enter postal ID (14 digits): ");
                    fgets(buffer, sizeof(buffer), stdin);
                    strtok(buffer, "\n");
                }
                createString(&mail.postal_id, buffer);

                // Ввод и проверка города получателя
                printf("Enter recipient city: ");
                fgets(buffer, sizeof(buffer), stdin);
                createString(&mail.recipient_address.city, strtok(buffer, "\n"));

                // Ввод и проверка улицы получателя
                printf("Enter recipient street: ");
                fgets(buffer, sizeof(buffer), stdin);
                createString(&mail.recipient_address.street, strtok(buffer, "\n"));

                // Ввод и проверка номера дома
                printf("Enter house number: ");
                while (scanf("%u", &mail.recipient_address.house_number) != 1) {
                    printf("Invalid input. Enter a valid house number: ");
                    while (getchar() != '\n'); // Очищаем буфер
                }
                getchar(); // Убираем символ новой строки

                // Ввод и проверка здания (необязательное поле)
                printf("Enter building (optional): ");
                fgets(buffer, sizeof(buffer), stdin);
                createString(&mail.recipient_address.building, strtok(buffer, "\n"));

                // Ввод и проверка номера квартиры
                printf("Enter apartment number: ");
                while (scanf("%u", &mail.recipient_address.apartment_number) != 1) {
                    printf("Invalid input. Enter a valid apartment number: ");
                    while (getchar() != '\n');
                }
                getchar();

                // Ввод и проверка индекса
                printf("Enter index (6 digits): ");
                fgets(buffer, sizeof(buffer), stdin);
                strtok(buffer, "\n");
                while (!isValidIndex(buffer)) {
                    printf("Invalid index. It must be exactly 6 digits.\n");
                    printf("Enter index (6 digits): ");
                    fgets(buffer, sizeof(buffer), stdin);
                    strtok(buffer, "\n");
                }
                createString(&mail.recipient_address.index, buffer);

                // Ввод и проверка веса
                printf("Enter weight: ");
                while (scanf("%lf", &mail.weight) != 1 || mail.weight < 0) {
                    printf("Invalid input. Enter a positive weight: ");
                    while (getchar() != '\n');
                }
                getchar();

                // Ввод и проверка времени создания
                printf("Enter creation time (dd:MM:yyyy hh:mm:ss): ");
                fgets(buffer, sizeof(buffer), stdin);
                strtok(buffer, "\n");
                while (!isValidDateTime(buffer)) {
                    printf("Invalid date-time format. Please enter in the format dd:MM:yyyy hh:mm:ss.\n");
                    printf("Enter creation time (dd:MM:yyyy hh:mm:ss): ");
                    fgets(buffer, sizeof(buffer), stdin);
                    strtok(buffer, "\n");
                }
                createString(&mail.creation_time, buffer);

                // Ввод и проверка времени доставки
                printf("Enter delivery time (dd:MM:yyyy hh:mm:ss): ");
                fgets(buffer, sizeof(buffer), stdin);
                strtok(buffer, "\n");
                while (!isValidDateTime(buffer)) {
                    printf("Invalid date-time format. Please enter in the format dd:MM:yyyy hh:mm:ss.\n");
                    printf("Enter delivery time (dd:MM:yyyy hh:mm:ss): ");
                    fgets(buffer, sizeof(buffer), stdin);
                    strtok(buffer, "\n");
                }
                createString(&mail.delivery_time, buffer);

                // Добавление почты
                if (addMail(post, mail) == 0) {
                    printf("Mail added successfully.\n");
                } else {
                    printf("Error adding mail.\n");
                    freeMail(&mail); // Освобождаем память в случае ошибки
                }
                break;
            }

            case 2: {
                String id;
                createString(&id, "");
                char buffer[256];
                printf("Enter postal ID to search: ");
                fgets(buffer, sizeof(buffer), stdin);
                createString(&id, strtok(buffer, "\n"));
                searchMailById(post, &id);
                deleteString(&id);
                break;
            }
            case 3:
                printDeliveredMails(post);
                break;
            case 4:
                sortMails(post);
                printf("Mails sorted.\n");
                break;
            case 5:
                printAllMails(post);
                break;
            case 0:
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

int main() {
    Post post;
    initializePost(&post);
    interactiveMenu(&post);
    freePost(&post);
    return 0;
}
