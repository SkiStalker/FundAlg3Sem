CC=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task2.c
TEST_TEMPLATES_DIR=../test_templates
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash

SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd

SUCCESS_RFILE_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_cmd

SUCCESS_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ofile
FAILED_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ofile

SUCCESS_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_ofile

SUCCESS_IFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ifile_ofile
SUCCESS_IFILE_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ifile_rfile_ofile
FAILED_IFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ifile_ofile
FAILED_IFILE_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ifile_rfile_ofile

.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< $(CLIBS) -o $@

clean:
	rm -r -f *.a

# вводим мы самостоятельно в прогу, поэтому нет тестов на input error, при этом в самой проге это ошибка обрабатывается

test_command_success_ofile_1:
	@$(SUCCESS_OFILE) "task2.a" "output3_2.txt"
	@echo "TEST 1 PASSED. we enter vectors at which it works"

# test_command_success_ofile_2:
# 	@$(SUCCESS_OFILE) "task5.a 0.001 0.3" "output_test6.txt"
# 	@echo "TEST 6 PASSED. we enter eps and x at which it works"

# test_command_success_ofile_3:
# 	@$(SUCCESS_OFILE) "task5.a 0.001 0.4" "output_test7.txt"
# 	@echo "TEST 7 PASSED. we enter eps and x at which it works"

# test_command_fail_ofile_1:
# 	@$(FAILED_OFILE) "task5.a 0.001 1" 4 "output_test8.txt"
# 	@echo "TEST 8 PASSED. we enter eps and x which is ok for 1 and 2 equations, but too big for 3 and 4 equations"

test: test_command_success_ofile_1 