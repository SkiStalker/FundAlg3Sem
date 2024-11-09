#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum ret_t{
    MEMORY_ERROR = 2,
    FILE_ERROR
};

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


typedef struct Node
{
    char letter;
    struct Node** next_nodes;
    int _size;
    int len;
}Node;

int Node_init(Node** node, char letter){
    *node = (Node*)malloc(sizeof(Node));
    if(*node == NULL){
        return MEMORY_ERROR;
    }
    (*node)->_size = 2;
    (*node)->len = 0;
    (*node)->letter = letter;
    (*node)->next_nodes = (Node**)malloc(sizeof(Node*) * (*node)->_size);
    if((*node)->next_nodes == NULL){
        (*node)->len = 0;
        (*node)->_size = 0;
        return MEMORY_ERROR;
    }
    for (int i = (*node)->len; i < (*node)->_size; i++)
    {
        (*node)->next_nodes[i] = NULL;
    }
    return 1;
}

int Node_delete(Node** node){ 
    for (int i = 0; i < (*node)->len; i++)
    {
        if((*node)->next_nodes[i] != NULL){
            Node_delete(&((*node)->next_nodes[i]));
        }
    }
    free((*node)->next_nodes);
    free((*node));
    return 1;
}

int Node_realloc(Node** node){ 
    if((*node)->next_nodes == NULL || (*node) == NULL){
        return MEMORY_ERROR;
    }
    (*node)->_size *= 2;
    Node** tmp = (Node**)realloc((*node)->next_nodes, (*node)->_size * sizeof(Node*));
    if(tmp == NULL){
        Node_delete(node); //мб ошибка
        return MEMORY_ERROR;
    }
    (*node)->next_nodes = tmp;
    for (int i = (*node)->len; i < (*node)->_size; i++)
    {
        (*node)->next_nodes[i] = NULL;
    }
    
    return 1;
}

int Node_add_Node(Node** head, Node** node){
    if((*head) == NULL || (*head)->next_nodes == NULL || (*node) == NULL || (*node)->next_nodes == NULL){
        return MEMORY_ERROR;
    }
    if((*head)->len == (*head)->_size){
        int ret = Node_realloc(head);
        if(ret == MEMORY_ERROR){
            return MEMORY_ERROR;
        }
    }
    (*head)->next_nodes[(*head)->len++] = (*node);
    return 1;
}

int Node_add(Node** head, char letter){
    if((*head) == NULL || (*head)->next_nodes == NULL){
        return MEMORY_ERROR;
    }
    if((*head)->len == (*head)->_size){
        int ret = Node_realloc(head);
        if(ret == MEMORY_ERROR){
            return MEMORY_ERROR;
        }
    }
    Node* node;
    Node_init(&node, letter);
    (*head)->next_nodes[(*head)->len++] = node;
    return 1;
}

int get_NodeV(char* str, Node** node, int* counter){
    int flag_s = 0; 
    for (int i = *counter; i < (int)strlen(str); i++)
    {
        (*counter)++;
        if(isalpha(str[i]) && (*node)->letter == ' ' && !flag_s){
            (*node)->letter = str[i];
        }else if(isalpha(str[i]) && (*node)->letter != ' '){
            Node_add(node, str[i]);
        }
        else if(str[i] == '(' && (*node)->len == 0){
            flag_s = 1;
        }
        else if(str[i] == '('){
            if((*node)->next_nodes[(*node)->len - 1] == NULL){
                Node_add(node, ' ');
                (*node)->len--;
            }
            get_NodeV(str, &(*node)->next_nodes[(*node)->len - 1], counter);
            i = *(counter) - 1;
        }
        else if(str[i] == ')'){
            return 1;
        }
    }
    return 0;
}

int LrootR(Node** node, int lvl, FILE* descr){
    String s;
    String_init(&s, "");
    for (int i = 0; i < lvl; i++)
    {
        String_concat_ch(&s, '\t');
    }
    String_concat_ch(&s, (*node)->letter);
    String_concat_ch(&s, '\n');
    fputs(s.data, descr);
    for (int i = 0; i < (*node)->len; i++)
    {
        if((*node)->next_nodes[i] != NULL){
            LrootR(&(*node)->next_nodes[i], lvl + 1, descr);
        }
    }
    String_delete(&s);
    return 0;
}

int parse_and_print(char* inFile, char* outFile){
    FILE* fi, *fo;
    fi = fopen(inFile, "r");
    if(fi == NULL){
        return FILE_ERROR;
    }
    fo = fopen(outFile, "w");
    if(fo == NULL){
        fclose(fi);
        return FILE_ERROR;
    }

    char c;
    Node* head;
    Node_init(&head, ' ');
    String s;
    String_init(&s, "");
    do
    {
        c = fgetc(fi);
        if(c != '\n' && c!= EOF){
            String_concat_ch(&s, c);
        }
        else{
            int counter = 0, lvl= 0;
            get_NodeV(s.data, &head, &counter);
            LrootR(&head, lvl, fo);
            String_assignment(&s, "");
            Node_delete(&head);
            Node_init(&head, ' ');
        }

    } while (c != EOF);
    String_delete(&s);
    fclose(fi);
    fclose(fo);
    return 1;
}


/* int main(){
    parse_and_print("test.txt", "qwe.txt");
} */
int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Wring amount of arguments!\nRight example: ./<command> <inFile> <outFile>\n");
        return -1;
    }
    char* inFile, *outFile;
    inFile = (char*)malloc(sizeof(char) * (strlen(argv[1] + 1)));
    outFile = (char*)malloc(sizeof(char) * (strlen(argv[2] + 1)));
    strcpy(inFile, argv[1]);
    strcpy(outFile, argv[2]);
    inFile[strlen(argv[1])] = '\0';
    outFile[strlen(argv[2])] = '\0';

    parse_and_print(inFile, outFile);

    free(inFile);
    free(outFile);
    return 0;
}