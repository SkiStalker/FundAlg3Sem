#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    SUCCESS,
    ERROR_MEMORY,
    ERROR_INVALID_INPUT,
    ERROR_DIVISION_BY_ZERO,
    ERROR_UNKNOWN_OPERATION,
    ERROR_OPEN_FILE
} ErrorCode;

typedef struct Node {
    int coeff;
    int exp;
    struct Node* next;
} Node;

typedef struct Polynomial {
    Node* head;
} Polynomial;

Polynomial* createPoly() {
    Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));
    if (!poly) {
        return NULL;
    }
    poly->head = NULL;
    return poly;
}

void freePoly(Polynomial* poly) {
    Node* current = poly->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(poly);
}

ErrorCode addTerm(Polynomial* poly, int coeff, int exp) {
    if (coeff == 0) return SUCCESS;

    Node* current = poly->head;
    Node* prev = NULL;

    while (current != NULL && current->exp > exp) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->exp == exp) {
        current->coeff += coeff;
        if (current->coeff == 0) {
            if (prev != NULL) {
                prev->next = current->next;
            }
            else {
                poly->head = current->next;
            }
            free(current);
        }
    }
    else {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (!newNode) return ERROR_MEMORY;
        newNode->coeff = coeff;
        newNode->exp = exp;
        newNode->next = current;

        if (prev == NULL) {
            poly->head = newNode;
        }
        else {
            prev->next = newNode;
        }
    }

    return SUCCESS;
}

Polynomial* addPoly(const Polynomial* poly1, const Polynomial* poly2) {
    Polynomial* result = createPoly();
    if (!result) return NULL;

    Node* node1 = poly1->head;
    Node* node2 = poly2->head;

    while (node1 || node2) {
        int coeff = 0, exp = 0;
        if (node1 && (!node2 || node1->exp > node2->exp)) {
            coeff = node1->coeff;
            exp = node1->exp;
            node1 = node1->next;
        }
        else if (node2 && (!node1 || node2->exp > node1->exp)) {
            coeff = node2->coeff;
            exp = node2->exp;
            node2 = node2->next;
        }
        else {
            coeff = node1->coeff + node2->coeff;
            exp = node1->exp;
            node1 = node1->next;
            node2 = node2->next;
        }
        if (coeff != 0) {
            addTerm(result, coeff, exp);
        }
    }
    return result;
}

Polynomial* subtractPoly(const Polynomial* poly1, const Polynomial* poly2) {
    Polynomial* result = createPoly();
    if (!result) return NULL;

    Node* node = poly1->head;
    while (node) {
        addTerm(result, node->coeff, node->exp);
        node = node->next;
    }
    node = poly2->head;
    while (node) {
        addTerm(result, -node->coeff, node->exp);
        node = node->next;
    }
    return result;
}

Polynomial* multiplyPoly(const Polynomial* poly1, const Polynomial* poly2) {
    Polynomial* result = createPoly();
    if (!result) return NULL;

    Node* term1 = poly1->head;
    while (term1) {
        Node* term2 = poly2->head;
        while (term2) {
            int coeff = term1->coeff * term2->coeff;
            int exp = term1->exp + term2->exp;
            addTerm(result, coeff, exp);
            term2 = term2->next;
        }
        term1 = term1->next;
    }
    return result;
}

Polynomial* dividePoly(const Polynomial* dividend, const Polynomial* divisor, Polynomial** remainder) {
    Polynomial* quotient = createPoly();
    if (!quotient) return NULL;

    *remainder = createPoly();
    if (!*remainder) {
        freePoly(quotient);
        return NULL;
    }

    Node* node = dividend->head;
    while (node) {
        addTerm(*remainder, node->coeff, node->exp);
        node = node->next;
    }

    Node* divisorHead = divisor->head;
    while ((*remainder)->head && (*remainder)->head->exp >= divisorHead->exp) {
        // Получаем коэффициент и степень для лидера (ведущего терма)
        int leadCoeff = (*remainder)->head->coeff / divisorHead->coeff;
        int leadExp = (*remainder)->head->exp - divisorHead->exp;

        // Добавляем в частное
        addTerm(quotient, leadCoeff, leadExp);

        // Вычисляем временный многочлен для вычитания
        Polynomial* temp = createPoly();
        addTerm(temp, leadCoeff, leadExp);
        Polynomial* tempDiv = multiplyPoly(temp, divisor);

        // Вычитаем из остатка
        Polynomial* newRemainder = subtractPoly(*remainder, tempDiv);

        freePoly(temp);
        freePoly(tempDiv);

        // Обновляем остаток
        freePoly(*remainder);
        *remainder = newRemainder;
    }

    return quotient;
}


