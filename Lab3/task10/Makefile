CC=gcc
CFLAGS=-Wall -Werror -Wextra

SOURCES=10.c
EXECUTABLES=$(SOURCES:.c=.a)
TEST_TEMPLATES_DIR=../../test_templates
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd

# Папки для файлов
INPUT_DIR=./input_files
OUTPUT_DIR=./output_files
EXPECTED_OUTPUT_DIR=./expected_outputs

.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -r -f *.a $(OUTPUT_DIR)/*.txt

test_1: test_invalid_input_1 test_invalid_input_2 test_invalid_input_3

test_2: test_valid_case_1 test_valid_case_2 test_valid_case_3

test_invalid_input_1:
	@$(FAILED_CMD) "./10.a" 1 "Использование: ./a.out <входной_файл> <выходной_файл>"
	@echo "TEST 1: Not enough arguments - Expected failure."

test_invalid_input_2:
	@$(FAILED_CMD) "./10.a $(INPUT_DIR)/input.txt" 1 "Использование: ./a.out <входной_файл> <выходной_файл>"
	@echo "TEST 2: Missing output file argument - Expected failure."

test_invalid_input_3:
	@$(FAILED_CMD) "./10.a $(INPUT_DIR)/no_file.txt $(OUTPUT_DIR)/output.txt" 1 "Ошибка при открытии входного файла"
	@echo "TEST 3: Non-existing input file - Expected failure."

test_valid_case_1:
	@./10.a $(INPUT_DIR)/rfile1.txt $(OUTPUT_DIR)/ofile1.txt
	@echo "TEST 4: Comparing output files..."
	@diff -u $(OUTPUT_DIR)/ofile1.txt $(EXPECTED_OUTPUT_DIR)/expected_ofile1.txt || echo "TEST 4: Files differ. Test failed."
	@echo "TEST 4: Files match. Test passed."

test_valid_case_2:
	@./10.a $(INPUT_DIR)/rfile2.txt $(OUTPUT_DIR)/ofile2.txt
	@echo "TEST 5: Comparing output files..."
	@diff -u $(OUTPUT_DIR)/ofile2.txt $(EXPECTED_OUTPUT_DIR)/expected_ofile2.txt || echo "TEST 5: Files differ. Test failed."
	@echo "TEST 5.1: Files match. Test passed."

test_valid_case_3:
	@./10.a $(INPUT_DIR)/rfile3.txt $(OUTPUT_DIR)/ofile3.txt
	@echo "TEST 6: Comparing output files..."
	@diff -u $(OUTPUT_DIR)/ofile3.txt $(EXPECTED_OUTPUT_DIR)/expected_ofile3.txt || echo "TEST 6: Files differ. Test failed."
	@echo "TEST 6.1: Files match. Test passed."

test: test_1 test_2
