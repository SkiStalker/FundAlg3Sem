#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct Node {
    int coeff;
    int power;  // Степень
    struct Node *next;
} Node;

// Функция для создания нового узла
Node* createNode(int coeff, int power) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    newNode->coeff = coeff;
    newNode->power = power;
    newNode->next = NULL;
    return newNode;
}

// Функция для добавления узла в список, соблюдая порядок степеней по убыванию
Node* insertNode(Node* head, int coeff, int power) {
    if (coeff == 0) return head;  // Пропуск коэффициента 0

    Node* newNode = createNode(coeff, power);
    if (head == NULL || head->power < power) {
        newNode->next = head;
        return newNode;
    }

    Node* current = head;
    if (current->power == power) {
        current->coeff += coeff;
        if (current->coeff == 0) {  // Удаление узла с коэффициентом 0
            Node* temp = current;
            current = temp->next;
            free(temp);
        }
        free(newNode);
        return head;
    }
    else {
        while (current->next != NULL && current->next->power >= power) {
            if (current->next->power == power) {
                current->next->coeff += coeff;
                if (current->next->coeff == 0) {  // Удаление узла с коэффициентом 0
                    Node* temp = current->next;
                    current->next = temp->next;
                    free(temp);
                }
                free(newNode);
                return head;
            }
            current = current->next;
        }
    }

    newNode->next = current->next;
    current->next = newNode;
    return head;
}

// Функция для отображения многочлена
int printPolynomial(Node* head) {
    if (head == NULL) {
        printf("0\n");
        return 1;
    }

    Node* temp = head;
    while (temp != NULL) {
        if (temp != head && temp->coeff > 0) {
            printf(" + ");
        }
        if (temp->coeff < 0) {
            printf(" - ");
        }
        printf("%dx^%d", abs(temp->coeff), temp->power);
        temp = temp->next;
    }
    printf("\n");
    return 0;
}

// Функция для очистки памяти, занятой многочленом
Node* freePolynomial(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return NULL;
}

// Функция для парсинга строки в многочлен
Node* parsePolynomial(const char* str) {
    Node* poly = NULL;
    int coeff, power, sign = 1;
    const char *ptr = str;

    while (*ptr) {
        while (*ptr == ' ') ptr++;  // Пропуск пробелов

        if (*ptr == '+') {
            sign = 1;
            ptr++;
        } else if (*ptr == '-') {
            sign = -1;
            ptr++;
        }

        if (isdigit(*ptr) || *ptr == 'x') {
            coeff = 1;
            if (isdigit(*ptr)) {
                coeff = 0;
                while (isdigit(*ptr)) {
                    coeff = coeff * 10 + (*ptr - '0');
                    ptr++;
                }
            }

            if (*ptr == 'x') {
                ptr++;
                power = 1;
                if (*ptr == '^') {
                    ptr++;
                    power = 0;
                    while (isdigit(*ptr)) {
                        power = power * 10 + (*ptr - '0');
                        ptr++;
                    }
                }
            } else {
                power = 0;
            }

            poly = insertNode(poly, sign * coeff, power);
        } else {
            ptr++;
        }
    }

    return poly;
}

// Функция для сложения двух многочленов
Node* addPolynomials(Node* p1, Node* p2) {
    Node* result = NULL;
    while (p1 != NULL) {
        result = insertNode(result, p1->coeff, p1->power);
        p1 = p1->next;
    }
    while (p2 != NULL) {
        result = insertNode(result, p2->coeff, p2->power);
        p2 = p2->next;
    }
    return result;
}

// Функция для вычитания многочленов
Node* subtractPolynomials(Node* p1, Node* p2) {
    Node* result = NULL;
    while (p1 != NULL) {
        result = insertNode(result, p1->coeff, p1->power);
        p1 = p1->next;
    }
    while (p2 != NULL) {
        result = insertNode(result, -p2->coeff, p2->power);
        p2 = p2->next;
    }
    return result;
}

// Функция для умножения двух многочленов
Node* multiplyPolynomials(Node* p1, Node* p2) {
    Node* result = NULL;
    for (Node* temp1 = p1; temp1 != NULL; temp1 = temp1->next) {
        for (Node* temp2 = p2; temp2 != NULL; temp2 = temp2->next) {
            result = insertNode(result, temp1->coeff * temp2->coeff, temp1->power + temp2->power);
        }
    }
    return result;
}

// Функция для вычисления значения многочлена в заданной точке
int evaluatePolynomial(Node* head, int x) {
    int result = 0;
    while (head != NULL) {
        result += head->coeff * pow(x, head->power);
        head = head->next;
    }
    return result;
}

// Функция для нахождения производной многочлена
Node* differentiatePolynomial(Node* head) {
    Node* result = NULL;
    while (head != NULL) {
        if (head->power > 0) {
            result = insertNode(result, head->coeff * head->power, head->power - 1);
        }
        head = head->next;
    }
    return result;
}

