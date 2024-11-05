#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef enum {
    SUCCESS,
    ERROR_MEMORY,
    ERROR_INVALID_INPUT
} ErrorCode;

// Структура для строки
typedef struct {
    char* data;
    int length;
} String;

// Создание строки на основе char*
ErrorCode create_string(String* str, const char* initial) {
    if (!initial) {
        return ERROR_INVALID_INPUT;
    }
    str->length = strlen(initial);
    str->data = (char*)malloc(str->length + 1);
    if (!str->data) {
        return ERROR_MEMORY;
    }
    strcpy(str->data, initial);
    return SUCCESS;
}

// Удаление содержимого строки
void delete_string(String* str) {
    if (str->data != NULL) {
        free(str->data);
    }
    str->data = NULL;
    str->length = 0;
}

// Сравнение строк по длине и лексикографически
int compare_strings(const String* str1, const String* str2) {
    if (str1->length != str2->length) {
        return str1->length - str2->length;
    }
    return strcmp(str1->data, str2->data);
}

// Проверка эквивалентности строк
int areStringsEqual(const String* str1, const String* str2) {
    return strcmp(str1->data, str2->data) == 0;
}

// Копирование содержимого в существующий экземпляр String
ErrorCode copyToExistingString(String* dest, const String* src) {
    if (!src->data) {
        return ERROR_INVALID_INPUT;
    }
    if (dest->data) {
        free(dest->data);
    }
    dest->data = (char*)malloc(src->length + 1);
    if (!dest->data) {
        return ERROR_MEMORY;
    }
    strcpy(dest->data, src->data);
    dest->length = src->length;
    return SUCCESS;
}

// Копирование содержимого в новый экземпляр String
ErrorCode copyToNewString(String** dest, const String* src) {
    if (!src->data) {
        return ERROR_INVALID_INPUT;
    }
    *dest = (String*)malloc(sizeof(String));
    if (!*dest) {
        return ERROR_MEMORY;
    }
    (*dest)->data = (char*)malloc(src->length + 1);
    if (!(*dest)->data) {
        free(*dest);
        return ERROR_MEMORY;
    }
    strcpy((*dest)->data, src->data);
    (*dest)->length = src->length;
    return SUCCESS;
}

// Конкатенация двух строк
ErrorCode concatenate_strings(String* dest, const String* src) {
    char* newData = (char*)realloc(dest->data, dest->length + src->length + 1);
    if (!newData) {
        return ERROR_MEMORY;
    }
    strcat(newData, src->data);
    dest->data = newData;
    dest->length += src->length;
    return SUCCESS;
}

// Структура Address
typedef struct {
    String city;
    String street;
    int houseNumber;
    String building;
    int apartment;
    String postalCode;
} Address;

// Структура Mail
typedef struct {
    Address recipientAddress;
    float weight;           // Вес посылки
    String parcelID;       // Идентификатор посылки 4
    String creationTime;    // Время создания
    String deliveryTime;    // Время доставки
} Mail;

typedef struct {
    Address* postOfficeAddress;
    Mail* mails;
    int mailCount;
} Post;

ErrorCode createAddress(Address* address, const char* city, const char* street, int houseNumber, const char* building, int apartment, const char* postalCode) {
    if (create_string(&address->city, city) != SUCCESS ||
        create_string(&address->street, street) != SUCCESS ||
        create_string(&address->building, building) != SUCCESS ||
        create_string(&address->postalCode, postalCode) != SUCCESS) {
        return ERROR_MEMORY;
    }
    address->houseNumber = houseNumber;
    address->apartment = apartment;
    return SUCCESS;
}

void deleteAddress(Address* address) {
    delete_string(&address->city);
    delete_string(&address->street);
    delete_string(&address->building);
    delete_string(&address->postalCode);
}

