#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>


int strLen(const char *str) {
    int len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

void strCpy(char *str1, char *str2) {
    while (*str2 != '\0') {
        *str1++ = *str2++;
    }
    *str1 = '\0';
}

char *reverse(const char *str) {
    int len = strLen(str);
    char *reversed = (char*)malloc(len + 1);
    if (reversed == NULL) {
        printf("Memory allocating error.\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';
    return reversed;
}

char * plus(char* arg1, char* arg2, int base) {
    int len1 = strLen(arg1), len2 = strLen(arg2);
    int lenUp, lenDown;

    char *res = (char*)malloc((len1 + len2 + 1)*sizeof(char));
    if (res == NULL) {
        printf("Memory allocating error.\n");
        return NULL;
    }
    char *up, *down;
    if (len1 >= len2) {
        up = arg1 + len1 - 1;
        down = arg2 + len2 - 1;
        lenDown = len2;
        lenUp = len1;
    } else {
        up = arg2 + len2 - 1;
        down = arg1 + len1 - 1;
        lenDown = len1;
        lenUp = len2;
    }
    int cur_poz = 0, mind = 0, r;

    char cur_el;
    while (lenDown > 0) {
        int down_el = (isdigit(*down)) ? *down - '0' : *down - 'a' + 10;
        int up_el = (isdigit(*up)) ? *up - '0' : *up - 'a' + 10;
        int cur_sum = down_el + up_el + mind;
        if (cur_sum >= base) {
            r = cur_sum - base;
            mind = (cur_sum / base);
        } else {
            r = cur_sum;
            mind = 0;
        }
        cur_el = (r > 9) ? (r - 10) + 'a' : r + '0';
        res[cur_poz++] = cur_el;

        down--; up--;
        lenDown--; lenUp--;
    }

    while (lenUp > 0) {
        if (mind > 0) {
            int s = mind + ((isdigit(*up)) ? *up - '0' : *up - 'a' + 10);
            if (s >= base) {
                r = s - base;
                mind = (s / base);
            } else {
                r = s;
                mind = 0;
            }
            cur_el = (r > 9) ? (r - 10) + 'a' : r + '0';
        } else {
            cur_el = *up;
        }
        res[cur_poz++] = cur_el;

        up--;
        lenUp--;
    }
    if (mind > 0) {
        res[cur_poz++] = (mind > 9) ? mind - 10 + 'a' : mind + '0';
    }
    res[cur_poz] = '\0';

    char *reversed = reverse(res);
    free(res);
    return reversed;
}

char * summ(int base, int cnt, ...) {
    va_list arg;
    va_start(arg, cnt);

    if (cnt < 1) {
        printf("Nothing was input.\n");
        va_end(arg);
        return NULL;
    } else if (cnt == 1) {
        char *str = va_arg(arg, char*);
        va_end(arg);
        return str;
    }

    char* first = va_arg(arg, char*);
    int len = strLen(first);

    char* res = (char*)malloc((len + 1) * sizeof(char));
    if (res == NULL) {
        printf("Memory allocating error.\n");
        return NULL;
    }
    strCpy(res, first);
    char* num;
    for (int i = 1; i < cnt; ++i) {
        num = va_arg(arg, char*);
        char* promRes = plus(res, num, base);
        strCpy(res, promRes);
        free(promRes);
    }
    return res;
}

int main() {
    char* res = summ(10, 4, "123", "8976", "235253", "52");
    printf("%s\n", res);
    free(res);
}