CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=lb1task4.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=/mnt/c/Users/Вероника/Documents/GitHub/FundAlg3Sem/test_templates
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
SUCCESS_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ofile
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd
FAILED_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ofile
SUCCESS_RFILE_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_cmd
SUCCESS_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_ofile


.PHONY: all test clean

all: $(EXECUTABLES) # Чтобы собрать все лабы надо ввести команду make

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean: # Чтобы удалить все исполняемые файлы надо ввести команду make clean
	rm -r -f *.a




# Все тесты разделены на категории по результату работы программы:
#
# В случае если вывод в консоль у программы однострочный:
# 1) Успешный с выводом в консоль (вывод сравнивается со строкой) - $(SUCCESS_CMD)
# 2) Провальный с выводом в консоль (вывод сравнивается со строкой) - $(FAILED_CMD)
# 3) Успешный с выводом в консоль и в файл (вывод сравнивается со строкой, а выходной файл с подготовленным файлом) - $(SUCCESS_RFILE_CMD)
#
# В случае если вывод в консоль у программы многострочный:
# 1) Успешный с выводом в консоль (вывод сравнивается со строками из файла) - $(SUCCESS_OFILE)
# 2) Провальный с выводом в консоль (вывод сравнивается со строками из файла) - $(FAILED_OFILE)
# 3) Успешный с выводом в консоль и в файл (вывод сравнивается со строками из файла, а выходной файл с подготовленным файлом) - $(SUCCESS_RFILE_OFILE)



# Использование тестов:
#
# 1) $(SUCCESS_CMD) "<вызов исполняемого файла с его аргументами>" "<ожидаемый в консоль вывод>"
# 2) $(FAILED_CMD) "<вызов исполняемого файла с его аргументами>" "<ожидаемый код возврата отличный от 0>" "<ожидаемый в консоль вывод>"
# 3) $(SUCCESS_RFILE_CMD) "<вызов исполняемого файла с его аргументами>" "<ожидаемый в консоль вывод>" "<выходной файл>" "<файл с которым должен совпадать выходной файл>"
#
# 1) $(SUCCESS_OFILE) "<вызов исполняемого файла с его аргументами>" "<файл с ожидаемым в консоль выводом>"
# 2) $(FAIED_OFILE) "<вызов исполняемого файла с его аргументами>" "<ожидаемый код возврата отличный от 0>" "<файл с ожидаемым в консоль выводом>"
# 3) $(SUCCESS_RFILE_OFILE) "<вызов исполняемого файла с его аргументами>" "<файл с ожидаемым в консоль выводом>" "<выходной файл>" "<файл с которым должен совпадать выходной файл>"



#
# Ниже приведены примеры тестов. В выводе после теста должна быть краткая информация о том что это был за тест. "TEST N PASSED" это не информативно и принято не будет.
# Символ @ необходимо писать для подавления лишнего вывода в консоль при выполнении make файла.
#
test_task_1: test_command_no_args_4 test_command_unknown_flag_4
test_task_2: test_command_valid_count_alphabetic_4

test_command_no_args_4:
	@$(FAILED_CMD) "lb1task4.a" 3 "Error: Invalid arguments provided."
	@echo "TEST 1: No arguments provided - Expected failure. Test PASSED for lb1task4.a"

test_command_unknown_flag_4:
	@$(FAILED_CMD) "lb1task4.a -x input.txt" 4 "Error: Unknown flag provided."
	@echo "TEST 2: Unknown flag provided - Expected failure. Test PASSED for lb1task4.a"

test_command_valid_count_alphabetic_4:
	@$(SUCCESS_IFILE_RFILE_OFILE) "lb1task4.a -i input1.txt" "input1.txt" "answ.txt" "out_input1.txt" "a_input1.txt"
	@echo "TEST 3: Count alphabetic characters - Expected success. Test PASSED for lb1task4.a"

test_command_valid_count_special_4:
	@$(SUCCESS_OFILE) "lb1task4.a -s input2.txt" "Expected output for special character count"
	@echo "TEST 4: Count special characters - Expected success. Test PASSED for lb1task4.a"

test_command_valid_exclude_digits_4:
	@$(SUCCESS_OFILE) "lb1task4.a -d input.txt" "Expected output without digits"
	@echo "TEST 5: Exclude digits - Expected success. Test PASSED for lb1task4.a"

test_command_valid_hex_representation_4:
	@$(SUCCESS_RFILE_OFILE) "lb1task4.a -a input3.txt" "Expected output in hex" "output.hex" "Expected output file comparison"
	@echo "TEST 6: Hexadecimal representation - Expected success. Test PASSED for lb1task4.a"

test_command_valid_output_file_4:
	@$(SUCCESS_RFILE_OFILE) "lb1task4.a -nd output.txt input.txt" "Expected output for specified file" "output.txt" "Expected output file comparison"
	@echo "TEST 7: Valid output file specified - Expected success. Test PASSED for lb1task4.a"

test_command_invalid_input_file_4:
	@$(FAILED_CMD) "lb1task4.a -i invalid.txt" "Could not open the input file"
	@echo "TEST 8: Invalid input file - Expected failure. Test PASSED for lb1task4.a"

test_command_memory_allocation_failure_4:
	@$(FAILED_CMD) "lb1task4.a -i memory_fail.txt" "Memory allocation failed"
	@echo "TEST 9: Memory allocation failure simulation - Expected failure. Test PASSED for lb1task4.a"


test: test_task_1 test_task_2