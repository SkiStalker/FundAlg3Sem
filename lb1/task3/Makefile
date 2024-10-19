CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=lb1task3.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
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

test_task_1: test_command_args_1 test_command_args_2 test_command_args_3 test_command_args_4 test_command_args_5 test_command_args_6 test_command_args_7 test_command_args_8

test_task_2: test_command_success_1 test_command_success_2 
test_command_args_1:
	@$(FAILED_CMD) "lb1task3.a" 5 "Not enough arguments"
	@echo "TEST 1: No arguments provided - Expected failure. Test PASSED for lb1task3.c"

test_command_args_2:
	@$(FAILED_CMD) "lb1task3.a -a" -1 "Unknown flag"
	@echo "TEST 2: Unknown flag used - Expected failure. Test PASSED for lb1task3.c"

test_command_args_3:
	@$(FAILED_CMD) "lb1task3.a -q 0.0001 2 3" 5 "Invalid number of parameters for flag -q"
	@echo "TEST 3 PASSED: entering the wrong number of arguments"

test_command_args_4:
	@$(FAILED_CMD) "lb1task3.a -q a a a a" 5 "All parameters must be valid numbers"
	@echo "TEST 4 PASSED: we enter a string that is not a number"

test_command_success_ofile_1:
	@$(SUCCESS_OFILE) "lb1task3.a -q 0.0001 2 3 4" "output_for_task_3.txt"
	@echo "TEST 5 PASSED: we enter the number at which it works"

test_command_args_5:
	@$(FAILED_CMD) "lb1task3.a -m 1" 5 "Invalid number of parameters for flag -m"
	@echo "TEST 6 PASSED: entering the wrong number of arguments"

test_command_args_6:
	@$(FAILED_CMD) "lb1task3.a -m a a" 5 "Both parameters must be valid numbers"
	@echo "TEST 7 PASSED: we enter a string that is not a number"

test_command_success_1:
	@$(SUCCESS_CMD) "lb1task3.a -m 4 2" "Checking divisibility: 4 is multiple 2"
	@echo "TEST 8 PASSED: we enter the number at which it works"

test_command_args_7:
	@$(FAILED_CMD) "lb1task3.a -t 0.001 1 2" 5 "Invalid number of parameters for flag -t"
	@echo "TEST 9 PASSED: entering the wrong number of arguments"

test_command_args_8:
	@$(FAILED_CMD) "lb1task3.a -t 0.001 a a a" 5 "All parameters must be valid numbers"
	@echo "TEST 10 PASSED: we enter a string that is not a number"

test_command_success_2:
	@$(SUCCESS_CMD) "lb1task3.a -t 0.001 1 2 3" "Triangle check: Can't form a triangle"
	@echo "TEST 11 PASSED: we enter the number at which it works"


test: test_task_1 test_task_2 test_command_success_ofile_1