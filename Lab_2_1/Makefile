CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task1.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=./../test_templates# Это директория с шаблонами тестов (если вы их разместите в той же директории что и лабы пишите ./)
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
	

test_command_args_1:
	@$(FAILED_CMD) "task1.a" -1 "Wrong number of arguments"
	@echo "TEST 1 PASSED"

test_command_args_2:
	@$(FAILED_CMD) "task1.a -l" -1 "Wrong number of arguments"
	@echo "TEST 2 PASSED"

test_command_args_3:
	@$(FAILED_CMD) "task1.a -a somestring" -1 "Flag <-a> is not supported."
	@echo "TEST 3 PASSED"

test_command_args_4:
	@$(SUCCESS_CMD) "task1.a -l somestring" "Length is 10."
	@echo "TEST 4 PASSED"

test_command_args_5:
	@$(SUCCESS_CMD) "task1.a -r 123456789" "Reverse is 987654321"
	@echo "TEST 5 PASSED"

test_command_args_6:
	@$(SUCCESS_CMD) "task1.a -u somestring" "EveryOddToUpped is sOmEsTrInG"
	@echo "TEST 6 PASSED"

test_command_args_7:
	@$(SUCCESS_CMD) "task1.a -n 123...t/e/x/t;;;TEXT9898" "NewOrder is 1239898textTEXT...///;;;"
	@echo "TEST 7 PASSED"

test_command_args_8:
	@$(FAILED_CMD) "task1.a -c text" -1 "Wrong number of arguments."
	@echo "TEST 8 PASSED"

test_command_args_9:
	@$(FAILED_CMD) "task1.a -c text text" -1 "Wrong number of arguments."
	@echo "TEST 9 PASSED"

test_command_args_10:
	@$(FAILED_CMD) "task1.a -c text text text" -1 "not a number"
	@echo "TEST 10 PASSED"

test_command_args_11:
	@$(FAILED_CMD) "task1.a -c text 0.1 text" -1 "not a number"
	@echo "TEST 11 PASSED"

test_command_args_12:
	@$(FAILED_CMD) "task1.a -c text -1 text" -1 "not a number"
	@echo "TEST 12 PASSED"

test_command_args_13:
	@$(SUCCESS_CMD) "task1.a -c text 100 123 SOME text TO waTCH" "New string is <textSOMEwaTCHTO123text>"
	@echo "TEST 13 PASSED"

test_command_args_14:
	@$(SUCCESS_CMD) "task1.a -c text 52 123 SOME text TO waTCH" "New string is <textSOMETO123waTCHtext>"
	@echo "TEST 14 PASSED"

test_command_args_15:
	@$(SUCCESS_CMD) "task1.a -c text 1052 test TEXT ab HH" "New string is <TEXTabtesttextHH>"
	@echo "TEST 15 PASSED"


test: test_command_args_1 test_command_args_2 test_command_args_3 test_command_args_4 test_command_args_5 test_command_args_6 test_command_args_7 test_command_args_8 test_command_args_9 test_command_args_10 test_command_args_11 test_command_args_12 test_command_args_13 test_command_args_14 test_command_args_15# Здесь перечисляются все написанные вами тесты. Запуск тестов производится по команде make test
	