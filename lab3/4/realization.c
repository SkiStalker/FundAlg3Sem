#include "functions.h"



return_code string_to_ulong(const char *str, unsigned long *result)
{
    char *end_ptr;
    *result = strtoul(str, &end_ptr, 10);
    if (*result == ULONG_MAX)
        return INVALID_INPUT;
    if (*end_ptr != '\0')
        return INVALID_INPUT;
    return OK;
}

return_code string_to_double(const char *str, double *result)
{
    char *endptr;
    *result = strtod(str, &endptr);
    if (*result == HUGE_VAL || *result == -HUGE_VAL || *endptr != '\0')
        return INVALID_INPUT;
    return OK;
}

return_code post_init(Post *post)
{
    post->address_ptr = (Address*)malloc(sizeof(Address));
    if (!post->address_ptr)
        return MEMORY_ALLOCATION_ERROR;
    post->mail_capacity = 2;
    post->mail_arr = (Mail*)malloc(sizeof(Mail) * post->mail_capacity);
    if (!post->mail_arr)
    {
        free(post->address_ptr);
        return MEMORY_ALLOCATION_ERROR;
    }

    return OK;
}

void clear_mail(Mail *mail)
{
    clear_address(&mail->address);
    string_clear(&mail->create_time);
    string_clear(&mail->postal_index);
    string_clear(&mail->getting_time);
}

void get_string_with_space(char *str, int max_size)
{
    char c;
    int size = 0;
    while (isspace(c = getchar()))
        ;
    str[size++] = c;
    while ((c = getchar()) != '\n')
    {
        str[size++] = c;
        if (size == max_size - 1)
            break;
    }
    str[size] = '\0';
}

return_code get_address(Address *address)
{
    if (!address)
        return MEMORY_ALLOCATION_ERROR;

    int i;
    unsigned long _apartment_number, _house_number;
    char house_number[12], city[128], street[128], building[128], apartment_number[128], index[8];
    printf("Enter city:\n ");
    get_string_with_space(city, 128);
    printf("Enter street:\n ");
    get_string_with_space(street, 128);
    printf("Enter house number:\n ");
    scanf("%11s", house_number);
    printf("Enter building:\n ");
    scanf("%127s", building);
    printf("Enter apartment_number:\n ");
    scanf("%127s", apartment_number);
    printf("Enter index(digit format: xxxxxx):\n ");
    scanf("%7s", index);

    for (i = 0; i < 6; ++i)
    {
        if (!isdigit(index[i]))
            return INVALID_INPUT;
    }
    if (index[6] != '\0')
    {
        while (getchar() != '\n')
            ;
        return INVALID_INPUT;
    }

    if (string_to_ulong(house_number, &_house_number))
        return INVALID_INPUT;
    if (string_to_ulong(apartment_number, &_apartment_number))
        return INVALID_INPUT;
    address->apartment_number = _apartment_number;
    address->house_number = (unsigned int)_house_number;
    address->street = string_create(street);
    address->building = string_create(building);
    address->city = string_create(city);
    address->index = string_create(index);
    if (!address->street.arr ||
        !address->building.arr ||
        !address->city.arr ||
        !address->index.arr)
        return MEMORY_ALLOCATION_ERROR;

    return OK;
}

return_code get_weight(double *weight)
{
    char _weight[128];
    if (!weight)
        return MEMORY_ALLOCATION_ERROR;

    printf("Enter mail weight: ");
    scanf("%127s", _weight);
    return string_to_double(_weight, weight);
}

return_code get_postal_index(String *str)
{
    char postal_index[16];
    int i;
    if (!str)
        return MEMORY_ALLOCATION_ERROR;
    str->arr = NULL;
    str->size = 0;
    printf("Enter postal index(14 digits): ");
    scanf("%15s", postal_index);
    if (postal_index[14] != '\0')
        return INVALID_INPUT;
    for (i = 0; i < 14; ++i)
    {
        if (!isdigit(postal_index[i]))
            return INVALID_INPUT;
    }
    *str = string_create(postal_index);
    if (str->size == -1)
        return MEMORY_ALLOCATION_ERROR;
    return OK;
}

return_code validate_date(const char *date)
{
    if (date[10] != '\0' || date[2] != ':' || date[5] != ':' || !isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[3]) || !isdigit(date[4]) || !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9]))
        return INVALID_INPUT;

    int day, month, year;
    day = atoi(date);
    month = atoi(date + 3);
    year = atoi(date + 6);
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return INVALID_INPUT;

    if (month == 2 && day > 28)
        return INVALID_INPUT;

    if (day > 31)
        return INVALID_INPUT;

    if (month > 12 || day == 0 || year < 1990 || year > 2025)
        return INVALID_INPUT;

    return OK;
}

