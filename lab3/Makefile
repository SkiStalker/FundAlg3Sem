CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=1.c 3.c 4.c 5.c 6.c# Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=./test_templates# Это директория с шаблонами тестов (если вы их разместите в той же директории что и лабы пишите ./)
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

all: $(EXECUTABLES) # Чтобы собрать все лабы надо ввести команду make

%.a : %.c
	$(CC) $(CFLAGS) $< $(CLIBS) -o $@

clean: # Чтобы удалить все исполняемые файлы надо ввести команду make clean
	rm -r -f *.a


# Принцип формирования названия выполняемого теста:
#
# (SUCESS|FAILED)[_IFILE][_RFILE](CMD|OFILE)
#		 1			2		3		 4
#
# 1){ОБЯЗАТЕЛЬНЫЙ} В зависимости от предполагаемого окончания теста мы выбираем либо успешный вариант - SUCCESS, либо провальный - FAILED 
# 2){ОПЦИОНАЛЬНЫЙ} При необходимости ввода в консоль программы (использвуя scanf или аналоги) добавляется суффикс _IFILE
# 3){ОПЦИОНАЛЬНЫЙ} Если программа предполагает наличие выходного файла с данными добавляется суффикс _RFILE
# 4){ОБЯЗАТЕЛЬНЫЙ} В зависимости от того однострочный или многострочный вывод в консоль ожидается у программы выбираем окончание CMD - для однострочного вывода и OFILE для многострочного
#


# Все тесты разделены на категории по результату работы программы и формату ввода:
#
# В случае если ввод в программу только с помощью аругментов командной строки:
# 	В случае если вывод в консоль у программы однострочный:
# 		1) Успешный с выводом в консоль (вывод сравнивается со строкой) - $(SUCCESS_CMD)
# 		2) Провальный с выводом в консоль (вывод сравнивается со строкой) - $(FAILED_CMD)
# 		3) Успешный с выводом в консоль и в файл (вывод сравнивается со строкой, а выходной файл с подготовленным файлом) - $(SUCCESS_RFILE_CMD)
#	
# 	В случае если вывод в консоль у программы многострочный:
# 		1) Успешный с выводом в консоль (вывод сравнивается со строками из файла) - $(SUCCESS_OFILE)
# 		2) Провальный с выводом в консоль (вывод сравнивается со строками из файла) - $(FAILED_OFILE)
# 		3) Успешный с выводом в консоль и в выходной файл (вывод сравнивается со строками из файла, а выходной файл с подготовленным файлом) - $(SUCCESS_RFILE_OFILE)
#
# В случае если ввод в консоль программы осуществляется из подготовленного файла а вывод у программы многострочный:  
# 	1) Успешный с вводом из файла, выводом в консоль (вывод сравнивается со строками из файла) - $(SUCCESS_IFILE_OFILE)
# 	2) Успешный с водом из файла, выводом в консоль и выходной файл (вывод сравнивается со строками из файла, а выходной файл с подготовленным файлом) - $(SUCCESS_IFILE_RFILE_OFILE)
# 	3) Провальный с вводом из файла, выводом в консоль (вывод сравнивается со строками из файла) - $(FAILED_IFILE_OFILE)
#


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
# 1) $(SUCCESS_IFILE_OFILE) "<вызов исполняемого файла с его аргументами>" "<файл данными которые вводятся в консоль через stdin>" "<файл с ожидаемым в консоль выводом>" 
# 2) $(SUCCESS_IFILE_RFILE_OFILE) "<вызов исполняемого файла с его аргументами>" "<файл данными которые вводятся в консоль через stdin>" "<файл с ожидаемым в консоль выводом>" "<выходной файл>" "<файл с которым должен совпадать выходной файл>"
# 3) $(FAILED_IFILE_OFILE) "<вызов исполняемого файла с его аргументами>" "<ожидаемый код возврата отличный от 0>" "<файл данными которые вводятся в консоль через stdin>" "<файл с ожидаемым в консоль выводом>"



#
# Ниже приведены примеры тестов. В выводе после теста должна быть краткая информация о том что это был за тест. "TEST N PASSED" это не информативно и принято не будет.
# Символ @ необходимо писать для подавления лишнего вывода в консоль при выполнении make файла.
#  

test_task_1: test_success_ofile_1 test_failed_ofile_1

test_success_ofile_1:
	@$(SUCCESS_IFILE_OFILE) "1.a" "task1_test/task1_input_1.txt" "task1_test/task1_output_1.txt"
	@echo "TEST 1 PASSED: проверка ввода числа 1.c"

test_failed_ofile_1:
	@$(FAILED_IFILE_OFILE) "1.a" -1 "task1_test/task1_input_2.txt" "task1_test/task1_output_2.txt"
	@echo "TEST 2 PASSED: проверка на ошибку ввода 1.c"

test_task_3: test_success_ofile_3_1 test_success_ofile_3_2 test_failed_ofile_3

test_success_ofile_3_1:
	@$(SUCCESS_RFILE_CMD) "3.a task3_test/task3_input_1.txt -a output.txt" "" "output.txt" "task3_test/task3_output_1.txt"
	@echo "TEST 3 PASSED: проверка на верный вывод 3.с"

