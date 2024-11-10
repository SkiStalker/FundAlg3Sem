#include "functions.h"
#include "return_code.h"

int main(int argc, char **argv) {
    Tree tree;
    char separators[128], user_ans;
    int i;
    if (argc < 3) {
        printf("Input error\n");
        printf("Usage ./{filename} input.txt , ; .\n");
        return BAD_INPUT_ERROR;
    }
    for (i = 2; i < argc; ++i) {
        if (strlen(argv[i]) != 1) {
            printf("Input separators error\n");
            return BAD_INPUT_ERROR;
        }
        separators[i - 2] = argv[i][0];
    }
    separators[i - 2] = '\0';

    FILE *input = fopen(argv[1], "r");
    if (!input){
        printf("File open error\n");
        return FILE_ERROR;
    }
    init_tree(&tree);
    if (build_tree_from_file(&tree, separators, input)) {
        delete_tree(&tree);
        fclose(input);
        printf("Memory error\n");
        return MEMORY_ERROR;
    }
    fclose(input);

    if (print_n_often(&tree, -1) == MEMORY_ERROR) {
        delete_tree(&tree);
        printf("Memory error\n");
        return MEMORY_ERROR;
    }

    print_menu();
    while ((user_ans = getchar()) != '0') {
        if (user_ans == '\n')
            continue;
        switch (user_ans) {
            case '1': {
                String word;
                Node *found;
                printf("Enter word: ");
                getchar();
                if (string_scan(&word)) {
                    printf("Memory error\n");
                    delete_string_content(&word);
                    break;
                }
                if (find_string(&word, &tree, &found) == MISSING_ERROR) {
                    printf("Word missing in the text\n");
                    delete_string_content(&word);
                    break;
                }
                printf("%s - %zu time(s)\n", word.arr, found->count_repeats);
                delete_string_content(&word);
                break;
            }
            case '2': {
                int n;
                printf("Enter count: ");
                if (scanf("%d", &n) != 1) {
                    printf("Input error. Try again\n");
                    break;
                }
                if (print_n_often(&tree, n) == MEMORY_ERROR) {
                    printf("Memory error detected. Try again later\n");
                }
                break;
            }
            case '3': {
                String word;
                if (find_longest_word(&tree, &word) == MISSING_ERROR) {
                    printf("File is empty\n");
                    break;
                }
                printf("Found - %s\n", word.arr);
                delete_string_content(&word);
                break;
            }
            case '4': {
                String word;
                if (find_shortest_word(&tree, &word) == MISSING_ERROR) {
                    printf("File is empty\n");
                    break;
                }
                printf("Found - %s\n", word.arr);
                delete_string_content(&word);
                break;
            }
            case '5': {
                int depth;
                if ((depth = max_depth(tree.root)) == -1) {
                    printf("Tree is empty\n");
                    break;
                }
                printf("Depth: %d\n", depth);
                break;
            }
            case '6': {
                char file_path[BUFSIZ];
                FILE *output;
                printf("Enter file name: ");
                scanf("%s", file_path);
                printf("%s\n", file_path);
                output = fopen(file_path, "w");
                if (!output) {
                    printf("File open error. Try again\n");
                    break;
                }
                upload_tree_to_file(&tree, output, separators[0]);
                printf("Success. WARNING: TREE DELETED!\n");
                fclose(output);
                delete_tree(&tree);
                init_tree(&tree);
                break;
            }
            case '7': {
                if (tree.root) {
                    printf("Tree is not empty, try to upload tree to file (6)\n");
                    break;
                }
                char file_path[BUFSIZ];
                printf("Enter file name: ");
                scanf("%s", file_path);
                input = fopen(file_path, "r");
                if (!input) {
                    printf("File open error. Try again\n");
                    break;
                }
                if (build_tree_from_file(&tree, separators, input) == MEMORY_ERROR) {
                    fclose(input);
                    delete_tree(&tree);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                printf("Success\n");
                fclose(input);
                break;
            }
        }
        free_buf();
        print_menu();
    }
    delete_tree(&tree);
}