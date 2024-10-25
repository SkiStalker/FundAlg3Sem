CC=gcc
CFLAGS=-Wall -Werror -Wextra

SOURCES=7.c
EXECUTABLES=$(SOURCES:.c=.a)
TEST_TEMPLATES_DIR=../../test_templates
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd

.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -r -f *.a

test_1: test_invalid_input_1 test_invalid_input_2 test_invalid_input_3 test_invalid_input_4 test_interval_failure test_no_argument

test_2: test_valid_case_1 test_valid_case_2 test_valid_case_3 test_valid_case_4 test_valid_case_5

# Тесты для неправильных аргументов
test_invalid_input_1:
	@$(FAILED_CMD) "7.a 1 10.g 2 0.000001" 1 "Неверное значение для нижней границы a."
	@echo "TEST 1: Invalid input for lower bound 'a' - Expected failure."

test_invalid_input_2:
	@$(FAILED_CMD) "7.a 1 0 44f 0.000001" 1 "Неверное значение для верхней границы b. Убедитесь, что a < b."
	@echo "TEST 2: Invalid input for upper bound 'b' - Expected failure."

test_invalid_input_3:
	@$(FAILED_CMD) "7.a 1 0 2 0.1f" 1 "Неверное значение для точности. Точность должна быть числом от 0 до 1."
	@echo "TEST 3: Invalid input for epsilon - Expected failure."

test_invalid_input_4:
	@$(FAILED_CMD) "7.a 1 2 2 0.000001" 1 "Неверное значение для верхней границы b. Убедитесь, что a < b."
	@echo "TEST 4: Invalid case where a >= b - Expected failure."

test_interval_failure:
	@$(FAILED_CMD) "7.a 2 0 2 0.000001" 1 "Функция не меняет знак на концах интервала [0.000000, 2.000000]. Не удалось найти корень на указанном интервале."
	@echo "TEST 4: Invalid interval fo sin(x) - Expected failure."

test_no_argument:
	@$(FAILED_CMD) "7.a" 1 "Использование: ./a.out <номер функции> <нижняя граница a> <верхняя граница b> <точность epsilon>"
	@echo "TEST 5: Not enough arguments - Expected failure."

# Тесты для правильной работы программы
test_valid_case_1:
	@$(SUCCESS_CMD) "7.a 1 0 2 0.000001" "Корень уравнения на интервале [0.000000, 2.000000] с точностью 0.000001: 1.414214"
	@echo "TEST 6: Root of x^2 - 2 with interval [0, 2] and epsilon 0.000001 - Expected success."

test_valid_case_2:
	@$(SUCCESS_CMD) "7.a 2 -1 3 0.000001" "Корень уравнения на интервале [-1.000000, 3.000000] с точностью 0.000001: 0.000000"
	@echo "TEST 7: Root of sin(x) with interval [-1, 3] and epsilon 0.000001 - Expected success."

test_valid_case_3:
	@$(SUCCESS_CMD) "7.a 5 0 1 0.000001" "Корень уравнения на интервале [0.000000, 1.000000] с точностью 0.000001: 0.739085"
	@echo "TEST 8: Root of cos(x) - x with interval [0, 1] and epsilon 0.000001 - Expected success."

test_valid_case_4:
	@$(SUCCESS_CMD) "7.a 3 1 2 0.000001" "Корень уравнения на интервале [1.000000, 2.000000] с точностью 0.000001: 1.834243"
	@echo "TEST 9: Root of x^3 - 5x + 3 with interval [1, 2] and epsilon 0.000001 - Expected success."

test_valid_case_5:
	@$(SUCCESS_CMD) "7.a 4 0 2 0.000001" "Корень уравнения на интервале [0.000000, 2.000000] с точностью 0.000001: 1.098613"
	@echo "TEST 10: Root of e^x - 3 with interval [0, 2] and epsilon 0.000001 - Expected success."

test: test_1 test_2
