CC=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task3.c
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

test_command_args_1:
	@$(FAILED_CMD) "task3.a" 1 "You need to enter 3 arguments (file_in)(flag(-a /a -d /d))(file_out)"
	@echo "TEST 1 PASSED. we enter without arguments"

test_command_args_2:
	@$(FAILED_CMD) "task3.a nepravilniyvvod" 1 "You need to enter 3 arguments (file_in)(flag(-a /a -d /d))(file_out)"
	@echo "TEST 2 PASSED. we enter a string instead of a file name"

test_command_args_3:
	@$(FAILED_CMD) "task3.a input.txt -a input.txt " 1 "You need to enter 3 arguments (file_in)(flag(-a /a -d /d))(file_out)"
	@echo "TEST 3 PASSED. we enter the same file instead of two different ones"

test_command_args_4:
	@$(FAILED_CMD) "task3.a input.txt -s output.txt" 1 "You need to enter 3 arguments (file_in)(flag(-a /a -d /d))(file_out)"
	@echo "TEST 4 PASSED. we enter a non-existent flag"

test_command_success_ofile_1:
	@./task3.a input.txt -a output.txt
	@diff output.txt flag_a_out.txt
	@echo "TEST 5 PASSED. we enter correct input and check output with flag a"

test_command_success_ofile_2:
	@./task3.a input.txt -d output.txt
	@diff output.txt flag_d_out.txt
	@echo "TEST 6 PASSED. we enter correct input and check output with flag d"

test: test_command_args_1 test_command_args_2 test_command_args_3 test_command_args_4 test_command_success_ofile_1 test_command_success_ofile_2