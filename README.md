# Информация для выполнения лабораторных по фундаментальным алгоритмам
Тут размещена информация необходимая для успешного выполнения лабораторных работ.

## Оглавление
1. [Важные ссылки](#важные-ссылки)
2. [Полезные ссылки](#полезные-ссылки)
3. [Общие правила](#общие-правила)
4. [Дистанционная сдача](#дистанционная-сдача)
5. [Лабораторные](#лабораторные)
    1. [№1](#1)
    2. [№2](#2)
    3. [№3](#3)
    4. [№4](#4)

## Важные ссылки
- [Таблица с результатами сдачи](https://docs.google.com/spreadsheets/d/1rtPmdYJtlkvIajOST0LYesvfwspxoClLx1zNEmBY5O8/edit?usp=sharing)

## Полезные ссылки
- [Visual Studio Code](https://code.visualstudio.com/download)
- [C Documentation](https://en.cppreference.com/w/c)
- [Ответы на все вопросы](https://yandex.ru/)

## Общие правила

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

## Дистанционная сдача
### Правила
- Сдать дистанционно можно только только через fork данного git репозитория, в нем вы создаете свою ветку с английским именем в формате <фамилия>\_<1 буква имени>\_<номер группы>;
- Для сдачи вы заводите pull request с сообщением, содержащим информацию о том кто вы и какие лабораторные хотите сдать в рамках данного PR, я в нем делаю пометки и при успешной сдаче закрываю;
- Дистанционно может быть сдано не более половины пунктов из одной лабораторной (если всего их 10 сдать можно 5);
- К каждому сдаваемому пункту должны быть приложены тесты покрывающие следующие ситуации:
    - Некорректное количество введенных аргументов командной строки;
    - Некорректный ввод аргумента командной строки (вместо 123, введено 123asdadqwe);
    - Корректный вывод программы при правильных аргументах;
    - Корректный вывод программы при неправильных аргументах (если применимо);
- Тесты должны быть в [требуемом формате](#тестирование);
- Сборка программы должна быть без ошибок (error) и предупреждений (warning).

### Тестирование
[Makefile](./examples/Lab1/Makefile) это файл, который должен быть в любой дистанционно сдаваемой лабе. Его вы берете за основу своего Makefile и редактируете необходимые пункты, а не переписываете все с нуля. </br>Рекомендуется использовать один Makefile на одну лабораторную работу, а не на каждый пункт.</br>
Внутри данного файла размещены примеры и пояснения использования [bash скриптов](./test_templates/) для тестирования. Допускается использование лишь тех bash скриптов, которые приведены в данной папке. Копировать их в папки со своими лабораторными не надо, используйте относительные пути к ним. </br>
Всего в файле 3 обязательных правила с соответствующими им командами: </br>
1. Сборка всех .cpp файлов в исполняемые файлы
    ```bash
    make
    ```

2. Очистка всех исполняемых файлов
    ```bash
    make clean
    ```

3. Выполнение тестов:
    ``` bash
    make test
    ```

### Рекомендации
- Если при выполнении <i>"make test"</i> возникает ошибка <i>"Permission denied"</i> необходимо переместиться в [директорию с тестами](./test_templates/) и применить команду;
    ```bash
    sudo chmod +x *
    ```
- Пользователям Windows установить [WSL](https://learn.microsoft.com/ru-ru/windows/wsl/install);
- При разработке тестов проанализировать содержимое тестирующих [bash скриптов](./test_templates/);
- В случае возникновения ошибки похожей на:
    ```
    ../../test_templates/failed_cmd: line 17: syntax error: unexpected end of file
    ```
    необходимо сменить вариант перевода строки с CRLF (\r\n) на LF (\n) (версия Windows на версию Linux).


## Лабораторные
### №1

[Файл](./Lab1.pdf) с заданием. </br> Дедлайн 19.10.2024

### №2

[Файл](./Lab2.pdf) с заданием. </br> Дедлайн 25.10.2024

### №3
[Файл](./Lab3.pdf) с заданием. </br> Дедлайн 08.11.2024

### №4
[Файл](./Lab4.pdf) с заданием.