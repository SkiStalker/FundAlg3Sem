CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=lb1task9.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
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
test_task_1: test_command_no_args_1 test_command_invalid_args_2 test_command_a_greater_than_b_3 test_command_a_b_out_of_range_4 test_not_a_number_5
test_task_2: test_command_dynamic_array_success_2 test_command_success_1
test_command_no_args_1:
	@$(FAILED_CMD) "lb1task9.a" 1 "Not enough arguments"
	@echo "TEST 1: No arguments provided - Expected failure. Test PASSED."

test_command_invalid_args_2:
	@$(FAILED_CMD) "lb1task9.a 5" 1 "Not enough arguments"
	@echo "TEST 2: Invalid number of arguments - Expected failure. Test PASSED."

test_command_a_greater_than_b_3:
	@$(FAILED_CMD) "lb1task9.a 10 5" 1 "Error: a must be less than b."
	@echo "TEST 3: a >= b - Expected failure. Test PASSED."

test_command_a_b_out_of_range_4:
	@$(FAILED_CMD) "lb1task9.a -3000000000000 10" 1 "Error: a and b must be within the range of int."
	@echo "TEST 4: a and b out of range - Expected failure. Test PASSED."

test_command_success_1:
	@$(SUCCESS_CMD) "lb1task9.a 1 10" "Fixed array: " "Array after swapping min and max: "
	@echo "TEST 5: Valid parameters - Expected success. Test PASSED."

test_command_dynamic_array_success_2:
	@$(SUCCESS_CMD) "lb1task9.a 1 10" "Dynamic array C: "
	@echo "TEST 6: Dynamic array created successfully. Test PASSED."

test_not_a_number_5:
	@$(FAILED_CMD) "lb1task9.a a b" 1 "Error: a and b must be valid integers."
	@echo "TEST 7: a or b is not a number"


test: test_task_1 test_task_2