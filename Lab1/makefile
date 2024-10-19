CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task2.c task3.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=../test_templates# Это директория с шаблонами тестов (если вы их разместите в той же директории что и лабы пишите ./)
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

all: $(EXECUTABLES) # Чтобы собрать все лабы надо ввести команду make #$(CC) $(CFLAGS) $< -o $@ -lm

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

test_task_2: test_command_args_2_1 test_command_args_2_2 test_command_args_2_3 test_command_args_2_4 test_command_args_2_5 test_command_args_2_6 test_command_success_ofile_2_7
	

test_command_args_2_1:
	@$(FAILED_CMD) "task2.a" -1 "ERROR: Incorrect input"
	@echo "Task 2. TEST 1 PASSED. No command line arguments were entered"

test_command_args_2_2:
	@$(FAILED_CMD) "task2.a ahaja" -1 "ERROR: Epsilon must be a positive number and less then 1.0"
	@echo "Task 2. TEST 2 PASSED. It was not a number that was entered"

test_command_args_2_3:
	@$(FAILED_CMD) "task2.a 1234" -1 "ERROR: Epsilon must be a positive number and less then 1.0"
	@echo "Task 2. TEST 3 PASSED. Entered an epsilon greater than 1"

test_command_args_2_4:
	@$(FAILED_CMD) "task2.a 0" -1 "ERROR: Epsilon must be a positive number and less then 1.0"
	@echo "Task 2. TEST 4 PASSED. Entered an epsilon = 0"

test_command_args_2_5:
	@$(FAILED_CMD) "task2.a -0.001" -1 "ERROR: Epsilon must be a positive number and less then 1.0"
	@echo "Task 2. TEST 5 PASSED. Entered a negative epsilon"

test_command_args_2_6:
	@$(FAILED_CMD) "task2.a 0.001 0.001" -1 "ERROR: Incorrect input"
	@echo "Task 2. TEST 6 PASSED. Entered a several epsilon"


test_command_success_ofile_2_7:
	@$(SUCCESS_OFILE) "task2.a 0.001" "task2_output_file.txt"
	@echo "Task 2. TEST 7 PASSED. Entered the epsilon = 0.001 at which it works"
	@echo " "


test_task_3: test_command_args_3_1 test_command_args_3_2 test_command_args_3_3 test_command_args_3_4 test_command_args_3_5 test_command_success_ofile_3_6 test_command_args_3_7 test_command_args_3_8 test_command_args_3_9 test_command_args_3_10 test_command_args_3_11 test_command_args_3_12 test_command_args_3_13


test_command_args_3_1:
	@$(FAILED_CMD) "task3.a" -1 "ERROR: Invalid input"
	@echo "Task 3. TEST 1 PASSED. No command line arguments were entered"

test_command_args_3_2:
	@$(FAILED_CMD) "task3.a dasdjoa" -1 "ERROR: The flag was not found"
	@echo "Task 3. TEST 2 PASSED. The flag and epsilon and numbers were not entered"

test_command_args_3_3:
	@$(FAILED_CMD) "task3.a -q djwajd djwajd djwoadj jodwa" -1 "ERROR: Invalid epsilon value"
	@echo "Task 3. TEST 3 PASSED. A flag was entered, but not a number or an epsilon"

test_command_args_3_4:
	@$(FAILED_CMD) "task3.a -q 0.001 ada dad da" -1 "ERROR: Invalid coefficient value"
	@echo "Task 3. TEST 4 PASSED. The flag and epsilon were introduced, but not the number"

test_command_args_3_5:
	@$(FAILED_CMD) "task3.a -q 0.001 djsaod" -1 "ERROR: Incorrect number of arguments for -q"
	@echo "Task 3. TEST 5 PASSED. The flag and epsilon were introduced, but not the number"

test_command_success_ofile_3_6:
	@$(SUCCESS_OFILE) "task3.a -q 0.001 5 3 2" "task3_output_file_q.txt"
	@echo "Task 3. TEST 6 PASSED. Entered the correct data: epsilon = 0.001, arguments: 5 3 2"

test_command_args_3_7:
	@$(SUCCESS_CMD) "task3.a -m 10 2" "10 is a multiple of 2"
	@echo "Task 3. TEST 7 PASSED. The flag was entered: -m, arguments: 10 2"

test_command_args_3_8:
	@$(FAILED_CMD) "task3.a -m 21 10" -1 "21 is not a multiple of 10"
	@echo "Task 3. TEST 8 PASSED. The flag was entered: -m, arguments: 21 10"

test_command_args_3_9:
	@$(FAILED_CMD) "task3.a -m 10 adsads" -1 "ERROR: The second number cannot be zero or unknown symbols"
	@echo "Task 3. TEST 9 PASSED. The flag was entered: -m and invalid arguments"

test_command_args_3_10:
	@$(FAILED_CMD) "task3.a -t 0.001 5 4 2" -1 "It can't be a rectangular"
	@echo "Task 3. TEST 10 PASSED. The flag was entered: -t and epsilon, arguments: 5 4 2"

test_command_args_3_11:
	@$(FAILED_CMD) "task3.a -t dasd ada da da" -1 "ERROR: Invalid epsilon value"
	@echo "Task 3. TEST 11 PASSED. The flag was entered: -t and invalid arguments"

test_command_args_3_12:
	@$(FAILED_CMD) "task3.a -t 0.001 djs dhsa ada" -1 "ERROR: Invalid side value"
	@echo "Task 3. TEST 12 PASSED. The flag was entered: -t and epsilon, but invalid value of sides"

test_command_args_3_13:
	@$(SUCCESS_CMD) "task3.a -t 0.0000001 5 4 3" "It can be a rectangular"
	@echo "Task 3. TEST 13 PASSED. The flag was entered: -t and epsilon, arguments: 5 4 3"


test: test_task_2 test_task_3 
	