return_code validate_time(const char *time)
{
    if (time[8] != '\0' || time[2] != ':' || time[5] != ':' || !isdigit(time[0]) || !isdigit(time[1]) || !isdigit(time[3]) || !isdigit(time[4]) || !isdigit(time[6]) || !isdigit(time[7]))
        return INVALID_INPUT;

    int h, m, s;
    h = atoi(time);
    m = atoi(time + 3);
    s = atoi(time + 6);
    if (h > 23 || m > 59 || s > 59)
    {
        return INVALID_INPUT;
    }
    return OK;
}

return_code get_time(String *str)
{
    char date[24], time[10];
    if (!str)
        return MEMORY_ALLOCATION_ERROR;

    scanf("%s", date);
    scanf("%s", time);
    if (validate_time(time) || validate_date(date))
        return INVALID_INPUT;
    strcat(date, " ");
    strcat(date, time);
    *str = string_create(date);
    if (str->size == -1)
        return MEMORY_ALLOCATION_ERROR;
    return OK;
}

void clear_address(Address *address)
{
    string_clear(&address->building);
    string_clear(&address->city);
    string_clear(&address->index);
    string_clear(&address->street);
}

return_code add_mail(Post *post, int *count_mails)
{
    Mail *mail_realloc;
    return_code state;
    if ((unsigned long int)*count_mails == post->mail_capacity)
    {
        post->mail_capacity *= 2;
        mail_realloc = (Mail *)realloc(post->mail_arr, post->mail_capacity * sizeof(Mail));
        if (!mail_realloc)
            return MEMORY_ALLOCATION_ERROR;
        post->mail_arr = mail_realloc;
    }
    if ((state = get_address(&post->mail_arr[*count_mails].address)) != OK)
        return state;
    if ((state = get_weight(&post->mail_arr[*count_mails].weight)) != OK)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        return state;
    }
    if ((state = get_postal_index(&post->mail_arr[*count_mails].postal_index)) != OK)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        return state;
    }
    printf("Enter create time(format: dd:MM:yyyy hh:mm:ss):\n ");
    if ((state = get_time(&post->mail_arr[*count_mails].create_time)) != OK)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        string_clear(&post->mail_arr[*count_mails].postal_index);
        return state;
    }
    printf("Enter getting time(format: dd:MM:yyyy hh:mm:ss):\n ");
    if ((state = get_time(&post->mail_arr[*count_mails].getting_time)) != OK)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        string_clear(&post->mail_arr[*count_mails].postal_index);
        string_clear(&post->mail_arr[*count_mails].create_time);
        return state;
    }
    if (compare_dates(post->mail_arr[*count_mails].getting_time.arr, post->mail_arr[*count_mails].create_time.arr) <= 0)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        string_clear(&post->mail_arr[*count_mails].postal_index);
        string_clear(&post->mail_arr[*count_mails].create_time);
        string_clear(&post->mail_arr[*count_mails].getting_time);
        return INVALID_INPUT;
    }
    post->mail_arr[*count_mails].delivered = 'n';

    (*count_mails)++;
    return OK;
}

return_code delete_mail(Post *post, int *count_mails)
{
    if (!post || !post->mail_arr)
        return MEMORY_ALLOCATION_ERROR;

    return_code state;

    String postal_index;
    if ((state = get_postal_index(&postal_index)) != OK)
        return state;

    for (int i = 0; i < *count_mails; ++i)
    {
        if (string_equals(&post->mail_arr[i].postal_index, &postal_index))
        {
            clear_mail(&post->mail_arr[i]);
            for (int j = i; j < (*count_mails - 1); ++j)
            {
                post->mail_arr[i] = post->mail_arr[i + 1];
            }
            (*count_mails)--;
            string_clear(&postal_index);
            return OK;
        }
    }
    string_clear(&postal_index);
    return INVALID_INPUT;
}

void print_address(Address *address)
{
    printf("City: %s\n", address->city.arr);
    printf("Street: %s\n", address->street.arr);
    printf("House number: %u\n", address->house_number);
    printf("Building: %s\n", address->building.arr);
    printf("Apartment number: %u\n", address->apartment_number);
    printf("Index: %s\n", address->index.arr);
}

