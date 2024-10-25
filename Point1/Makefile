CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=point1.c
EXECUTABLES=$(SOURCES:.c=.a)
TEST_TEMPLATES_DIR=./test_templates
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd.txt
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd.txt

.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -r -f *.a


test_1: test_command_args_1 test_command_args_2 test_command_args_3 test_command_args_4

test_2: test_command_success_1 test_command_success_2 test_command_success_3 test_command_success_4 test_command_success_5

# Тесты для неправильных аргументов
test_command_args_1:
	@$(FAILED_CMD) "point1.a" 1 "Usage: <flag> <string>"
	@echo "TEST 1: No arguments provided - Expected failure."

test_command_args_2:
	@$(FAILED_CMD) "point1.a -x abcdef" 1 "Unknown flag: -x"
	@echo "TEST 2: Unknown flag used - Expected failure."

test_command_args_3:
	@$(FAILED_CMD) "point1.a -l" 1 "Usage: <flag> <string>"
	@echo "TEST 3: Insufficient arguments - Expected failure."

test_command_args_4:
	@$(FAILED_CMD) "point1.a -c abc gjdkg" 1 "Error: Invalid number input"
	@echo "TEST 4: Invalid seed for concatenation - Expected failure."

# Тесты для правильной работы программы
test_command_success_1:
	@$(SUCCESS_CMD) "point1.a -l abcdef" "String length = 6"
	@echo "TEST 5: String length test - Expected success."

test_command_success_2:
	@$(SUCCESS_CMD) "point1.a -r abcdef" "Reversed string: fedcba"
	@echo "TEST 6: Reverse string test - Expected success."

test_command_success_3:
	@$(SUCCESS_CMD) "point1.a -u abcdef" "New string: aBcDeF"
	@echo "TEST 7: Uppercase odd positions test - Expected success."

test_command_success_4:
	@$(SUCCESS_CMD) "point1.a -n gdfgj439tkdsd.gf44" "New string: 43944gdfgjtkdsdgf."
	@echo "TEST 8: String with needed order test - Expected success."

test_command_success_5:
	@$(SUCCESS_CMD) "point1.a -c 15 sfoksodfk sadkadosakd rwerwer" "Concatenated string: sfoksodfkrwerwersadkadosakd"
	@echo "TEST 9: Concatenation string test - Expected success."


test: test_1 test_2