ErrorCode addMail(Post* post, Mail mail) {
    if (!post) {
        return ERROR_INVALID_INPUT;
    }

    // Allocate or reallocate memory for the new mail entry
    Mail* temp = realloc(post->mails, (post->mailCount + 1) * sizeof(Mail));
    if (!temp) {
        return ERROR_MEMORY;
    }
    post->mails = temp;

    // Initialize the new mail entry within post->mails
    Mail* newMail = &post->mails[post->mailCount];

    // Deep copy Address details
    if (createAddress(&newMail->recipientAddress,
        mail.recipientAddress.city.data,
        mail.recipientAddress.street.data,
        mail.recipientAddress.houseNumber,
        mail.recipientAddress.building.data,
        mail.recipientAddress.apartment,
        mail.recipientAddress.postalCode.data) != SUCCESS) {
        return ERROR_MEMORY;
    }

    // Copy other Mail fields
    newMail->weight = mail.weight;

    // Deep copy the strings in Mail (parcelID, creationTime, deliveryTime)
    if (create_string(&newMail->parcelID, mail.parcelID.data) != SUCCESS ||
        create_string(&newMail->creationTime, mail.creationTime.data) != SUCCESS ||
        create_string(&newMail->deliveryTime, mail.deliveryTime.data) != SUCCESS) {
        deleteAddress(&newMail->recipientAddress);  // Free the address in case of error
        return ERROR_MEMORY;
    }

    // Increment the mail count after successful addition
    post->mailCount++;
    return SUCCESS;
}




ErrorCode deleteMail(Post* post, const char* parcelID) {
    for (int i = 0; i < post->mailCount; i++) {
        if (strcmp(post->mails[i].parcelID.data, parcelID) == 0) {
            deleteAddress(&post->mails[i].recipientAddress);
            delete_string(&post->mails[i].parcelID);
            delete_string(&post->mails[i].creationTime);
            delete_string(&post->mails[i].deliveryTime);
            memmove(&post->mails[i], &post->mails[i + 1], (post->mailCount - i - 1) * sizeof(Mail));
            post->mailCount--;
            return SUCCESS;
        }
    }
    return ERROR_INVALID_INPUT;
}

// Поиск объекта Mail по идентификатору
Mail* findMailByID(Post* post, const char* parcelID) {
    for (int i = 0; i < post->mailCount; i++) {
        if (strcmp(post->mails[i].parcelID.data, parcelID) == 0) {
            return &post->mails[i];
        }
    }
    return NULL;
}

int compareMail(const void* a, const void* b) {
    Mail* mailA = (Mail*)a;
    Mail* mailB = (Mail*)b;
    int res = strcmp(mailA->recipientAddress.postalCode.data, mailB->recipientAddress.postalCode.data);
    if (res == 0) {
        return strcmp(mailA->parcelID.data, mailB->parcelID.data);
    }
    return res;
}

void sortMails(Post* post) {
    qsort(post->mails, post->mailCount, sizeof(Mail), compareMail);
}

// Проверка срока доставки на истечение
int isDeliveryExpired(const String* deliveryTime) {
    struct tm deliveryTm = { 0 };
    int day, month, year, hour, minute, second;

    // Разбираем строку формата "dd:MM:yyyy hh:mm:ss"
    if (sscanf(deliveryTime->data, "%2d:%2d:%4d %2d:%2d:%2d",
        &day, &month, &year, &hour, &minute, &second) != 6) {
        printf("Invalid date format\n");
        return 0; // Ошибка в формате даты
    }

    // Заполняем структуру tm
    deliveryTm.tm_mday = day;
    deliveryTm.tm_mon = month - 1;  // Месяцы идут от 0 до 11
    deliveryTm.tm_year = year - 1900; // Года идут от 1900
    deliveryTm.tm_hour = hour;
    deliveryTm.tm_min = minute;
    deliveryTm.tm_sec = second;

    // Конвертируем в time_t
    time_t delivery = mktime(&deliveryTm);
    if (delivery == -1) {
        printf("Error in date conversion\n");
        return 0;
    }

    // Сравниваем с текущим временем
    return time(NULL) > delivery;
}

