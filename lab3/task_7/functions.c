#include "functions.h"

void print_menu() {
    puts("Select an action:");
    puts("1) Find liver;\n2) Add liver;\n3) Delete liver;\n4) Change liver;\n5) Upload to file;\n6) Undo;\n7) End dialog;\n");
    printf("Your input: ");
}

void free_buf() {
    while (getchar() != '\n');
}

enum return_code get_liver(Liver *liver) {
    size_t count = 0;
    if (!liver)
        return MEMORY_ERROR;

    printf("Enter surname: ");
    count += scanf("%s", liver->surname);
    printf("Enter name: ");
    count += scanf("%s", liver->name);
    printf("Enter patronymic(if no enter '-'): ");
    count += scanf("%s", liver->patronymic);
    if (liver->patronymic[0] == '-')
        liver->patronymic[0] = '\0';
    printf("Enter birth date(dd.mm.yyyy): ");
    count += scanf("%d.%d.%d", &liver->birth_date.day, &liver->birth_date.month, &liver->birth_date.year);
    getchar();
    printf("Enter sex(M/W): ");
    count += scanf("%c", &liver->sex);
    printf("Enter income: ");
    count += scanf("%lf", &liver->income);

    if (count != 8 || liver_validate(liver))
        return BAD_INPUT_ERROR;
    return OK;
}

enum return_code stack_init(Stack *stack) {
    if (!stack)
        return MEMORY_ERROR;
    stack->head = NULL;
    stack->size = 0;
    return OK;
}

enum return_code stack_push(Stack *stack, char c) {
    SNode *new_node;
    if (!stack)
        return MEMORY_ERROR;

    new_node = (SNode *)malloc(sizeof(SNode));
    if (!new_node)
        return MEMORY_ERROR;

    new_node->c = c;
    new_node->next = stack->head;
    stack->head = new_node;
    stack->size++;
    return OK;
}

enum return_code stack_pop(Stack *stack, char *top) {
    SNode *temp;
    if (!stack || !top)
        return MEMORY_ERROR;

    if (!stack->size)
        return MEMORY_ERROR;

    *top = stack->head->c;
    temp = stack->head;
    stack->head = temp->next;
    free(temp);
    stack->size--;
    return OK;
}

enum return_code stack_destroy(Stack *stack) {
    SNode *temp, *next;
    if (!stack)
        return MEMORY_ERROR;

    temp = stack->head;
    while (temp) {
        next = temp->next;
        free(temp);
        temp = next;
    }
    stack->head = NULL;
    stack->size = 0;
    return OK;
}