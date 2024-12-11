#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect args count");
        return -1;
    }
    printf("%s", argv[1]);
    int a;
    int b;
    int c;

    scanf("%d\n%d\n%d", &a, &b, &c);
    int res = a + b == c;
    printf(res ? "true" : "false");
    return 0;
}