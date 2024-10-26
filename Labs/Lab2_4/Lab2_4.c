#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum {
    VALID,
    NOT_A_NUM,
    EMPTY_LINE,
    MALLOC_FAILED,
    WRONG_NUMBER_OF_ARGS,
    NO_CONVEXITY,
    CONVEXITY,
    N_NOT_POSITIVE_NUMBER,
    WRONG_BASE_OR_ARG,
    OK
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case EMPTY_LINE: printf("empty line\n"); break;
        case NOT_A_NUM: printf("not a number\n"); break;
        case MALLOC_FAILED: printf("malloc failed\n"); break;
        case WRONG_NUMBER_OF_ARGS: printf("wrong number of arguments\n"); break;
        case NO_CONVEXITY: printf("No convexity\n"); break;
        case CONVEXITY: printf("Convexity\n"); break;
        case N_NOT_POSITIVE_NUMBER: printf("N IS NOT A POSITIVE NUMBER\n"); break;
        case WRONG_BASE_OR_ARG: printf("WRONG BASE OR ARGUMENT\n"); break;
        case OK: break;
        default: printf("Unknown error code\n"); break;
    }
}

char* intToStr(int N, char* buf) {
    char* str = buf;
    int i = BUFSIZ - 2;
    while (N > 0) {
        str[i--] = N % 10 + '0';
        N /= 10;
    }
    str[BUFSIZ - 1] = '\0';
    return str;
}

char* fbaseto10(char* el, const int base) {
    int res = 0, len = 0;
    while (*el) {
        if (isalpha(*el)) res = res * base + *el++ - 'A' + 10;
        else res = res * base + *el++ - '0';
    }
    int res_1 = res;
    while(res_1) {
        len++;
        res_1 /= 10;
    }
    char* buf = malloc(sizeof(char) * len);
    if (buf == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    char* result = intToStr(res, buf);
    return result;
}

int ValidateNum(char* argv, int base) {
    int cnt = 0;
    while (*argv != '\0') {
        if (!isalnum(*argv)) {
            return NOT_A_NUM;
        }
        char* x = fbaseto10(argv, 10);
        if(x == NULL) {
            return MALLOC_FAILED;
        }
        if (atoi(x) + 1 > base) {
            free(x);
            return WRONG_BASE_OR_ARG;
        }
        argv++;
        cnt++;
    }
    if (cnt == 0) {
        return EMPTY_LINE;
    }
    return VALID;
}

typedef struct {
    double x, y;
}Point;

int TheConvexityOfThePolygon(int cnt, ...){
    if (cnt < 6) {
        return WRONG_NUMBER_OF_ARGS;
    }
    va_list args;
    va_start (args, cnt);
    int flag_x = 0, ind = 0;
    Point* mass_points = (Point*)malloc((cnt / 2 + 1) * sizeof(Point));
    if (mass_points == NULL) {
        free(mass_points);
        return MALLOC_FAILED;
    }
    for(int i = 0; i < cnt; i++) {
        double tek = va_arg(args, double);
        if (flag_x == 0) {
            mass_points[ind].x = tek;
            flag_x = 1;
        }
        else {
            mass_points[ind++].y = tek;
            flag_x = 0;
        }
    }
    mass_points[ind] = mass_points[0];
    if (flag_x == 1) {
        free(mass_points);
        return WRONG_NUMBER_OF_ARGS;
    }
    double pred_flag_turn = 0;
    for (int i = 1; i < ind; i++) {
        double ABx = mass_points[i].x - mass_points[i - 1].x, ABy = mass_points[i].y - mass_points[i - 1].y;
        double BCx = mass_points[i + 1].x - mass_points[i].x, BCy = mass_points[i + 1].y - mass_points[i].y;
        double flag_turn = ABx * BCy - ABy * BCx;
        if (pred_flag_turn * flag_turn < 0) {
            free(mass_points);
            return NO_CONVEXITY;
        }
        pred_flag_turn = flag_turn;
    }
    free(mass_points);
    va_end (args);
    return CONVEXITY;
}

int CalculatingThePolynomial(double point, int n, ...) {
    if (n <= 0) {
        return N_NOT_POSITIVE_NUMBER;
    }
    va_list args;
    va_start (args, n);
    double* mass_coefs = (double*)malloc(n * sizeof(double));
    if (mass_coefs == NULL) {
        return MALLOC_FAILED;
    }
    for(int i = n - 1; i > -1; i--) {
        mass_coefs[i] = va_arg(args, double);
    }
    double result = mass_coefs[0], prev = point;
    for (int i = 1; i < n; i++) {
        result += prev * mass_coefs[i];
        prev *= point;
    }
    printf("%lf\n", result);
    free(mass_coefs);
    return OK;
}

int ISCUPCERNUMBER(char* num, int base) {
    char* x = fbaseto10(num, base);
    if(x == NULL) {
        return MALLOC_FAILED;
    }
    int number = atoi(x);
    free(x);
    if (number == 1) return 1;
    int squared = number * number;
    int numDigits = (int)log10(squared) + 1;
    for (int splitPos = 1; splitPos < numDigits; splitPos++) {
        int divisor = (int)pow(10, splitPos);
        int splitSum = squared / divisor + squared % divisor;
        if (splitSum == number) return 1;
    }
    return 0;
}

int CUPCERNUMBERS(int base, int cnt, ...) {
    if (base < 2 || base > 36) {
        return WRONG_BASE_OR_ARG;
    }
    va_list args;
    va_start (args, cnt);
    for(int i = 0; i < cnt; i++) {
        char* tek_num_str = va_arg(args, char*);
        if (ValidateNum(tek_num_str, base) != VALID) {
            ValidateCode(ValidateNum(tek_num_str, base));
            continue;
        }
        if (ISCUPCERNUMBER(tek_num_str, base) == 1) {
            printf("%s IS CUPCER'S NUMBER\n", tek_num_str);
        }
        else {
            printf("%s IS NOT CUPCER'S NUMBER\n", tek_num_str);
        }
    }
    va_end(args);
    return OK;
}

int main() {
    ValidateCode(TheConvexityOfThePolygon(10, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.5, 0.5));
    ValidateCode(TheConvexityOfThePolygon(8, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0));
    ValidateCode(TheConvexityOfThePolygon(3, 0.4, 0.2, 0.1));
    ValidateCode(CalculatingThePolynomial(2.0, 2, 1.0, 2.0));
    ValidateCode(CalculatingThePolynomial(6.0, 3, 3.0, 2.0, 1.0));
    ValidateCode(CalculatingThePolynomial(14.3, 4, 1.0, 2.0, 1.5));
    ValidateCode(CUPCERNUMBERS(15, 3, "G", "B", "2"));
    ValidateCode(CUPCERNUMBERS(27, 3, "R", "Z", "5"));
    ValidateCode(CUPCERNUMBERS(-1, 3, "R", "Z", "5"));
    return 0;
}