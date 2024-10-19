CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=lb2task1.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
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

test_task_1: test_command_args_1 test_command_args_2 test_command_args_3 test_command_args_4 

test_task_2: test_command_success_1 test_command_success_2 test_command_success_3 test_command_success_ofile_1 test_command_success_4
test_command_args_1:
	@$(FAILED_CMD) "lb2task1.a" 1 "Not enough arguments."
	@echo "TEST 1: No arguments provided - Expected failure."

test_command_args_2:
	@$(FAILED_CMD) "lb2task1.a -x xdjsdj" 3 "Unknown flag"
	@echo "TEST 2: Unknown flag used - Expected failure. Test PASSED for lb1task3.c"

test_command_args_3:
	@$(FAILED_CMD) "lb2task1.a -l" 1 "Not enough arguments."
	@echo "TEST 3 PASSED: entering the wrong number of arguments"

test_command_args_4:
	@$(FAILED_CMD) "lb2task1.a  -c 5 shjk kdf sdgsgsg" 4 "Error: Expected 5 strings, but got 3."
	@echo "TEST 4 PASSED: you must enter the specified number of lines"

test_command_success_ofile_1:
	@$(SUCCESS_OFILE) "lb2task1.a -u hsjdkkxlhgftstdwhgyjaklsdoifure7463wyuishdjknjvshfuryt4736tyrughdjfbknlgodieru9w4t8273ew9fusdiofbgjhlknb5y3hj4gyuwte6f7yguhjeb35tn4wbjfegdyus7vdysufrbw4tj3bwr" "output.txt"
	@echo "TEST 5 PASSED: we enter the number at which it works"

test_command_success_1:
	@$(SUCCESS_CMD) "lb2task1.a -l -14567gfdxcghj.huj" "Length of string: 18"
	@echo "TEST 6 PASSED: we enter the number at which it works"

test_command_success_2:
	@$(SUCCESS_CMD) "lb2task1.a -r 454sdfqw" "Reversed string: wqfds454"
	@echo "TEST 7 PASSED: we enter the number at which it works"

test_command_success_3:
	@$(SUCCESS_CMD) "lb2task1.a -u yxhdjsske6w7ehu" "String with uppercase at odd positions: yXhDjSsKe6w7eHu"
	@echo "TEST 8 PASSED: we enter the number at which it works"

test_command_success_4:
	@$(SUCCESS_CMD) "lb2task1.a -n fthj765.uhgj9" "String with separated characters: 7659fthjuhgj."
	@echo "TEST 9 PASSED: we enter the number at which it works"



test: test_task_1 test_task_2