// Функция для композиции двух многочленов (подстановка одного многочлена в другой)
Node* composePolynomials(Node* p1, Node* p2) {
    Node* result = NULL;
    for (Node* temp1 = p1; temp1 != NULL; temp1 = temp1->next) {
        int coeff = temp1->coeff;
        int power = temp1->power;
        // Подставляем p2 в p1
        Node* temp2 = p2;
        int polyValue = 0;
        while (temp2 != NULL) {
            polyValue += temp2->coeff * pow(coeff, temp2->power);
            temp2 = temp2->next;
        }
        result = insertNode(result, polyValue, power);
    }
    return result;
}






// Функция для деления двух многочленов (целочисленное деление)
Node* dividePolynomials(Node* p1, Node* p2) {
    Node* quotient = NULL;
    Node* remainder = NULL;
    remainder = p1;

    while (remainder != NULL && remainder->power >= p2->power) {
        int coeff = remainder->coeff / p2->coeff;
        int power = remainder->power - p2->power;

        quotient = insertNode(quotient, coeff, power);

        Node* temp = NULL;
        for (Node* temp2 = p2; temp2 != NULL; temp2 = temp2->next) {
            temp = insertNode(temp, coeff * temp2->coeff, power + temp2->power);
        }

        remainder = subtractPolynomials(remainder, temp);

        freePolynomial(temp);
    }

    return quotient;
}

// Функция для нахождения остатка от деления многочлена на многочлен
Node* modPolynomials(Node* p1, Node* p2) {
    Node* remainder = p1;

    while (remainder != NULL && remainder->power >= p2->power) {
        int coeff = remainder->coeff / p2->coeff;
        int power = remainder->power - p2->power;

        // Создаем многочлен, который нужно вычесть
        Node* temp = NULL;
        for (Node* temp2 = p2; temp2 != NULL; temp2 = temp2->next) {
            temp = insertNode(temp, coeff * temp2->coeff, power + temp2->power);
        }

        // Вычитаем (частное * делитель) из остатка
        remainder = subtractPolynomials(remainder, temp);
        freePolynomial(temp);
    }

    return remainder;
}






// Функция для обработки операций, включая Div и Mod
Node* processOperation(char* command, Node* sum) {
    char poly1[256], poly2[256];
    int x;

    if (strncmp(command, "Add", 3) == 0) {
        sscanf(command, "Add(%[^,],%[^)])", poly1, poly2);
        Node* p1 = parsePolynomial(poly1);
        Node* p2 = parsePolynomial(poly2);
        sum = addPolynomials(p1, p2);
        printPolynomial(sum);
        freePolynomial(p1);
        freePolynomial(p2);
    } else if (strncmp(command, "Sub", 3) == 0) {
        sscanf(command, "Sub(%[^,],%[^)])", poly1, poly2);
        Node* p1 = parsePolynomial(poly1);
        Node* p2 = parsePolynomial(poly2);
        sum = subtractPolynomials(p1, p2);
        printPolynomial(sum);
        freePolynomial(p1);
        freePolynomial(p2);
    } else if (strncmp(command, "Mult", 4) == 0) {
        sscanf(command, "Mult(%[^,],%[^)])", poly1, poly2);
        Node* p1 = parsePolynomial(poly1);
        Node* p2 = parsePolynomial(poly2);
        sum = multiplyPolynomials(p1, p2);
        printPolynomial(sum);
        freePolynomial(p1);
        freePolynomial(p2);
    } else if (strncmp(command, "Eval", 4) == 0) {
        sscanf(command, "Eval(%d)", &x);
        int result = evaluatePolynomial(sum, x);
        printf("Значение в точке x = %d: %d\n", x, result);
    } else if (strncmp(command, "Diff", 4) == 0) {
        sum = differentiatePolynomial(sum);
        printPolynomial(sum);
    } else if (strncmp(command, "Cmps", 4) == 0) {
        sscanf(command, "Cmps(%[^,],%[^)])", poly1, poly2);
        Node* p1 = parsePolynomial(poly1);
        Node* p2 = parsePolynomial(poly2);
        sum = composePolynomials(p1, p2);
        printPolynomial(sum);
        freePolynomial(p1);
        freePolynomial(p2);
    } else if (strncmp(command, "Div", 3) == 0) {
        sscanf(command, "Div(%[^,],%[^)])", poly1, poly2);
        Node* p1 = parsePolynomial(poly1);
        Node* p2 = parsePolynomial(poly2);
        sum = dividePolynomials(p1, p2);
        printPolynomial(sum);
        freePolynomial(p1);
        freePolynomial(p2);
    } else if (strncmp(command, "Mod", 3) == 0) {
        sscanf(command, "Mod(%[^,],%[^)])", poly1, poly2);
        Node* p1 = parsePolynomial(poly1);
        Node* p2 = parsePolynomial(poly2);
        sum = modPolynomials(p1, p2);
        printPolynomial(sum);
        freePolynomial(p1);
        freePolynomial(p2);
    }

    return sum;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Использование: %s <файл1> <файл2> ... <файлN>\n", argv[0]);
        return 1;
    }
    Node* sum = NULL;
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Ошибка при открытии файла\n");
        return 1;
    }

    char command[256];
    while (fgets(command, sizeof(command), file)) {
        if (command[0] != '%' && command[0] != '[') {
            sum = processOperation(command, sum);
        }
    }

    fclose(file);
    freePolynomial(sum);
    return 0;
}