// Вывод всех доставленных и истекших отправлений
void printDeliveredMails(const Post* post) {
    printf("Delivered mails:\n");
    for (int i = 0; i < post->mailCount; i++) {
        Mail* mail = &post->mails[i];
        if (isDeliveryExpired(&mail->deliveryTime)) {
            printf("Parcel ID: %s, Created: %s, delivered\n", mail->parcelID.data, mail->creationTime.data);
        }
    }
}

void printExpiredMails(const Post* post) {
    printf("Delivered mails:\n");
    for (int i = 0; i < post->mailCount; i++) {
        Mail* mail = &post->mails[i];
        if (!isDeliveryExpired(&mail->deliveryTime)) {
            printf("Parcel ID: %s, Created: %s, expired\n", mail->parcelID.data, mail->creationTime.data);
        }
    }
}

void printMails(const Post* post) {
    printf("Mails sorted by postal code and parcel ID:\n");
    for (int i = 0; i < post->mailCount; i++) {
        Mail* mail = &post->mails[i];
        printf("Parcel ID: %s, Postal Code: %s\n", mail->parcelID.data, mail->recipientAddress.postalCode.data);
    }
}

int isDigitsOnly(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidPostalCode(const char* postalCode) {
    return strlen(postalCode) == 6 && isDigitsOnly(postalCode);
}

int isValidParcelID(const char* parcelID) {
    return strlen(parcelID) == 14 && isDigitsOnly(parcelID);
}

int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12 || day < 1) {
        return 0;
    }

    if (month == 2) { 
        int maxDays = isLeapYear(year) ? 29 : 28;
        return day <= maxDays;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11) { 
        return day <= 30;
    }

    return day <= 31;
}

int isValidTimeFormat(const char* timeStr) {
    int day, month, year, hour, minute, second;
    if (sscanf(timeStr, "%2d:%2d:%4d %2d:%2d:%2d", &day, &month, &year, &hour, &minute, &second) != 6) {
        return 0;
    }

    return isValidDate(day, month, year) &&
        hour >= 0 && hour < 24 &&
        minute >= 0 && minute < 60 &&
        second >= 0 && second < 60;
}


void print_menu() {
    printf("\nMenu:\n");
    printf("1. Add mail\n2. Delete mail\n3. Find mail by ID\n");
    printf("4. View delivered mails\n5. View expired mails\n");
    printf("0. Exit\n");
}

