CC = gcc
CFLAGS = -Wall -Werror -Wextra

SOURCES = 1.c
EXECUTABLES = $(SOURCES:.c=.a)
TEST_TEMPLATES_DIR = ../../test_templates
BASH = bash
SUCCESS_CMD = $(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
FAILED_CMD = $(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd

.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -r -f *.a

test_1: test_invalid_input_1 test_invalid_input_2 test_no_argument

test_2: test_valid_negative test_valid_positive test_zero test_large_number

# Тесты для неправильных аргументов
test_invalid_input_1:
	@$(FAILED_CMD) "1.a 1f." 1 "Ошибка: Ввод должен быть числом."
	@echo "TEST 1: Extra arguments - Expected failure."

test_invalid_input_2:
	@$(FAILED_CMD) "1.a 12 34" 1 "Ошибка: Использование: ./a.out <число>"
	@echo "TEST 2: Extra arguments - Expected failure."

test_no_argument:
	@$(FAILED_CMD) "1.a" 1 "Ошибка: Использование: ./a.out <число>"
	@echo "TEST 3: No arguments provided - Expected failure."

# Тесты для правильных значений
test_valid_negative:
	@$(SUCCESS_CMD) "1.a -15" $$'Преобразование числа -15 в разные системы счисления:\nРезультат в системе 2^1: -1 1 1 1\nРезультат в системе 2^2: -3 3\nРезультат в системе 2^3: -1 7\nРезультат в системе 2^4: -F\nРезультат в системе 2^5: -F'
	@echo "TEST 4: Convert negative number -15 - Expected success."

test_valid_positive:
	@$(SUCCESS_CMD) "1.a 10" $$'Преобразование числа 10 в разные системы счисления:\nРезультат в системе 2^1: 1 0 1 0\nРезультат в системе 2^2: 2 2\nРезультат в системе 2^3: 1 2\nРезультат в системе 2^4: A\nРезультат в системе 2^5: A'
	@echo "TEST 5: Convert positive number 10 - Expected success."

test_zero:
	@$(SUCCESS_CMD) "1.a 0" $$'Преобразование числа 0 в разные системы счисления:\nРезультат в системе 2^1: 0\nРезультат в системе 2^2: 0\nРезультат в системе 2^3: 0\nРезультат в системе 2^4: 0\nРезультат в системе 2^5: 0'
	@echo "TEST 6: Convert zero - Expected success."

test_large_number:
	@$(SUCCESS_CMD) "1.a 255" $$'Преобразование числа 255 в разные системы счисления:\nРезультат в системе 2^1: 1 1 1 1 1 1 1 1\nРезультат в системе 2^2: 3 3 3 3\nРезультат в системе 2^3: 3 7 7\nРезультат в системе 2^4: F F\nРезультат в системе 2^5: 7 V'
	@echo "TEST 7: Convert large number 255 - Expected success."

test: test_1 test_2