void print_mail_info(Mail *mail)
{
    print_address(&mail->address);
    printf("Create time: %s; getting_time: %s\n", mail->create_time.arr, mail->getting_time.arr);
    printf("Postal index: %s\n", mail->postal_index.arr);
    printf("Weight: %lf\n", mail->weight);
}

return_code search_mail(Post *post, int *count_mails)
{
    if (!post || !post->mail_arr)
        return MEMORY_ALLOCATION_ERROR;

    return_code state;

    String postal_index;
    if ((state = get_postal_index(&postal_index)) != OK)
        return state;

    for (int i = 0; i < *count_mails; ++i)
    {
        if (string_equals(&post->mail_arr[i].postal_index, &postal_index))
        {
            print_mail_info(&post->mail_arr[i]);
            string_clear(&postal_index);
            return OK;
        }
    }
    string_clear(&postal_index);
    return INVALID_INPUT;
}


int compare_mails(const void *a, const void *b)
{
    const Mail *mail_1 = (const Mail *)a;
    const Mail *mail_2 = (const Mail *)b;
    int state;
    if ((state = string_compare(&mail_1->address.index, &mail_2->address.index)) != 0)
    {
        return state;
    }

    return string_compare(&mail_1->postal_index, &mail_2->postal_index);
}

return_code set_delivered(Post *post, int *count_mails)
{
    if (!post || !post->mail_arr)
        return MEMORY_ALLOCATION_ERROR;

    return_code state;

    String postal_index;
    if ((state = get_postal_index(&postal_index)) != OK)
    {
        string_clear(&postal_index);
        return state;
    }

    for (int i = 0; i < *count_mails; ++i)
    {
        if (string_equals(&post->mail_arr[i].postal_index, &postal_index))
        {
            post->mail_arr[i].delivered = 'y';
            string_clear(&postal_index);
            return OK;
        }
    }
    string_clear(&postal_index);
    return INVALID_INPUT;
}

int time_compare_mails(const void *a, const void *b)
{
    const Mail *mail_1 = (const Mail *)a;
    const Mail *mail_2 = (const Mail *)b;

    String time_1, time_2;
    int param_1, param_2;
    time_1 = mail_1->create_time;
    time_2 = mail_2->create_time;

    param_1 = atoi(time_1.arr + 6);
    param_2 = atoi(time_2.arr + 6);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1.arr + 3);
    param_2 = atoi(time_2.arr + 3);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1.arr);
    param_2 = atoi(time_2.arr);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }
    return strcmp(time_1.arr + 11, time_2.arr + 11);
}

int compare_dates(const char *time_1, const char *time_2)
{
    int param_1, param_2;
    param_1 = atoi(time_1 + 6);
    param_2 = atoi(time_2 + 6);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1 + 3);
    param_2 = atoi(time_2 + 3);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1);
    param_2 = atoi(time_2);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }
    return strcmp(time_1 + 11, time_2 + 11);
}

return_code search_delivered(Post *post, int count_mails, int *count_delivered, Mail **arr_delivered)
{
    if (!post || !post->mail_arr || !arr_delivered)
        return MEMORY_ALLOCATION_ERROR;

    int capacity = 2;
    *count_delivered = 0;
    *arr_delivered = NULL;
    Mail *delivered = (Mail *)malloc(sizeof(Mail) * capacity), *temp_realloc;
    if (!delivered)
        return MEMORY_ALLOCATION_ERROR;

    for (int i = 0; i < count_mails; ++i)
    {
        if (post->mail_arr[i].delivered == 'y')
        {
            if (*count_delivered == capacity)
            {
                capacity *= 2;
                temp_realloc = (Mail *)realloc(delivered, sizeof(Mail) * capacity);
                if (!temp_realloc)
                {
                    free(delivered);
                    return MEMORY_ALLOCATION_ERROR;
                }
                delivered = temp_realloc;
            }

            delivered[(*count_delivered)++] = post->mail_arr[i];
        }
    }
    qsort(delivered, *count_delivered, sizeof(Mail), time_compare_mails);
    *arr_delivered = delivered;
    return OK;
}

