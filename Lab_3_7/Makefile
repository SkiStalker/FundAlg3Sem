CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=3_7.c# Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=../test_templates# Это директория с шаблонами тестов (если вы их разместите в той же директории что и лабы пишите ./)
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
SUCCESS_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ofile
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd
FAILED_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ofile
SUCCESS_RFILE_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_cmd
SUCCESS_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_ofile
SUCCESS_IFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ifile_ofile
SUCCESS_IFILE_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ifile_rfile_ofile

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



# В случае если ввод в консоль программы осуществляется из подготовленного файла а вывод у программы многострочный:
# 	1) Успешный с вводом из файла, выводом в консоль (вывод сравнивается со строками из файла) - $(SUCCESS_IFILE_OFILE)
# 	2) Успешный с водом из файла, выводом в консоль и выходной файл (вывод сравнивается со строками из файла, а выходной файл с подготовленным файлом) - $(SUCCESS_IFILE_RFILE_OFILE)
# 	3) Провальный с вводом из файла, выводом в консоль (вывод сравнивается со строками из файла) - $(FAILED_IFILE_OFILE)
#

#
# Ниже приведены примеры тестов. В выводе после теста должна быть краткая информация о том что это был за тест. "TEST N PASSED" это не информативно и принято не будет.
# Символ @ необходимо писать для подавления лишнего вывода в консоль при выполнении make файла.
#

test_input_output_success_1:
	@$(SUCCESS_IFILE_RFILE_OFILE) "3_7.a" "input_stdin.txt" "weqrewfgvca.txt" "qwerty.txt" "qwerty1.txt"
	@echo "TEST 1 PASSED"


#test_command_success_ofile_1:
#	@$(SUCCESS_OFILE) "2_10.a" "output_for_2_10.txt"
#	@echo "TEST 1 PASSED"

#test_command_args_2:
#	@$(FAILED_CMD) "task_1_3.a -a" -1 "NOT EXISTING FLAG - -a"
#	@echo "TEST 2 PASSED. we enter a flag that does not exist"

#test_command_args_3:
#	@$(FAILED_CMD) "task_1_3.a -q 0.0001 2 3" 2 "The wrong amount of arguments! Must be 4 for -q key!"
#	@echo "TEST 3 PASSED. entering the wrong number of arguments"
#
#test_command_args_2:
#	@$(FAILED_CMD) "1_9.a a a" 1 "The argument must be a number!"
#	@echo "TEST 2 PASSED. we enter a string that is not a number"
#
#test_command_args_3:
#	@$(FAILED_CMD) "1_9.a 3 2" 1 "Error: a should be less than or equal to b."
#	@echo "TEST 3 PASSED. a should be less than or equal to b."

#test_command_success_ofile_1:
#	@$(SUCCESS_OFILE) "task_1_3.a -q 0.0001 2 3 4" "output_for_task_1_3.txt"
#	@echo "TEST 5 PASSED. we enter the number at which it works"
#
#test_command_args_5:
#	@$(FAILED_CMD) "task_1_3.a -m 1" 2 "The wrong amount of arguments! Must be 2 for -m key!"
#	@echo "TEST 6 PASSED. entering the wrong number of arguments"
#
#test_command_args_6:
#	@$(FAILED_CMD) "task_1_3.a -m a a" 11 "The argument must be a number!"
#	@echo "TEST 7 PASSED. we enter a string that is not a number"
#
#test_command_success_1:
#	@$(SUCCESS_CMD) "task_1_3.a -m 4 2" "The first number is a multiple of the second."
#	@echo "TEST 8 PASSED. we enter the number at which it works"
#
#test_command_args_7:
#	@$(FAILED_CMD) "task_1_3.a -m 0 0" 13 "Must be a non_zero integer"
#	@echo "TEST 9 PASSED. we enter a string that is not a number"
#
#test_command_args_8:
#	@$(FAILED_CMD) "task_1_3.a -t 0.001 1 2" 2 "The wrong amount of arguments! Must be 4 for -t key!"
#	@echo "TEST 10 PASSED. entering the wrong number of arguments"
#
#test_command_args_9:
#	@$(FAILED_CMD) "task_1_3.a -t 0.001 a a a" 11 "The argument must be a number!"
#	@echo "TEST 11 PASSED. we enter a string that is not a number"
#
#test_command_success_2:
#	@$(SUCCESS_CMD) "task_1_3.a -t 0.001 1 2 3" "It can't be a rectangular."
#	@echo "TEST 12 PASSED. we enter the number at which it works"


#test_command_args_ofile_1:
#	@$(FAILED_OFILE) "task_1.a" -1 "./failed_output_file_for_task_1.txt"
#	@echo "TEST 2 PASSED"
#
#test_command_success_1:
#	@$(SUCCESS_CMD) "task_1.a 12" "12"
#	@echo "TEST 3 PASSED"
#
#test_command_success_ofile_1:
#	@$(SUCCESS_OFILE) "1_2.a 0.000001" "output_for_task_1.txt"
#	@echo "TEST 4 PASSED"

#test_command_args_2:
#	@$(FAILED_CMD) "task_2.a" 2 "Incorrect args count"
#	@echo "TEST 5 PASSED"
#
#test_success_task_2:
#	@$(SUCCESS_RFILE_CMD) "task_2.a ./input_file_for_task_2.txt ./output_file_for_task_2.txt" "" "./output_file_for_task_2.txt" "expected_data_for_task_2.txt"
#	@echo "TEST 6 PASSED"


test: test_input_output_success_1# Здесь перечисляются все написанные вами тесты. Запуск тестов производится по команде make test



