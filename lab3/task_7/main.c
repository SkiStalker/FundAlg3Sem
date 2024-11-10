#include "functions.h"
#include "unidirect_list.h"

int main(int argc, char **argv) {
    UniList list, list_new_data, list_old_data;
    Stack stack_modifications;
    FILE *input;
    enum return_code state;
    size_t count_modifications = 0;

    char user_ans;
    if (argc != 2) {
        puts("Input error. Enter (program_name) (input_file)");
        return BAD_INPUT_ERROR;
    }

    list_init(&list);
    list_init(&list_new_data);
    list_init(&list_old_data);
    stack_init(&stack_modifications);

    input = fopen(argv[1], "r");
    if (!input)
        return FILE_ERROR;
    state = list_file_scan(&list, input);
    fclose(input);
    if (state == BAD_INPUT_ERROR) {
        puts("Incorrect data in the file\n");
        return BAD_INPUT_ERROR;
    } else if (state == MEMORY_ERROR) {
        puts("Memory error\n");
        return MEMORY_ERROR;
    } else
        puts("The file was OK and fully read\n");

    print_menu();
    while ((user_ans = getchar()) != '0') {
        if (user_ans == '\n')
            continue;
        switch (user_ans) {
            case '1': {
                Liver *found, search;
                if (get_liver(&search) == BAD_INPUT_ERROR) {
                    printf("Get liver: input error! Try again\n");
                    break;
                }
                if ((state = list_find(&list, &search, &found)) == MEMORY_ERROR) {
                    printf("List find: memory error\n");
                    break;
                } else if (state == MISSING_ERROR) {
                    printf("\nLiver is missing\n");
                } else {
                    printf("\nLiver found\n");
                }
                break;
            }
            case '2': {
                Liver new_liver;
                puts("Enter liver info:");
                if (get_liver(&new_liver) == BAD_INPUT_ERROR) {
                    printf("Get liver: input error! Try again\n");
                    break;
                }
                if (list_insert(&list, &new_liver) == MEMORY_ERROR) {
                    printf("List_insert: Memory error\n");
                    break;
                }
                printf("OK! New liver added\n");
                if (stack_push(&stack_modifications, '3') == MEMORY_ERROR ||
                    list_push(&list_new_data, &new_liver) == MEMORY_ERROR) {
                    list_destroy(&list_old_data);
                    list_destroy(&list_new_data);
                    list_destroy(&list);
                    stack_destroy(&stack_modifications);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                count_modifications++;
                break;
            }
            case '3': {
                Liver to_delete;
                puts("Enter liver info:");
                if (get_liver(&to_delete) == BAD_INPUT_ERROR) {
                    printf("Get liver: input error! Try again\n");
                    break;
                }
                if (list_delete_node(&list, &to_delete) == MISSING_ERROR) {
                    printf("Liver MISSING_ERROR\n");
                    break;
                }
                printf("OK! Liver deleted\n");
                if (list_push(&list_old_data, &to_delete) ||
                    stack_push(&stack_modifications, '4')) {
                    list_destroy(&list_old_data);
                    list_destroy(&list_new_data);
                    list_destroy(&list);
                    stack_destroy(&stack_modifications);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                count_modifications++;
                break;
            }
            case '4': {
                Liver to_change, new_data;
                puts("Enter liver to change:");
                if (get_liver(&to_change) == BAD_INPUT_ERROR) {
                    printf("Get liver: input error! Try again\n");
                    break;
                }
                puts("Enter new data:");
                if (get_liver(&new_data) == BAD_INPUT_ERROR) {
                    printf("Get liver: input error! Try again\n");
                    break;
                }
                if ((state = liver_change(&list, &to_change, &new_data)) == MISSING_ERROR) {
                    printf("\nLiver is missing\n");
                    break;
                }
                if (state == MEMORY_ERROR) {
                    printf("Liver change: memory error\n");
                    break;
                }
                if (state == OK) {
                    printf("\nSuccess! Liver update\n");
                    if (list_push(&list_old_data, &to_change) == MEMORY_ERROR ||
                        list_push(&list_new_data, &new_data) == MEMORY_ERROR ||
                        stack_push(&stack_modifications, '2') == MEMORY_ERROR) {
                        list_destroy(&list_old_data);
                        list_destroy(&list_new_data);
                        list_destroy(&list);
                        stack_destroy(&stack_modifications);
                        printf("Memory error\n");
                        return MEMORY_ERROR;
                    }
                    count_modifications++;
                }
                break;
            }
            case '5': {
                FILE *output;
                char out_path[BUFSIZ];
                printf("Enter out file: ");
                scanf("%s", out_path);
                output = fopen(out_path, "w");
                if (!output) {
                    printf("File open error\n");
                    break;
                }
                list_file_print(&list, output);
                fclose(output);
                printf("OK! Data uploaded\n");
                break;
            }
            case '6': {
                int i, count_undo;
                char c;
                Liver to_change, new_data;
                printf("Enter count undo: ");
                if (!scanf("%d", &count_undo)) {
                    printf("Input error! Try again\n");
                    break;
                }
                if (count_undo < 0 || (size_t)count_undo > (count_modifications / 2)) {
                    printf("You can undo %ld changes\n", count_modifications / 2);
                    break;
                }

                for (i = 0; i < count_undo; ++i) {
                    stack_pop(&stack_modifications, &c);
                    if (c == '2') {
                        list_pop(&list_new_data, &to_change);
                        list_pop(&list_old_data, &new_data);
                        liver_change(&list, &to_change, &new_data);
                    } else if (c == '3') {
                        list_pop(&list_new_data, &new_data);
                        list_delete_node(&list, &new_data);
                    } else if (c == '4') {
                        list_pop(&list_old_data, &new_data);
                        if (list_insert(&list, &new_data) == MEMORY_ERROR) {
                            printf("List_inser: Memory error\n");
                            stack_destroy(&stack_modifications);
                            list_destroy(&list);
                            list_destroy(&list_old_data);
                            list_destroy(&list_new_data);
                            return MEMORY_ERROR;
                        }
                    }
                }
                printf("Undo OK. Cancelled %ld modifications\n", count_modifications / 2);
                count_modifications = 0;
                list_destroy(&list_old_data);
                list_destroy(&list_new_data);
                stack_destroy(&stack_modifications);
                break;
            }
        }
        free_buf();
        puts("\n");
        print_menu();
    }
    stack_destroy(&stack_modifications);
    list_destroy(&list);
    list_destroy(&list_old_data);
    list_destroy(&list_new_data);
    return 0;
}