void mainMenu(Post* post) {
    int choice;
    char parcelID[15];
    do {
        print_menu();
        printf("Choose an option: ");
        while (scanf("%d", &choice) != 1) {  
            printf("Invalid choice, please enter a number.\n");
            while (getchar() != '\n');  
            printf("Choose an option: ");
        }
        switch (choice) {
        case 1: {
            Mail mail;
            char city[100], street[100], building[100], postalCode[7];
            int houseNumber, apartment;

            printf("Enter city: ");
            getchar();
            fgets(city, sizeof(city), stdin);
            city[strcspn(city, "\n")] = '\0';
            if (strlen(city) == 0) {
                printf("Invalid input: city cannot be empty.\n");
                break;
            }

            printf("Enter street: ");
            fgets(street, sizeof(street), stdin);
            street[strcspn(street, "\n")] = '\0';
            if (strlen(street) == 0) {
                printf("Invalid input: street cannot be empty.\n");
                break;
            }

            printf("Enter house number: ");
            if (scanf("%d", &houseNumber) != 1 || houseNumber <= 0) {
                printf("Invalid input: house number must be a positive integer.\n");
                getchar();
                break;
            }
            getchar();

            printf("Enter building: ");
            fgets(building, sizeof(building), stdin);
            building[strcspn(building, "\n")] = '\0';

            printf("Enter apartment number: ");
            if (scanf("%d", &apartment) != 1 || apartment <= 0) {
                printf("Invalid input: apartment number must be a positive integer.\n");
                getchar();
                break;
            }
            getchar();

            printf("Enter postal code (6 digits): ");
            scanf("%6s", postalCode);
            getchar();
            if (!isValidPostalCode(postalCode)) {
                printf("Invalid input: postal code must be 6 digits.\n");
                break;
            }

            if (createAddress(&mail.recipientAddress, city, street, houseNumber, building, apartment, postalCode) != SUCCESS) {
                printf("Invalid memory\n");
                break;
            }

            char parcelIDInput[15], creationTime[20], deliveryTime[20];
            float weight;

            printf("Enter parcel ID (14 digits): ");
            scanf("%14s", parcelIDInput);
            getchar();
            if (!isValidParcelID(parcelIDInput)) {
                printf("Invalid input: parcel ID must be 14 digits.\n");
                break;
            }

            printf("Enter weight: ");
            if (scanf("%f", &weight) != 1 || weight < 0) {
                printf("Invalid input: weight must be a non-negative number.\n");
                getchar();
                break;
            }
            getchar();

            printf("Enter creation time (dd:MM:yyyy hh:mm:ss): ");
            fgets(creationTime, sizeof(creationTime), stdin);
            creationTime[strcspn(creationTime, "\n")] = '\0';
            if (!isValidTimeFormat(creationTime)) {
                printf("Invalid input: creation time must follow the format dd:MM:yyyy hh:mm:ss.\n");
                break;
            }
            getchar();
            printf("Enter delivery time (dd:MM:yyyy hh:mm:ss): ");
            fgets(deliveryTime, sizeof(deliveryTime), stdin);
            deliveryTime[strcspn(deliveryTime, "\n")] = '\0';
            if (!isValidTimeFormat(deliveryTime)) {
                printf("Invalid input: delivery time must follow the format dd:MM:yyyy hh:mm:ss.\n");
                break;
            }

            mail.weight = weight;
            mail.recipientAddress.apartment = apartment;
            mail.recipientAddress.houseNumber = houseNumber;

            if (create_string(&mail.parcelID, parcelIDInput) != SUCCESS ||
                create_string(&mail.creationTime, creationTime) != SUCCESS ||
                create_string(&mail.deliveryTime, deliveryTime) != SUCCESS) {
                printf("Error creating mail details.\n");
                break;
            }

            if (addMail(post, mail) != SUCCESS) {
                printf("Error adding mail.\n");
                deleteAddress(&mail.recipientAddress);
                delete_string(&mail.parcelID);
                delete_string(&mail.creationTime);
                delete_string(&mail.deliveryTime);
            }
            else {
                sortMails(post);
                printf("Mail added successfully.\n");
            }
            break;
        }
        case 2:
            printf("Enter Parcel ID to delete: ");
            scanf("%14s", parcelID);
            getchar();
            if (!isValidParcelID(parcelID)) {
                printf("Invalid input: parcel ID must be 14 digits.\n");
            }
            else if (deleteMail(post, parcelID) != SUCCESS) {
                printf("Error deleting mail.\n");
            }
            else {
                printf("We delete mail with ID: %s\n", parcelID);
            }
            break;

        case 3: {
            printf("Enter Parcel ID to find: ");
            scanf("%14s", parcelID);
            getchar();
            if (!isValidParcelID(parcelID)) {
                printf("Invalid input: parcel ID must be 14 digits.\n");
            }
            else {
                Mail* mail = findMailByID(post, parcelID);
                if (mail) {
                    printf("Parcel ID: %s, Weight: %.2f\n", mail->parcelID.data, mail->weight);
                }
                else {
                    printf("Mail not found.\n");
                }
            }
            break;
        }
        case 4:
            printDeliveredMails(post);
            break;

        case 5:
            printExpiredMails(post);
            break;

        case 0:
            printf("Finish program\n");
            break;

        default:
            printf("Invalid choice, try again.\n");
        }
    } while (choice != 0);
}


