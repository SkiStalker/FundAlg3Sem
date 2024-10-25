#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

enum ret_t{
    SUCCESS,
    MEMORY_ERROR
};

typedef int (*callback)(char*);

void error_handler(int ret){
    switch (ret)
    {
    case MEMORY_ERROR:
        printf("Memory error!\n");
        break;
    
    default:
        break;
    }
}

int l_strlen(char* string){
    int counter = 0;
    for (int i = 0; string[i] != '\0'; i++)
    {
        counter++;
    }
    return counter;
}

int mystrcpy(char* to, char* from){
    int i = 0;
    do
    {

        to[i] = from[i];
        i++;
    } while (from[i - 1] != '\0');
    return 1;
}

char* more_size(char* array, int* memo){
    *memo *= 2;
    char* n_array = (char*)realloc(array, *memo);
    if(n_array == NULL){
        *memo = 0;
        free(array);
        return NULL;
    }
    else{
        array = n_array;
        return array;
    }
}

int n_order(char* string){
    int len = l_strlen(string);
    //char n_string[BUFSIZ];
    int memo = 4;
    char* n_string = (char*)malloc(sizeof(char) * memo);
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if(isdigit(string[i])){
            n_string[j++] = string[i];
            if(j == memo){
                n_string = more_size(n_string, &memo);
                if(memo == 0){
                    return MEMORY_ERROR;
                }
            }
        }
    }
    for (int i = 0; i < len; i++)
    {
        if(isalpha(string[i])){
            n_string[j++] = string[i];
            if(j == memo){
                n_string = more_size(n_string, &memo);
                if(memo == 0){
                    return MEMORY_ERROR;
                }
            }
        }
    }
    for (int i = 0; i < len; i++)
    {
        if(!isalnum(string[i])){
            n_string[j++] = string[i];
            if(j == memo){
                n_string = more_size(n_string, &memo);
                if(memo == 0){
                    return MEMORY_ERROR;
                }
            }
        }
    }
    n_string[j] = '\0';
    mystrcpy(string, n_string);
    free(n_string);
}

int has_only_numbers(char* string){
    for (int i = 0; string[i] != '\0'; i++)
    {
        if(!isdigit(string[i])){
            return 0;
        }
    }
    return 1;
}

int check(int* used, int len){
    for (int i = 0; i < len; i++)
    {
        if(!used[i]){
            return 0;
        }
    }
    return 1;
}

int my_strcat(char* str1, char* str2){
    int end = l_strlen(str1);
    for (int i = 0; str2[i] != '\0'; i++)
    {
        str1[end++] = str2[i];
    }
    str1[end] = '\0';
}

char* c_concat_rand(char* argv[]){
    char* ret_str;
    int len = l_strlen(argv[2]);
    int cnt = 1;
    
    for (int i = 4; argv[i] != NULL; i++)
    {
        len += l_strlen(argv[i]);
        ++cnt;
    }
    int* used = (int*)calloc(cnt, sizeof(char));
    ret_str = (char*)malloc(sizeof(char) * (len + 1));
    ret_str[0] = '\0';
    if(has_only_numbers(argv[3])){
        int seed = atoi(argv[3]);
        srand(seed);
        while (!check(used, cnt))
        {
            int rnd = rand() % cnt;
            if(used[rnd]){
                continue;
            }
            used[rnd] = 1;
            my_strcat(ret_str, argv[rnd >= 1? rnd + 3: 2]);
        }
        return ret_str;
        
    }
    else{
        mystrcpy(ret_str, "Seed error!\n\0");
        return ret_str;
    }
    mystrcpy(ret_str, "Error!\n\0");
    return ret_str;
}


int u_toupper(char* string){
    for (int i = 0; string[i] != '\0'; i++)
    {
        if(i % 2 == 1 && !isupper(string[i]) && isalpha(string[i])){
            string[i] = string[i] - ('a' - 'A');
        }
    }
    return SUCCESS;
}

int r_reverse(char* string){
    //char rev[BUFSIZ];
    int len = l_strlen(string); 
    char* rev = (char*)malloc(sizeof(char) * (len + 1));
    if(rev == NULL){
        return MEMORY_ERROR;
    }
    rev[len] = '\0';
    for (int i = len - 1, j = 0; i >= 0; i--, j++)
    {
        rev[i] = string[j];
    }
    mystrcpy(string, rev);
    free(rev);
    return SUCCESS;
}



int my_strcmp(char* str1, char* str2){
    for (int i = 0; str1[i] != '\0' || str2[i] != '\0'; i++)
    {
        if(str1[i] != str2[i]){
            return 1;
        }
    }
    return 0;
}

int find_flag(char flag, char* flags, int len){ //тут мб ошибка
    for (int i = 0; i < len; i++)
    {
        if(flag == flags[i]){
            return i;
        }
    }
    return -1;
}


int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Wrong amount of arguments!\n");
        return -1;
    }
    else if(l_strlen(argv[1]) != 2 || argv[1][0] != '-'){
        printf("Unknown flag %s!", argv[1]);
        return -1;
    }
        
    
    int ret = SUCCESS;
    switch (argv[1][1])
    {
    case 'l':
        {
            printf("%d\n", l_strlen(argv[2]));
        }
        break;
    case 'r':
        {
            ret = r_reverse(argv[2]);
            printf("%s\n", argv[2]);
        }
        break;
    case 'u':
        {
            ret = u_toupper(argv[2]);
            printf("%s\n", argv[2]);
        }
        break;
    case 'n':
        {
            ret = n_order(argv[2]);
            printf("%s\n", argv[2]);
        }
        break;
    case 'c':
        {
            if(argc < 5){
                printf("The number of arguments must not be less than 4!\n");
                return -1;
            }
            char* res = c_concat_rand(argv);
            printf("%s\n", res);
            free(res);
        }
        break;
    default:
        printf("Unknown key!\n");
        return -1;
        break;
    }
    error_handler(ret);
    return 0;
    
}