return_code search_overdue(Post *post, int count_mails, int *count_overdue, Mail **arr_overdue)
{
    if (!post || !post->mail_arr || !arr_overdue)
        return MEMORY_ALLOCATION_ERROR;

    int capacity = 2;
    *count_overdue = 0;
    *arr_overdue = NULL;
    Mail *overdue = (Mail *)malloc(sizeof(Mail) * capacity), *temp_realloc;
    if (!overdue)
        return MEMORY_ALLOCATION_ERROR;

    time_t _cur_time = time(NULL);
    struct tm *now = localtime(&_cur_time);
    char cur_time[24];
    sprintf(cur_time, "%02d:%02d:%d %02d:%02d:%02d", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec);
    for (int i = 0; i < count_mails; ++i)
    {
        if (post->mail_arr[i].delivered == 'n' && compare_dates(post->mail_arr[i].getting_time.arr, cur_time) < 0)
        {
            if (*count_overdue == capacity)
            {
                capacity *= 2;
                temp_realloc = (Mail *)realloc(overdue, sizeof(Mail) * capacity);
                if (!temp_realloc)
                {
                    free(overdue);
                    return MEMORY_ALLOCATION_ERROR;
                }
                overdue = temp_realloc;
            }

            overdue[(*count_overdue)++] = post->mail_arr[i];
        }
    }
    qsort(overdue, *count_overdue, sizeof(Mail), time_compare_mails);
    *arr_overdue = overdue;
    return OK;
}

void clear_post(Post *post, int count_mails)
{
    clear_address(post->address_ptr);
    free(post->address_ptr);
    for (int i = 0; i < count_mails; ++i)
    {
        clear_mail(&post->mail_arr[i]);
    }
    free(post->mail_arr);
}
void interactive_dialog(Post *post, int *count_mails) {
    int choice;
    return_code state;

    printf("Choose an action:\n");
    printf("1. Add mail\n");
    printf("2. Delete mail\n");
    printf("3. Search mail by postal index\n");
    printf("4. Search delivered mail\n");
    printf("5. Search overdue mail\n");
    printf("6. Print all mails\n");
    printf("0. End dialog\n");

    do {
        printf("- ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: { // Add mail
                state = add_mail(post, count_mails);
                if (state == OK) {
                    printf("Mail added successfully.\n");
                } else if (state == MEMORY_ALLOCATION_ERROR) {
                    printf("Memory allocation error while adding mail.\n");
                } else if (state == INVALID_INPUT) {
                    printf("Invalid input. Please try again.\n");
                }
                qsort(post->mail_arr, *count_mails, sizeof(Mail), compare_mails);
                break;
            }

            case 2: { // Delete mail by postal index
                state = delete_mail(post, count_mails);
                if (state == OK) {
                    printf("Mail deleted successfully.\n");
                } else if (state == MEMORY_ALLOCATION_ERROR) {
                    printf("Memory error while deleting mail.\n");
                } else if (state == INVALID_INPUT) {
                    printf("Mail not found or invalid input.\n");
                }
                break;
            }

            case 3: { // Search mail by postal index
                state = search_mail(post, count_mails);
                if (state == OK) {
                    printf("Mail found. Information displayed.\n");
                } else if (state == MEMORY_ALLOCATION_ERROR) {
                    printf("Memory error during search.\n");
                } else if (state == INVALID_INPUT) {
                    printf("Mail not found.\n");
                }
                break;
            }

            case 4: { // Search for delivered mail
                Mail *delivered_mails = NULL;
                int count_delivered = 0;
                state = search_delivered(post, *count_mails, &count_delivered, &delivered_mails);
                if (state == OK && count_delivered > 0) {
                    printf("Delivered mails:\n");
                    for (int i = 0; i < count_delivered; i++) {
                        print_mail_info(&delivered_mails[i]);
                    }
                    free(delivered_mails);
                } else {
                    printf("No delivered mails found.\n");
                }
                break;
            }

            case 5: { // Search for overdue mail
                Mail *overdue_mails = NULL;
                int count_overdue = 0;
                state = search_overdue(post, *count_mails, &count_overdue, &overdue_mails);
                if (state == OK && count_overdue > 0) {
                    printf("Overdue mails:\n");
                    for (int i = 0; i < count_overdue; i++) {
                        print_mail_info(&overdue_mails[i]);
                    }
                    free(overdue_mails);
                } else {
                    printf("No overdue mails found.\n");
                }
                break;
            }

            case 6: { // Print all mails
                if (*count_mails == 0) {
                    printf("No mails available.\n");
                } else {
                    printf("All mails:\n");
                    for (int i = 0; i < *count_mails; i++) {
                        print_mail_info(&post->mail_arr[i]);
                    }
                }
                break;
            }

            case 0:
                printf("Ending dialog.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);

    clear_post(post, *count_mails);
}