Polynomial* modulusPoly(const Polynomial* dividend, const Polynomial* divisor) {
    Polynomial* remainder;
    Polynomial* quotient = dividePoly(dividend, divisor, &remainder);
    freePoly(quotient);
    return remainder;
}

Polynomial* differentiatePoly(const Polynomial* poly) {
    Polynomial* result = createPoly();
    if (!result) return NULL;

    Node* node = poly->head;
    while (node) {
        if (node->exp > 0) {
            int newCoeff = node->coeff * node->exp;
            int newExp = node->exp - 1;
            addTerm(result, newCoeff, newExp);
        }
        node = node->next;
    }
    return result;
}

int power(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

int evaluatePoly(const Polynomial* poly, int x) {
    int result = 0;
    Node* node = poly->head;
    while (node) {
        result += node->coeff * power(x, node->exp);
        node = node->next;
    }
    return result;
}

Polynomial* parsePolynomial(const char* str) {
    Polynomial* poly = createPoly();
    if (!poly) return NULL;

    int coeff = 0, exp = 0, sign = 1;
    bool readingCoeff = true;
    bool expProvided = false;

    while (*str) {
        if (*str == '-') {
            sign = -1;
            readingCoeff = true;
            expProvided = false;
        }
        else if (*str == '+') {
            sign = 1;
            readingCoeff = true;
            expProvided = false;
        }
        else if (isdigit(*str)) {
            int value = 0;
            while (isdigit(*str)) {
                value = value * 10 + (*str - '0');
                str++;
            }
            if (readingCoeff) {
                coeff = sign * value;
                readingCoeff = false;
            }
            else {
                exp = value;
                expProvided = true;
            }
            continue;
        }
        else if (*str == 'x') {
            if (readingCoeff) coeff = sign;
            if (*(str + 1) == '^') {
                str += 2;  // Skip past "x^"
                exp = *str - '0';  // Assuming single-digit exponents
                expProvided = true;
            }
            else {
                exp = 1;  // Implicit exponent 1 for "x"
                expProvided = true;
            }
            readingCoeff = false;
        }

        if (readingCoeff == false && expProvided) {
            addTerm(poly, coeff, exp);
            coeff = 0;
            exp = 0;
            sign = 1;
            expProvided = false;
        }

        str++;
    }

    // Add the last term if needed
    if (coeff != 0 || readingCoeff == false) {
        addTerm(poly, coeff, exp);
    }

    return poly;
}


Polynomial* composePoly(const Polynomial* poly1, const Polynomial* poly2) {
    Polynomial* result = createPoly();
    if (!result) return NULL;

    // Пройти по всем членам первого многочлена
    Node* node1 = poly2->head;
    while (node1) {
        // Для каждого члена poly1 добавляем его композицию с poly2
        Polynomial* termPoly = createPoly();
        addTerm(termPoly, node1->coeff, 0); // Это просто коэффициент
        Polynomial* termResult = multiplyPoly(termPoly, poly1); // Композиция

        // Добавляем результат в итоговый многочлен
        addTerm(result, termResult->head->coeff, node1->exp);
        freePoly(termPoly);
        freePoly(termResult);

        node1 = node1->next;
    }
    return result;
}



void printPoly(const Polynomial* poly) {
    if (!poly || !poly->head) {
        printf("0\n");
        return;
    }

    Node* current = poly->head;
    bool isFirstTerm = true;

    while (current) {
        int coeff = current->coeff;
        int exp = current->exp;

        // Пропускаем термы с нулевым коэффициентом
        if (coeff == 0) {
            current = current->next;
            continue;
        }

        // Если это не первый терм, нужно добавить знак
        if (!isFirstTerm) {
            if (coeff > 0) {
                printf(" + ");
            }
            else {
                printf(" - ");
            }
        }

        // Выводим коэффициент и переменную
        if (exp == 0) {
            // Если степень 0, выводим только коэффициент
            printf("%d", abs(coeff));
        }
        else if (exp == 1) {
            // Если степень 1, выводим коэффициент и переменную без степени
            if (abs(coeff) == 1) {
                printf("x");
            }
            else {
                printf("%dx", abs(coeff));
            }
        }
        else {
            // Для всех остальных степеней
            if (abs(coeff) == 1) {
                printf("x^%d", exp);
            }
            else {
                printf("%dx^%d", abs(coeff), exp);
            }
        }

        current = current->next;
        isFirstTerm = false;
    }
    printf("\n");
}


ErrorCode processCommand(const char* command, Polynomial** accumulator) {
    char commandCopy[256];
    strncpy(commandCopy, command, sizeof(commandCopy) - 1);
    commandCopy[sizeof(commandCopy) - 1] = '\0';
    char* operation = strtok(commandCopy, "(");
    char* arguments = strtok(NULL, ")");
    if (!operation || !arguments) {
        return ERROR_INVALID_INPUT;
    }

    Polynomial* poly1 = NULL, * poly2 = NULL;
    char* arg1 = strtok(arguments, ",");
    char* arg2 = strtok(NULL, ",");

    // Обработка аргументов
    if (arg1) {
        if (arg2) {
            poly1 = parsePolynomial(arg1);
            poly2 = parsePolynomial(arg2);
        }
        else {
            poly1 = *accumulator;
            poly2 = parsePolynomial(arg1);
        }
    }

    Polynomial* result = NULL;
    if (strcmp(operation, "Add") == 0) {
        result = addPoly(poly1, poly2);
        printf("Addition result: ");
    }
    else if (strcmp(operation, "Sub") == 0) {
        result = subtractPoly(poly1, poly2);
        printf("Subtraction result: ");
    }
    else if (strcmp(operation, "Mult") == 0) {
        result = multiplyPoly(poly1, poly2);
        printf("Multiplication result: ");
    }
    else if (strcmp(operation, "Div") == 0) {
        Polynomial* remainder = NULL;
        result = dividePoly(poly1, poly2, &remainder);
        if (!result) {
            return ERROR_DIVISION_BY_ZERO;
        }
        printf("Division result (Quotient): ");
        freePoly(remainder);
    }
    else if (strcmp(operation, "Mod") == 0) {
        result = modulusPoly(poly1, poly2);
        printf("Modulus result: ");
    }
    else if (strcmp(operation, "Eval") == 0) {
        int x = atoi(arg1);
        int evalResult = evaluatePoly(*accumulator, x);  // Используем аккумулятор
        printf("Evaluation result at x=%d: %d\n", x, evalResult);
        return SUCCESS;
    }
    else if (strcmp(operation, "Diff") == 0) {
        if (arg1) {
            poly1 = parsePolynomial(arg1);  // Используем выражение из аргумента
        }
        result = differentiatePoly(poly1);
        printf("Differentiation result: ");
    }
    else if (strcmp(operation, "Cmps") == 0) {
        if (arg2) {
            poly1 = parsePolynomial(arg1);
            poly2 = parsePolynomial(arg2);
        }
        else {
            poly1 = *accumulator;
            poly2 = parsePolynomial(arg1);
        }
        result = composePoly(poly1, poly2);
        printf("Composition result: ");
    }
    else {
        return ERROR_UNKNOWN_OPERATION;
    }

    if (result) {
        printPoly(result);
        freePoly(*accumulator);
        *accumulator = result;
    }

    return SUCCESS;
}


ErrorCode processFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return ERROR_OPEN_FILE;
    }

    Polynomial* accumulator = createPoly();
    if (!accumulator) {
        fclose(file);
        return ERROR_MEMORY;
    }

    char line[256];
    bool inMultilineComment = false;  // Флаг для многострочного комментария
    while (fgets(line, sizeof(line), file)) {
        // Пропускаем пустые строки
        if (strlen(line) == 0 || line[0] == '\n') {
            continue;
        }

        // Пропускаем однострочные комментарии
        if (line[0] == '%') {
            continue;
        }

        // Обрабатываем многострочные комментарии
        if (!inMultilineComment) {
            if (line[0] == '[') {
                inMultilineComment = true;
                continue;  // Пропускаем строку начала многострочного комментария
            }
        }
        else {
            if (strchr(line, ']')) {  // Закрывающая скобка для многострочного комментария
                inMultilineComment = false;
                continue;  // Пропускаем строку конца многострочного комментария
            }
            continue;  // Пропускаем строки внутри многострочного комментария
        }

        // Теперь строки не являются комментариями, можно обрабатывать команду
        ErrorCode result = processCommand(line, &accumulator);
        if (result != SUCCESS) {
            printf("Error processing command: %d\n", result);
        }
    }

    freePoly(accumulator);
    fclose(file);
    return SUCCESS;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: No filename provided.\n");
        return ERROR_INVALID_INPUT;
    }

    ErrorCode result = processFile(argv[1]);
    if (result != SUCCESS) {
        printf("Error: Failed to process file.\n");
        return ERROR_INVALID_INPUT;
    }

    printf("File processed successfully.\n");
    return SUCCESS;
}
