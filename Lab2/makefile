CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task1.c task2.c task3.c task4.c task8.c
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


test_task_1: test_command_args_1_1 test_command_args_1_2 test_command_args_1_3 test_command_args_1_4 test_command_args_1_5 test_command_args_1_6 test_command_args_1_7 test_command_args_1_8 test_command_args_1_9
	

test_command_args_1_1:
	@$(FAILED_CMD) "task1.a" -1 "ERROR: Invalid input"
	@echo "Task 1. TEST 1 PASSED. No arguments have been entered"

test_command_args_1_2:
	@$(FAILED_CMD) "task1.a -l" -1 "ERROR: Invalid input"
	@echo "Task 1. TEST 2 PASSED. A flag was entered, but not a string"

test_command_args_1_3:
	@$(FAILED_CMD) "task1.a -b abcdef" -1 "ERROR: Invalid flag"
	@echo "Task 1. TEST 3 PASSED. An unknown flag was entered"

test_command_args_1_4:
	@$(FAILED_CMD) "task1.a -djawdw dowahd wdwaojdwoajd w" -1 "ERROR: Invalid flag"
	@echo "Task 1. TEST 4 PASSED. An unknown flag was introduced and there are many arguments"


test_command_args_1_5:
	@$(SUCCESS_CMD) "task1.a -l danwdnaF13r" "String length: 11"
	@echo "Task 1. TEST 5 PASSED. Checking for the -l flag and the danwdnaF13r string"

test_command_args_1_6:
	@$(SUCCESS_CMD) "task1.a -r abc123@da" "Reversed string: ad@321cba"
	@echo "Task 1. TEST 6 PASSED. Checking for the -r flag and the abc123@da string"

test_command_args_1_7:
	@$(SUCCESS_CMD) "task1.a -u qwertyuiop" "Converted string: qWeRtYuIoP"
	@echo "Task 1. TEST 7 PASSED. Checking for the -u flag and the qwertyuiop string"

test_command_args_1_8:
	@$(SUCCESS_CMD) "task1.a -n abc123@#da" "Converted string: 123abcda@#"
	@echo "Task 1. TEST 8 PASSED. Checking for the -l flag and the danwdnaF13r string"

test_command_args_1_9:
	@$(SUCCESS_CMD) "task1.a -c 5 1aidajw1 2aodndw2 3nfanf3 4awaf4" "Concatenated string: 2aodndw21aidajw13nfanf34awaf4"
	@echo "Task 1. TEST 9 PASSED. Checking for the -c flag and submitted lines"
	@echo "\n"



test_task_2: test_command_args_ofile_2_1


test_command_args_ofile_2_1:
	@$(SUCCESS_OFILE) "task2.a" "./task2_output_file.txt"
	@echo "Task 2. TEST 1 PASSED. Check the task2_output_file.txt"
	@echo "\n"


test_task_3: test_command_args_3_1 test_command_args_3_2 test_command_args_3_3 test_command_args_3_4 test_command_args_ofile_3_5 test_command_args_ofile_3_6


test_command_args_3_1:
	@$(FAILED_CMD) "task3.a" -1 "ERROR: Invalid input"
	@echo "Task 3. TEST 1 PASSED. No arguments have been entered"

test_command_args_3_2:
	@$(FAILED_CMD) "task3.a "abc"" -1 "ERROR: Invalid input"
	@echo "Task 3. TEST 2 PASSED. Entered only a substring without files"

test_command_args_3_3:
	@$(FAILED_CMD) "task3.a  djwapdjwdowa" -1 "ERROR: Invalid input"
	@echo "Task 3. TEST 3 PASSED. An unknown string was entered"

test_command_args_3_4:
	@$(FAILED_CMD) "task3.a "abc" file.txt" -1 "ERROR: One or more files could not be opened"
	@echo "Task 3. TEST 4 PASSED. Unknown file was entered"

test_command_args_ofile_3_5:
	@$(SUCCESS_OFILE) "task3.a "abc" ./task3_input_file1.txt ./task3_input_file2.txt" "./task3_output_file1.txt"
	@echo "Task 3. TEST 5 PASSED. Entered a substring and two files where this substring is"

test_command_args_ofile_3_6:
	@$(SUCCESS_OFILE) "task3.a "abc" ./task3_input_file1.txt ./task3_input_file3.txt" "./task3_output_file2.txt"
	@echo "Task 3. TEST 6 PASSED. Entered a substring and two files, one of the files does not have a substring"
	@echo "\n"

test_task_4: test_command_args_ofile_4_1

test_command_args_ofile_4_1:
	@$(SUCCESS_OFILE) "task4.a" "./task4_output_file.txt"
	@echo "Task 4. TEST 1 PASSED. Check the task2_output_file.txt"
	@echo "\n"

test_task_8: test_command_args_ofile_8_1

test_command_args_ofile_8_1:
	@$(SUCCESS_OFILE) "task8.a" "./task8_output_file.txt"
	@echo "Task 8. TEST 1 PASSED. Check the task2_output_file.txt"


test: test_task_1 test_task_2 test_task_3 test_task_4 test_task_8
	