int main() {
    // Пункт 1
    String str1, str2, str3 = { NULL, 0 };
    String* strCopy;
    ErrorCode errCode;

    // Создание строки из литерала
    errCode = create_string(&str1, "Hello");
    if (errCode != SUCCESS) {
        printf("Error creating str1\n");
        return 1;
    }
    printf("str1: %s\n", str1.data);

    // Создание второй строки
    errCode = create_string(&str2, "World");
    if (errCode != SUCCESS) {
        printf("Error creating str2\n");
        delete_string(&str1);
        return 1;
    }
    printf("str2: %s\n", str2.data);

    // Сравнение строк
    int comparison = compare_strings(&str1, &str2);
    printf("Comparison between str1 and str2: %d\n", comparison);

    // Копирование содержимого str1 в str3
    errCode = copyToExistingString(&str3, &str1);
    if (errCode != SUCCESS) {
        printf("Error copying str1 to str3\n");
    }
    else {
        printf("str3 (copy of str1): %s\n", str3.data);
    }


    // Копирование содержимого str1 в новый экземпляр String
    errCode = copyToNewString(&strCopy, &str1);
    if (errCode != SUCCESS) {
        printf("Error creating a new copy of str1\n");
    }
    else {
        printf("strCopy (new copy of str1): %s\n", strCopy->data);
    }

    // Конкатенация str1 и str2
    errCode = concatenate_strings(&str1, &str2);
    if (errCode != SUCCESS) {
        printf("Error concatenating str1 and str2\n");
    }
    else {
        printf("str1 after concatenation with str2: %s\n", str1.data);
    }

    // Проверка эквивалентности строк
    int isEqual = areStringsEqual(&str1, &str3);
    printf("Are str1 and str3 equal? %s\n", isEqual ? "Yes" : "No");

    // Очистка памяти для строк
    delete_string(&str1);
    delete_string(&str2);
    delete_string(&str3);
    delete_string(strCopy);
    free(strCopy);

    //2 пункт
    Post post = { NULL, NULL, 0 };

    // Добавление тестовых данных для проверки сортировки
    Mail mail1, mail2, mail3;
    createAddress(&mail1.recipientAddress, "CityA", "StreetA", 1, "Bldg1", 101, "123456");
    create_string(&mail1.parcelID, "00000000000001");
    create_string(&mail1.creationTime, "01:01:2023 10:00:00");
    create_string(&mail1.deliveryTime, "02:01:2023 10:00:00");
    mail1.weight = 2.5;

    createAddress(&mail2.recipientAddress, "CityB", "StreetB", 2, "Bldg2", 202, "654321");
    create_string(&mail2.parcelID, "00000000000002");
    create_string(&mail2.creationTime, "01:02:2023 11:00:00");
    create_string(&mail2.deliveryTime, "02:11:2024 11:00:00");
    mail2.weight = 1.5;

    createAddress(&mail3.recipientAddress, "CityA", "StreetA", 1, "Bldg1", 102, "123456");
    create_string(&mail3.parcelID, "00000000000003");
    create_string(&mail3.creationTime, "01:03:2023 12:00:00");
    create_string(&mail3.deliveryTime, "04:11:2024 12:00:00");
    mail3.weight = 3.0;

    addMail(&post, mail1);
    addMail(&post, mail2);
    addMail(&post, mail3);

    // Вывод перед сортировкой
    printf("Before sorting:\n");
    printMails(&post);

    // Сортировка
    sortMails(&post);

    // Вывод после сортировки
    printf("After sorting:\n");
    printMails(&post);

    // Запуск основного меню
    mainMenu(&post);

    // Очистка памяти
    for (int i = 0; i < post.mailCount; i++) {
        deleteAddress(&post.mails[i].recipientAddress);
        delete_string(&post.mails[i].parcelID);
        delete_string(&post.mails[i].creationTime);
        delete_string(&post.mails[i].deliveryTime);
    }
    free(post.mails);
    return 0;
}

