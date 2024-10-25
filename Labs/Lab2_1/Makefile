CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=Lab2_1.c
TEST_TEMPLATES_DIR=/mnt/c/Users/Алексей/CLionProjects/Zavorotnyi_M80-214--23/test_templates
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
SUCCESS_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ofile
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd
FAILED_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ofile
SUCCESS_RFILE_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_cmd
SUCCESS_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_ofile


.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -r -f *.a




test_command_args1_1:
	@$(FAILED_CMD) "./Lab2_1.a" -1 "Wrong number of arguments"
	@echo "TASK 1: TEST WITH NO ARGUMENTS PASSED"

test_command_args2_1:
	@$(FAILED_CMD) "./Lab2_1.a -q wwfr" -1 "Flag -q is not supported."
	@echo "TASK 1: TEST WITH WRONG AMOUNT OF ARGUMENTS PASSED"

test_command_args4_1:
	@$(FAILED_CMD) "./Lab2_1.a -c wrt yr hty" -1 "not a number"
	@echo "TASK 1: TEST WITH WRONG ARGUMENTS PASSED"

test_success1_1:
	@$(SUCCESS_CMD) "./Lab2_1.a -l qaz" "Length is 3."
	@echo "TASK 1: TEST -l ARGUMENT SUCCESS"

test_success2_1:
	@$(SUCCESS_CMD) "./Lab2_1.a -r qaz" "Reverse is zaq"
	@echo "TASK 1: TEST -r ARGUMENT SUCCESS"

test_success3_1:
	@$(SUCCESS_CMD) "./Lab2_1.a -u werwfj" "EveryOddToUpped is wErWfJ"
	@echo "TASK 1: TEST -u ARGUMENT SUCCESS"

test_success4_1:
	@$(SUCCESS_CMD) "./Lab2_1.a -n wt65erwf8j30" "NewOrder is 65830wterwfj"
	@echo "TASK 1: TEST -n ARGUMENT SUCCESS"

test_success5_1:
	@$(SUCCESS_CMD) "./Lab2_1.a -c tyr 1234 iyu kuy4" "New string is kuy4iyutyr"
	@echo "TASK 1: TEST -c ARGUMENT SUCCESS"

test: test_command_args1_1 test_command_args2_1 test_command_args4_1 test_success1_1 test_success2_1 test_success3_1 test_success4_1 test_success5_1