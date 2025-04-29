# Лабораторная работа №2

## Общие требования

- Все задания реализуются на языке программирования C++ (стандарт C++14 и выше). Реализованные в заданиях
  приложения не должны завершаться аварийно.
- Запрещено использование глобальных переменных (включая `errno`).
- Запрещено использование оператора безусловного перехода (`goto`).
- Запрещено использование функций, завершающих выполнение программы вне `main`.
- При реализации необходимо разделять обработку данных и ввод/вывод.
- Данные, вводимые пользователем, должны проверяться на корректность.
- Необходимо корректно освобождать всю выделенную динамическую память.
- Все ошибки, связанные с операциями ввода-вывода, должны быть обработаны.
- Сравнение вещественных чисел должно учитывать значение `epsilon`.
- Код должен соответствовать принципам SOLID.
- Запрещено использование функций стандартной библиотеки языка C, если есть аналоги в C++.
- Оформление кода должно соответствовать `clang-format`.
- Код должен быть проверен с использованием `cppcheck`.

## О сборке

1. Каждое задание должно находиться в отдельной папке.
2. Проект должен собираться с использованием CMake (версии 3.10 и выше).
3. Структура проекта должна содержать директории:
    - `src/` — исходный код
    - `include/` — заголовочные файлы
    - `tests/` — unit-тесты
    - `build/` — директория для сборки (добавить в `.gitignore`)
4. Основной `CMakeLists.txt` должен:
    - Определять исполняемый файл лабораторной работы
    - Подключать Google Test
    - Создавать отдельную цель для тестов
    - Включать тесты с помощью `enable_testing()` и `add_test()`
    - Поддерживать покрытие тестов с `gcov` и `lcov`
    - Проверять код с `cppcheck`
5. Код должен компилироваться без предупреждений и ошибок при использовании флагов:
   `-Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=leak`
6. Все зависимости должны загружаться автоматически.

## Задание 1. Реализация собственного большого числа (BigInt)

### Требования:

- Реализовать класс `BigInt` для работы с произвольными длинными числами.
- Поддерживать сложение, вычитание, умножение, деление, сравнение (`==, !=, <, >, <=, >=`).
- Класс должен поддерживать ввод и вывод через потоки (`operator<<` и `operator>>`) парсинг как из long long, так и из строки.
- Реализовать перемещение (`move`) и копирование (`copy`).
- Написать юнит-тесты для всех операций.

## Задание 2. Быстрое возведение в степень по модулю

### Требования:

- Реализовать функцию `BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod)`, которая выполняет возведение `base` в степень `exp` по модулю `mod`.
- Использовать алгоритм быстрого возведения в степень `O(log N)`.
- Покрыть реализацию юнит-тестами.

## Задание 3. Умножение с использованием быстрого преобразования Фурье (FFT)

### Требования:

- Реализовать умножение больших чисел с использованием быстрого преобразования Фурье.
- Операция должна работать быстрее, чем `O(N^2)`.
- Покрыть реализацию юнит-тестами.

## Задание 4. Умножение по методу Карацубы

### Требования:

- Реализовать умножение больших чисел методом Карацубы (`O(N^(log2(3)))`).
- Использовать рекурсивный подход.
- Покрыть реализацию юнит-тестами.

## Задание 5. Деление методом Ньютона

### Требования:

- Реализовать деление больших чисел методом Ньютона (`O(N log N)`).
- Использовать итеративный метод приближения.
- Покрыть реализацию юнит-тестами.

---

### **Рекомендации:**
- Оптимизировать код с учетом кэширования и многопоточности.
- При тестировании использовать `EXPECT_EQ`, `EXPECT_NE`, `EXPECT_LT`, `EXPECT_GT`.
- Минимизировать дублирование кода с помощью фикстур (`TEST_F`).
- Добиться покрытия кода тестами не менее **99%** (`gcov`, `lcov`).


### **Пример hpp файла**

``` c++
#pragma once
#include <iostream>
#include <vector>

class BigInt {
public:
    BigInt();
    BigInt(long long value);
    BigInt(const std::string& str);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    ~BigInt();

    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;

    BigInt operator+=(const BigInt& other);
    BigInt operator-=(const BigInt& other);
    BigInt operator*=(const BigInt& other);
    BigInt operator/=(const BigInt& other);
    BigInt operator++();
    BigInt operator--();

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt mod_exp(const BigInt& exp, const BigInt& mod) const;
    BigInt fft_multiply(const BigInt& a) const;
    BigInt karatsuba_multiply(const BigInt& a) const;
    BigInt newton_divide(const BigInt& a) const;


    friend std::istream& operator>>(std::istream& is, BigInt& num);
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
private:
    std::vector<unsigned long long> digits;
    bool isNegative;
};

```