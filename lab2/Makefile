CC=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task2_1.c task2_2.c task2_3.c task2_7.c task2_9.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
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

test_task_1: test_1_without_args test_1_l test1_incor_flag test_1_r test_1_u test_1_n test_1_c
	

test_1_without_args:
	@$(FAILED_CMD) "task2_1.a" 1 "To little parameters, must be bigger 2"
	@echo "Test 2.1 without args passed"

test_1_l:
	@$(SUCCESS_CMD) "task2_1.a -l abc" "3"
	@echo "Test 2.1 flag l passed"

test1_incor_flag:
	@$(FAILED_CMD) "task2_1.a /n abc" 1 "Flag /n does not exist"
	@echo "Test 2.1 with incorrect flag passed"

test_1_r:
	@$(SUCCESS_CMD) "task2_1.a -r abc" "cba"
	@echo "Test 2.1 flag r passed"

test_1_u:
	@$(SUCCESS_CMD) "task2_1.a -u abcd" "aBcD"
	@echo "Test 2.1 flag u passed"

test_1_n:
	@$(SUCCESS_CMD) "task2_1.a -n a!b12c" "12abc!"
	@echo "Test 2.1 flag n passed"

test_1_c:
	@$(SUCCESS_CMD) "task2_1.a -c abc 3 qwe rty" "abcqwerty"
	@echo "Test 2.1 flag c passed"

test_task_2: t21 t22 t23 t24

t21:
	@$(SUCCESS_CMD) "task2_2.a 1" "1.817121"
	@echo "Task 2.2 geometric mean for correct nums passed"
t22:
	@$(FAILED_CMD) "task2_2.a 2" 1 "No positive real num"
	@echo "Task 2.2 geometric mean for negative nums passed"
t23:
	@$(SUCCESS_CMD) "task2_2.a 3" "-0.125000"
	@echo "Task 2.2 negative power and num passed"
t24:
	@$(SUCCESS_CMD) "task2_2.a 4" "256.000000"
	@echo "Task 2.2 positive power and num passed"

test_task_7: t71 t72 t73

t71:
	@$(SUCCESS_CMD) "task2_7.a 1" "3.141602"
	@echo "Task 2.7 for increasing func passed"

t72:
	@$(SUCCESS_CMD) "task2_7.a 2" "3.141602"
	@echo "Task 2.7 for decreasing func passed"
t73:
	@$(FAILED_CMD) "task2_7.a 3" 1 "Epsilon must be bigger 0 and less 1"
	@echo "Task 2.7 for incorrect epsilon passed"

test_task_9: t91 t92

t91:
	@$(SUCCESS_CMD) "task2_9.a 1" "base 2:    0.100000:No 0.000100:No 0.500000:Yes 0.250000:Yes 0.800000:No 0.130000:No "
	@echo "Task 2.9 for correct base and nums passed"
t92:
	@$(FAILED_CMD) "task2_9.a 2" 1 "base 2:    0.100000:No Fraction must be from 0 to 1"
	@echo "Task 2.9 for negative fraction passed"

test_task_3: t31 t32

t31:
	@$(SUCCESS_CMD) "task2_3.a 1" "1.txt:   Substring in 2 string(11 symbol)  2.txt:   No such substring "
	@echo "Task 2.3 for presense and absense substring in 2 files passed"
t32:
	@$(FAILED_CMD) "task2_3.a 2" 1 "1.txt:   Substring in 2 string(11 symbol)  File opening error"
	@echo "Task 2.3 for non-existent file passed"


test: test_task_1 test_task_2 test_task_7 test_task_9 test_task_3