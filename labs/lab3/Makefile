CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task_10.c task_08.c task_09.c			# Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=../../test_templates# Это директория с шаблонами тестов (если вы их разместите в той же директории что и лабы пишите ./)
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

test_task_10: test_10_command_args1 test_10_command_args2 test_10_command_args3 test_10_command_args4 test_10_success

test_10_command_args1:
	@$(FAILED_CMD) "task_10.a" 1 "ERROR: no input file provided"
	@echo "TEST 1 PASSED: task 10 handle not providing input file"

test_10_command_args2:
	@$(FAILED_CMD) "task_10.a inp" 1 "ERROR: no output file provided"
	@echo "TEST 2 PASSED: task 10 handle not providing input file"

test_10_command_args3:
	@$(FAILED_CMD) "task_10.a inp inp" 1 "ERROR: input and output files are the same"
	@echo "TEST 3 PASSED: task 10 handle input and output file being the same"

test_10_command_args4:
	@$(FAILED_CMD) "task_10.a inp.not_existing inp" 1 "ERROR: failed to open file [inp.not_existing]"
	@echo "TEST 4 PASSED: task 10 handle failing to open input file"

test_10_success:
	@$(SUCCESS_RFILE_CMD) "task_10.a task_10_in.txt task_10_out.txt" "" "./task_10_out.txt" "./task_10_out_expected.txt"
	@echo "TEST 5 PASSED: task 10 working correctly"


test_task_08: test_08_command_args1 test_08_command_args2 test_08_success

test_08_command_args1:
	@$(FAILED_CMD) "task_08.a" 1 "ERROR: no input file provided"
	@echo "TEST 6 PASSED: task 08 handle not providing input file"

test_08_command_args2:
	@$(FAILED_CMD) "task_08.a inp.not_existing" 1 "ERROR: failed to open file [inp.not_existing]"
	@echo "TEST 7 PASSED: task 08 handle failing to open input file"

test_08_success:
	@$(SUCCESS_OFILE) "task_08.a ./task_08_in.txt" "./task_08_out_expected.txt"
	@echo "TEST 8 PASSED: task 08 working correctly"

test_task_09: test_09_command_args1 test_09_command_args2 test_09_command_args3 test_09_command_args4 test_09_success1 test_09_success2

test_09_command_args1:
	@$(FAILED_CMD) "task_09.a" 1 "ERROR: no input file provided"
	@echo "TEST 9 PASSED: task 09 handle not providing input file"

test_09_command_args2:
	@$(FAILED_CMD) "task_09.a inp" 1 "ERROR: at least one separator must be provided"
	@echo "TEST 10 PASSED: task 09 handle not providing separators"

test_09_command_args3:
	@$(FAILED_CMD) "task_09.a inp.not_existing inp" 1 "ERROR: each separator must be a signle symbol"
	@echo "TEST 11 PASSED: task 09 handle invalid separator"

test_09_command_args4:
	@$(FAILED_CMD) "task_09.a inp.not_existing ' '" 1 "ERROR: failed to open file [inp.not_existing]"
	@echo "TEST 12 PASSED: task 09 handle failing to open input file"

test_09_success1:
	@$(SUCCESS_IFILE_OFILE) "task_09.a ./task_09_in.txt ; :" "./task_09_stdin1.txt" "./task_09_out_expected.txt"
	@echo "TEST 13 PASSED: task 09 working correctly"

test_09_success2:
	@$(SUCCESS_IFILE_OFILE) "task_09.a ./task_09.c ;" "./task_09_stdin2.txt" "./task_09_out_expected.txt"
	@echo "TEST 14 PASSED: task 09 working correctly"



test: test_task_10 test_task_08 test_task_09
