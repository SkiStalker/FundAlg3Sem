# Сборка, тестирование и анализ покрытия кода с помощью CMakeLists.txt

## Подготовка
[CMakeLists.txt файл](examples/CMake/Lab1/CMakeLists.txt) должен быть скопирован в корень директории пункта лабораторной работы. Структура пункта лабораторной работы должна быть следующей:
1. Папка include с вашими .h и .hpp файлами;
2. Папка source с вашими .cpp файлами;
3. Папка tests с единственным файлом .cpp, который будет содержать ваши тесты (его название не принципиально).

Необходимо установить необходимые пакеты с помощью команды:
<br>
Для систем с менеджером пакетов apt:
```bash
sudo apt install lcov cppcheck libgtest-dev cmake
```
Для систем с другими менеджерами пакетов:
<br>
Идти гуглить соответствующие пакеты

## Настройка, сборка и анализ покрытия
1. Вы перемещаетесь в терминале в корень пункта лабораторной работы;
2. Настраиваете проект:
    ```bash
    cmake -DCMAKE_BUILD_TYPE:STRING=Debug -S./ -B./build
    ```
    Флаг "-DCMAKE_BUILD_TYPE:STRING=Debug" нужен для включения отладочной информации в сборку. Без него coverage работать не будет;
3. Собираете проект:
    ```bash
    cmake --build build -j$(nproc)
    ```
4. Перемещаетесь в директорию с собранным исполняемым файлом тестов:
    ```bash
    cd build
    ```
5. Запускаете тесты:
    ```bash
    ./tests
    ```
6. Убеждаетесь что все тесты прошли успешно;
7. Проверяете код с помощью cppcheck:
    ```bash
    cmake --build . --target cppcheck -j$(nproc)
    ```
8. Генерируете анализ покрытия тестов:
    ```bash
    cmake --build . --target coverage -j$(nproc)
    ```
9. Проверяете что в папке coverage_report появился файл index.html:
    ```bash
    cat coverage_report/index.html > /dev/null
    ```

## Использование совместно с VSCode
При установке плагинов:
1. [CMake Language Support](https://marketplace.visualstudio.com/items?itemName=josetr.cmake-language-support-vscode)
2. [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
3. [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)

На боковой панели появится значок CMake, перейдя на который будет необходимо настроить первичное окружение: вам предложат настроить набор утилит и на автомате подтянется актуальный GCC.<br>
Дальнейшие действия:
1. Нажать на пункт `Сборка`, убедившись что стоит цель `All`;
2. Нажать на пункт `Запуск`;
3. В пункте `Сборка` заменить цель на `coverage`;
4. Нажать пункт `Сборка`.

После данных в корне открытой в VSCode директории вы получите аналогичную предыдущему пункту папку build.