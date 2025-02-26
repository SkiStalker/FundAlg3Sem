#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef int (*callback) (const char*);

typedef struct {
    int a
} t_t;

int for_q(const char* str) {
    return 0;
}

int for_f(const char* str) {
    return 0;
}

int string_eq(const char* l, const char* r) {
    return !strcmp(l, r);
}

int find_flag(const char* arg, const char** f_arr, int f_arr_sz) {
    int i = 0 ;

    for (i  =0; i < f_arr_sz; i++) {
        if (string_eq(arg, f_arr[i])) {
            return i;
        }
    }
    return -1;
}

void foo() {
    srand(time(NULL));
    rand();
}


int main (int argc, char* argv[]) {

    int i  =0;

    const char* flags[] = {"-q", "/q", "-f", "/f"};

    callback callbacks[] ={ &for_q, &for_f};

    printf("%s\n", argv[0]);

    for (i = 1; i < argc; i++) {
        int res = find_flag(argv[i], flags, sizeof(flags) / sizeof (const char*));

        if (res == -1) {
            printf("Unknown flag %s\n", argv[i]);
            return -1;
        } else {
            callback find_callback = callbacks[res / 2];
            find_callback(argv[i]);
        }
    }
// "123123" "3" "12" "3" "12"
    return 0;
}