#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_space(int n, FILE* ofile) {
    for (int i = 0; i < n; i++)
        fprintf(ofile, " ");
}

void print_node(int n, char* node, FILE* ofile) {
    print_space(n, ofile);
    fprintf(ofile, "%s\n", node);
}

int is_symbol(char c) {
    return c != ' ' && c != ',' && c != '(' && c != ')';
}

void fprintf_tree(char* cur_string, FILE* ofile) {
    fprintf(ofile, "Current tree:\n\n");
    char node[BUFSIZ];
    int last = strlen(cur_string) - 1, level = 0, i_node = 0;
    if (cur_string[last] == '\n')
        cur_string[last] = '\0';
    for (int i_str = 0; cur_string[i_str] != '\0'; i_str++) {
        if ((cur_string[i_str] == ' ' || cur_string[i_str] == ',') && i_str > 0) {
            if (is_symbol(cur_string[i_str - 1])) {
                node[i_node] = '\0';
                print_node(level, node, ofile);
                i_node = 0;
                node[0] = '\0';
            }
        }
        else if (is_symbol(cur_string[i_str])) {
            if (!i_str || cur_string[i_str - 1] == '(')
                level++;
            node[i_node] = cur_string[i_str];
            i_node++;
        }
        else if (cur_string[i_str] == ')') {
            if (is_symbol(cur_string[i_str - 1])) {
                node[i_node] = '\0';
                print_node(level, node, ofile);
                i_node = 0;
                node[0] = '\0';
            }
            if (level > 0)
                level--;
        }
    }
     node[i_node] = '\0';
     print_node(level, node, ofile);
    fprintf(ofile, "\n\n");
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Incorrect number of arguments");
        return 1;
    }

    FILE* ifile = fopen(argv[1], "r");
    if (!ifile) {
        printf("File %s opening error", argv[1]);
        return 1;
    }

    FILE* ofile = fopen(argv[2], "w");
    if (!ofile) {
        fclose(ifile);
        printf("File %s opening error", argv[2]);
        return 1;
    }

    char cur_string[BUFSIZ];
    int flag = 0;

    while (fgets(cur_string, BUFSIZ, ifile)) {
        flag = 1;
        fprintf_tree(cur_string, ofile);
    }

    if (!flag)
        fprintf(ofile, "File %s is empty", argv[1]);
    printf("Data was written in %s", argv[2]);

    fclose(ifile);
    fclose(ofile);

    return 0;
}