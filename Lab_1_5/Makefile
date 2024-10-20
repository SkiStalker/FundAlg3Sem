CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=lr1.5.cpp# Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=/mnt/c/Users/Кря/CLionProjects/Gusev_S_214/Lab_1_5# Это директория с шаблонами тестов (если вы их разместите в той же директории что и лабы пишите ./)
EXECUTABLES=$(SOURCES:.cpp=.a)
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

%.a : %.cpp
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

test_command_args_1:
	@$(FAILED_CMD) "lr1.5.a" 7 "Wrong number of arguments."
	@echo "TEST 1 PASSED. we enter without arguments"

test_command_args_2:
	@$(FAILED_CMD) "lr1.5.a 0.00000 0.5" 2 "Epsilon is zero."
	@echo "TEST 2 PASSED. we enter a zero epsilon"

test_command_args_3:
	@$(FAILED_CMD) "lr1.5.a  0.000000001 0.5" 5 "Too long string was entered."
	@echo "TEST 3 PASSED. we enter too long epsilon"

test_command_args_4:
	@$(FAILED_CMD) "lr1.5.a 0.001 0.000000000000005" 5 "Too long string was entered."
	@echo "TEST 4 PASSED. we enter too long argument"

test_command_success_ofile_1:
	@$(SUCCESS_OFILE) "lr1.5.a 0.001 0.5" "output_for_task_1_5.txt"
	@echo "TEST 5 PASSED. we enter the number at which it works"

test_command_args_5:
	@$(FAILED_CMD) "lr1.5.a 0.001 0.5 23 1 4" 7 "Wrong number of arguments."
	@echo "TEST 6 PASSED. entering the wrong number of arguments"

test_command_args_6:
	@$(FAILED_CMD) "lr1.5.a 0.001 a" 3 "Value is not a number."
	@echo "TEST 7 PASSED. we enter an argument that is not a number"

test_command_success_ofile_2:
	@$(SUCCESS_OFILE) "lr1.5.a 0.001 0.00000000" "output_for_task_1_5_2.txt"
	@echo "TEST 8 PASSED. we enter the number at which it works"

test_command_args_7:
	@$(FAILED_CMD) "lr1.5.a a 0.5" 3 "Value is not a number."
	@echo "TEST 9 PASSED. we enter a epsilon that is not a number"

test_command_args_8:
	@$(FAILED_CMD) "lr1.5.a 21 0.5" 6 "Epsilon is too big."
	@echo "TEST 10 PASSED. entering too big epsilon"

test_command_args_9:
	@$(FAILED_CMD) "lr1.5.a -0.000001 0.5" 4 "Epsilon can not be negative."
	@echo "TEST 10 PASSED. entering the negative epsilon"

test_command_success_ofile_3:
	@$(SUCCESS_OFILE) "lr1.5.a 0.001 1" "output_for_task_1_5_3.txt"
	@echo "TEST 12 PASSED. we enter the number at which it works"

test: test_command_args_1 test_command_args_2 test_command_args_3 test_command_args_4 test_command_success_ofile_1 test_command_args_5 test_command_args_6 test_command_success_ofile_2 test_command_args_7 test_command_args_8 test_command_args_9 test_command_success_ofile_3# Здесь перечисляются все написанные вами тесты. Запуск тестов производится по команде make test