CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=task9_1.c# Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=../test_templates# Это директория с шаблонами тестов (если вы их разместите в той же директории что и лабы пишите ./)
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

test_command_args_9_1:
	@$(FAILED_CMD) "task9_1.a" 1 "Wrong amount of arguments!"
	@echo "TASK 9: TEST WITH NO ARGUMENTS PASSED"


test_failed_task_9_1:
	@$(FAILED_CMD) "task9_1.a -10 100 qwerty" 2 "Invalid input"
	@echo "TASK 9: TEST WITH WRONG AMOUNT OF ARGUMENTS PASSED"

test_failed_1_task_9_1:
	@$(FAILED_CMD) "task9_1.a -10 100 20" 2 "Invalid input: array size must be positive and min_value should be less than max_value."
	@echo "TASK 9: TEST WITH WRONG ARGUMENTS PASSED"


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


test: test_command_args_9_1  test_failed_task_9_1 test_failed_1_task_9_1# Здесь перечисляются все написанные вами тесты. Запуск тестов производится по команде make test

