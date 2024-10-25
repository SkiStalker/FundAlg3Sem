CC=gcc
CFLAGS=-Wall -Werror -Wextra

SOURCES=10.c
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

test_1: test_invalid_epsilon test_invalid_a test_invalid_degree test_invalid_coefficient

test_2: test_valid_reexpansion test_valid_reexpansion1 test_valid_reexpansion2

# Тесты для неправильных аргументов
test_invalid_epsilon:
	@$(FAILED_CMD) "10.a -0.1 2 2 1.0 0.5 0.2" 1 "Ошибка: epsilon должно быть числом > 0 и <= 1."
	@echo "TEST 1: Invalid epsilon - Expected failure."

test_invalid_a:
	@$(FAILED_CMD) "10.a 0.1 16.f 2 1.0 0.5 0.2" 1 "Ошибка: a должно быть числом <= 1e10."
	@echo "TEST 2: Invalid a value - Expected failure."

test_invalid_degree:
	@$(FAILED_CMD) "10.a 0.1 2 1" 1 "Ошибка: степень должна быть положительным целым числом, и должно быть достаточно коэффициентов."
	@echo "TEST 3: Invalid degree or insufficient coefficients - Expected failure."

test_invalid_coefficient:
	@$(FAILED_CMD) "10.a 0.1 2 1.0 0.5 abc" 1 "Ошибка: степень должна быть положительным целым числом, и должно быть достаточно коэффициентов."
	@echo "TEST 4: Invalid coefficient - Expected failure."

# Тесты для правильной работы программы
test_valid_reexpansion:
	@$(SUCCESS_CMD) "10.a 0.001 2 2 1.0 0.5 0.2" "Новые коэффициенты многочлена в степенях (x - a): g0 = 2.4 g1 = 0.9 g2 = 0.2 "
	@echo "TEST 5: Valid polynomial re-expansion - Expected success."

test_valid_reexpansion1:
	@$(SUCCESS_CMD) "10.a 0.001 55 1 7.0 0.12 0.234" "Новые коэффициенты многочлена в степенях (x - a): g0 = 13.6 g1 = 0.12 "
	@echo "TEST 6: Valid polynomial re-expansion - Expected success."

test_valid_reexpansion2:
	@$(SUCCESS_CMD) "10.a 0.001 14 2 1.0 0.5 0.2 14.5" "Новые коэффициенты многочлена в степенях (x - a): g0 = 27.6 g1 = 3.3 g2 = 0.2 "
	@echo "TEST 7: Valid polynomial re-expansion - Expected success."

test: test_1 test_2
