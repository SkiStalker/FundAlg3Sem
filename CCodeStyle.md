# Правила написания кода лабораторных работ на языке Си

1. Нельзя использовать функцию _exit_ вне функции _main_ (в самой функции _main_ она не имеет смысла, т.к. можно вызвать return).

2. При работе с массивами и функциями запрещено передавать в них что то кроме значения переменных, констант или арифметических операций над ними (за исключением унарных).</br>
    Верный вариант:
    ```c
    #include <stdio.h>
    int main(void) {
        int i = 0, j = 1;
        int arr[12];
        int a = arr[i];
        i++;
        int r = fabs(j - i);
        printf("(i++)=%d, fabs(j-i)=%d, (a--)=%d", i, r, a);
        i++;
        a--;
        return 0;
    }
    ```
    Неверный вариант:
    ```c
    #include <stdio.h>
    int main(void) {
        int i = 0, j = 1;
        int arr[12];
        int a = arr[i++];
        printf("(i++)=%d, fabs(j-i)=%d, (a--)=%d", i++, fabs(j-i), a--);
        return 0;
    }
    ```

3. Все входные данные программы должны быть проверены на адекватность и корректность, нельзя использовать "сырой" ввод. Также необходимо контролировать количество переданных в командной строке аргументов.</br>
    Верный вариант:
    ```c
    #include <stdio.h>
    int main(int argc, char* argv[]) {
        if (argc > 1) {
            return -1;
        } 
        int i = 0;
        scanf("%d", &i);
        if (i < 100) {
            return 0;
        }
        return -1;
    }
    ```
    Неверный вариант:
    ```c
    #include <stdio.h>
    int main(int argc, char* argv[]) {
        int i = 0;
        scanf("%d", &i);
        return 0;
    } 
    ```

4. Если в коде выделяется динамическая память обязательно необходимо предусматривать ситуации, в которых память не может выделиться или перевыделиться. Также нельзя забывать очищать выделенную память.</br>
    Верный вариант:
    ```c
    #include <stdlib.h>
    int main(void) {
        int* ptr = (int*)malloc(sizeof(int) * 12);
        if (!ptr) {
            return -1;
        }

        ptr = (int*)realloc((void*)ptr, sizeof(int) * 20);
        if (!ptr) {
            return -1;
        }

        free(ptr);
        return 0;
    }
    ```
    Неверный вариант:
    ```c
    #include <stdio.h>
    int main(void) {
        int* ptr = (int*)malloc(sizeof(int) * 12);
        ptr = (int*)realloc((void*)ptr, sizeof(int) * 20);
        return 0;
    }   
    ```

5. При работе с файлами нужно проверять что он открылся и потом закрывать его.</br>
    Верный вариант:
    ```c
    #include <stdio.h>
    int main(void) {
        FILE* fp = fopen("traps.mp4", "r");
        if (!fp) {
            return -1;
        }

        fclose(fp);
        return 0;
    }
    ```
    Неверный вариант:
    ```c
    #include <stdio.h>
    int main(void) {
        FILE* fp = fopen("traps.mp4", "r");
        return 0;
    }
    ```

6. Глобальные переменные использовать нельзя.</br>
    Верный вариант:
    ```c
    int main(void) {
        return 0;
    }
    ```

    Неверный вариант:
    ```c
    int a = 12;

    int main(void) {
        return 0;
    }
    ```

7. Каждая функция должна возвращать код успешности ее выполнения.</br>
    Верный вариант:
    ```c
    #include <stdio.h>
    int foo(int a) {
        if (a > 2) {
            printf("Success");
            return 0;
        }
        return 1;
    }

    int main(void) {
        if (foo(2)) {
            return -1;
        }
        return 0;
    }
    ```

    Неверный вариант:
    ```c
    #include <stdio.h>
    void foo(int a) {
        if (a > 2) {
            printf("Success");
        }
    }

    int main(void) {
        foo(2);
        return 0;
    }
    ```

8. При сравнении чисел с плавающей точкой необходимо задавать допустимую &Delta;.</br>

    Верный вариант:
    ```c
    #include <math.h>
    int equal(double a, double b, double delta) {
        if (abs(a - b) < delta) {
            return 1;
        }
        return 0;
    }

    int main(void) {
        int eq = equal(1.2, 1.3, 0.0001);
        return 0;
    }
    ```

    Неверный вариант:
    ```c
    int equal(double a, double b) {
        if (a == b) {
            return 1;
        }
        return 0;
    }

    int main(void) {
        int eq = equal(1.2, 1.3);
        return 0;
    }
    ```

9. Все передаваемые в функции переменные, которые не предполагается модифицировать, должны иметь модификатор доступа _const_.</br>
    Верный вариант:
    ```c
    #include <string.h>
    #include <stdio.h>
    int str_act(const char* str) {
        printf("%s", str);
        return 0;
    }

    int main(void) {
        str_act("Hello world");
        return 0;
    }
    ```

    Неверный вариант:
    ```c
    #include <string.h>
    #include <stdio.h>
    int str_act(char* str) {
        printf("%s", str);
        return 0;
    }

    int main(void) {
        str_act("Hello world");
        return 0;
    }
    ```

10. Использование оператора _goto_ недопустимо и его наличие означает несдачу лабораторной работы.

11. Вывод ошибки в консоль должен быть информативный. </br>
    Верный вариант:
    ```c
    #include <string.h>
    #include <stdio.h>
    int func(const char* str) {
        if (!strlen(str)) {
            return -1;
        }
        printf("%s", str);
        return 0;
    }

    int main(int argc, char* argv[]) {
        if (argc != 2) {
            printf("Wrong argv count. Required 1 passed %d.\n", argc - 1);
            return -1;
        }
        int res = str_act("Hello world");
        if (res) {
            printf("Error. Passed string was empty.\n");
            return -2;
        }
        return 0;
    }
    ```

    Неверный вариант:
    ```c
    #include <string.h>
    #include <stdio.h>
    int func(const char* str) {
        if (!strlen(str)) {
            return -1;
        }
        printf("%s", str);
        return 0;
    }

    int main(int argc, char* argv[]) {
        if (argc != 2) {
            printf("ERROR.\n");
            return -1;
        }
        int res = str_act("Hello world");
        if (res) {
            printf("ERROR.\n");
            return -2;
        }
        return 0;
    }
    ```
12. Запрещены вложенные тренарные операторы. </br>
    Верный вариант:
    ```c
    int main(void)
    {
        ...
        char delimiter;
        if (space) {
            delimiter = ' ';
        } else if (comma) {
            delimiter = ',';
        } else {
            delimiter = '\n';
        }
        ...
    }
    ```
    Неверный вариант:
    ```c
    int main(void)
    {
        ...
        char delimiter = space ? ' ' : comma ? ',' : '\n';
        ...
    }
    ```

13. Условные операторы и циклы должны иметь тело, обрамленное фигурными скобками. </br>
    Верный вариант:
    ```c
    int main(void) 
    {
        ...
        if (cond) {
            printf("Foo");
        }

        while(str[i] != '\0') {
            i++;
        }
        ...
    }
    ```
    Неверный вариант:
    ```c
    int main(void) 
    {
        ...
        if (cond) printf("Foo");

        while(str[i++] != '\0');
        ...
    }
    ```

14. Название структуры должно отличаться от ее псевдонима. </br>
    Верный вариант:
    ```c
    typedef struct _Node {
        int val;
        struct _Node * children;
    } Node;
    ```
    Неверный вариант:
    ```c
    typedef struct Node {
        int val;
        struct Node * children;
    } Node;
    ```