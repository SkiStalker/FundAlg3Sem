CC=gcc
CFLAGS=-Wall -Werror -Wextra

SOURCES=9.c
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

test_1: test_invalid_base test_base_too_low test_invalid_fraction test_invalid_fraction1 test_missing_arguments

test_2: test_valid_finite test_valid_finite1 test_valid_finite2 test_valid_infinite test_valid_infinite1 test_valid_infinite2

# Тесты для неправильных аргументов
test_invalid_base:
	@$(FAILED_CMD) "9.a 10.f 0.5" 1 "Ошибка: основание должно быть положительным целым числом."
	@echo "TEST 1: Invalid base - Expected failure."

test_base_too_low:
	@$(FAILED_CMD) "9.a 1 0.5" 1 "Ошибка: основание должно быть целым числом больше 1."
	@echo "TEST 2: Base too low - Expected failure."

test_invalid_fraction:
	@$(FAILED_CMD) "9.a 10 4" 1 "Ошибка: '4' не является допустимым числом."
	@echo "TEST 3: Invalid fraction input - Expected failure."

test_invalid_fraction1:
	@$(FAILED_CMD) "9.a 10 0." 1 "Ошибка: '0.' не является допустимым числом."
	@echo "TEST 3: Invalid fraction input - Expected failure."

test_missing_arguments:
	@$(FAILED_CMD) "9.a 10" 1 "Использование: ./a.out <основание> <дробь1> <дробь2> ..."
	@echo "TEST 4: Missing fraction argument - Expected failure."

# Тесты для правильной работы программы
test_valid_finite:
	@$(SUCCESS_CMD) "9.a 10 0.5" "Число 0.5 имеет конечное представление в системе с основанием 10."
	@echo "TEST 5: Finite representation for 0.5 in base 10 - Expected success."

test_valid_finite1:
	@$(SUCCESS_CMD) "9.a 2 0.25" "Число 0.25 имеет конечное представление в системе с основанием 2."
	@echo "TEST 6: Finite representation for 0.25 in base 2 - Expected success."

test_valid_finite2:
	@$(SUCCESS_CMD) "9.a 16 0.25" "Число 0.25 имеет конечное представление в системе с основанием 16."
	@echo "TEST 6: Finite representation for 0.25 in base 16 - Expected success."

test_valid_infinite:
	@$(SUCCESS_CMD) "9.a 10 0.3333" "Число 0.3333 не имеет конечное представление в системе с основанием 10."
	@echo "TEST 8: Infinite representation for 0.3333 in base 10 - Expected success."


test_valid_infinite1:
	@$(SUCCESS_CMD) "9.a 2 0.1" "Число 0.1 не имеет конечное представление в системе с основанием 2."
	@echo "TEST 9: Infinite representation for 0.1 in base 2 - Expected success."

test_valid_infinite2:
	@$(SUCCESS_CMD) "9.a 16 0.1" "Число 0.1 не имеет конечное представление в системе с основанием 16."
	@echo "TEST 10: Infinite representation for 0.1 in base 16 - Expected success."

test: test_1 test_2
