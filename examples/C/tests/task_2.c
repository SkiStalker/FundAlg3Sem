#include <stdio.h>
#include <ctype.h>

enum return_codes {
    SUCCESS,
    FILE_NOT_EXIST,
    WRONG_ARGS_COUNT
};


int main(int argc, char* argv[]) {
    char c;
    
    if (argc != 3) {
        printf("Incorrect args count");
        return WRONG_ARGS_COUNT;
    }
    FILE* fin = fopen(argv[1], "r");
    if (!fin) {
        printf("Can't open input file");
        return FILE_NOT_EXIST;
    }


    FILE* fout = fopen(argv[2], "w");
    if (!fin) {
        printf("Can't open input file");
        return FILE_NOT_EXIST;
    }
    while((c = getc(fin)) != EOF) {
        putc(toupper(c), fout);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}