test_success_ofile_3_2:
	@$(SUCCESS_RFILE_CMD) "3.a task3_test/task3_input_2.txt /d output.txt" "" "output.txt" "task3_test/task3_output_2.txt"
	@echo "TEST 4 PASSED: проверка на верный вывод 3.c"

test_failed_ofile_3:
	@$(FAILED_CMD) "3.a task3_test/task3_input_3.txt /d task3_test/task3_input_3.txt" -1 "ERROR: files must be different" "test"
	@echo "TEST 5 PASSED: проверка на ошибку при вводе двух одинаковых файлов 3.с"

test_task_4: test_command_args_4 test_success_command_4 test_failed_command_4

test_command_args_4:
	@$(FAILED_CMD) "4.a" -1 "Неизвестная команда. Пожалуйста, попробуйте снова." "test" 
	@echo "TEST 6 PASSED: Проверка на ввод неизвестной команды 4.c"

test_success_command_4:
	@$(SUCCESS_IFILE_OFILE) "4.a" "task4_test/success_command_input_task4.txt" "task4_test/success_command_output_task4.txt"
	@$(SUCCESS_IFILE_OFILE) "4.a" "task4_test/success_add_input_task4.txt" "task4_test/success_add_output_task4.txt"
	@$(SUCCESS_IFILE_OFILE) "4.a" "task4_test/success_delivery_input_task4.txt" "task4_test/success_delivery_output_task4.txt"
	@echo "TEST 7 PASSED: Проверка вывода всех посылок, добавление, удаление, поиск, вывод всех доставленных 4.c"

test_failed_command_4:
	@$(FAILED_IFILE_OFILE) "4.a" -1 "task4_test/failed_add_input_task4.txt" "task4_test/failed_add_output_task4.txt"
	@$(FAILED_IFILE_OFILE) "4.a" -1 "task4_test/failed_search_input_task4.txt" "task4_test/failed_search_output_task4.txt"
	@$(FAILED_IFILE_OFILE) "4.a" -1 "task4_test/failed_delete_input_task4.txt" "task4_test/failed_search_output_task4.txt"
	@echo "TEST 8 PASSED: Проверка на ошибку добавление, удаления, поиска 4.c"


test_task_5: test_command_args_5 test_success_command_5 test_failed_command_5

test_command_args_5:
	@$(FAILED_CMD) "5.a task5_test/student_list.txt" -1 "Неизвестная команда. Пожалуйста, попробуйте снова." "test" 
	@$(FAILED_CMD) "5.a" -1 "Укажите имя файла как аргумент программы" "test" 
	@echo "TEST 9 PASSED: Проверка на ввод неизвестной команды 5.c"

test_success_command_5:
	@$(SUCCESS_IFILE_OFILE) "5.a task5_test/student_list.txt" "task5_test/success_command_input_task5.txt" "task5_test/success_command_output_task5.txt"
	@$(SUCCESS_IFILE_RFILE_OFILE) "5.a task5_test/student_list.txt" "task5_test/success_fsearch_input_task5.txt" "task5_test/success_fsearch_output_task5.txt" "student.txt" "task5_test/test_student.txt"
	@$(SUCCESS_IFILE_RFILE_OFILE) "5.a task5_test/student_list.txt" "task5_test/success_better_input_task5.txt" "task5_test/success_better_output_task5.txt" "better_student.txt" "task5_test/test_better.txt"
	@echo "TEST 10 PASSED: Проверка вывода всех студентов, поиск, сортировки, файл с лучшими, вывод с найденным студентом 5.c"

test_failed_command_5:
	@$(FAILED_OFILE) "5.a task5_test/failed_file_input_task5.txt" -1 "task5_test/failed_file_output_task5.txt"
	@$(FAILED_IFILE_OFILE) "5.a task5_test/student_list.txt" -1 "task5_test/failed_search_input_task5.txt" "task5_test/failed_search_output_task5.txt"
	@$(FAILED_IFILE_OFILE) "5.a task5_test/student_list.txt" -1 "task5_test/failed_sort_input_task5.txt" "task5_test/failed_sort_output_task5.txt"
	@echo "TEST 11 PASSED: Проверка на ошибку обработки файла, поиска, сортировки 5.c"


test_task_6: test_command_args_6 test_success_command_6

test_command_args_6:
	@$(FAILED_CMD) "6.a" -1 "Неизвестная команда. Пожалуйста, попробуйте снова." "test"
	@echo "TEST 12 PASSED: Проверка на ввод неизвестной команды 6.c"

test_success_command_6:
	@$(SUCCESS_IFILE_OFILE) "6.a" "task6_test/success_print_input_task6.txt" "task6_test/success_print_output_task6.txt" 
	@echo "TEST 13 PASSED: Проверка на вывод всех остановок 6.c"

test: test_task_1 test_task_3  test_task_4 test_task_5 test_task_6 # Здесь перечисляются все написанные вами тесты. Запуск тестов производится по команде make test
	