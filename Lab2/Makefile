CС=gcc
CFLAGS= -Wall -Wextra
CLIBS= -lm

SOURCES=main1.c main2.c main4.c main8.c main9.c# Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
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

test_task_1: FAILED_test_command_args_1_CMD SUCCESS_test_command_args_2_CMD SUCCESS_test_command_args_3_CMD SUCCESS_test_command_args_4_CMD SUCCESS_test_command_args_5_CMD SUCCESS_test_command_args_6_CMD
	
FAILED_test_command_args_1_CMD:
	@$(FAILED_CMD) "main1.a" -1 "Wrong amount of arguments!"
	@echo "TASK 1: TEST WITH WRONG AMOUNT OF ARGUMENTS PASSED"

SUCCESS_test_command_args_2_CMD:
	@$(SUCCESS_CMD) "main1.a -l onetwothree" "11"
	@echo "TASK 1: TEST WITH RIGHT INPUT PASSED. -l WORKS CORRECTLY"

SUCCESS_test_command_args_3_CMD:
	@$(SUCCESS_CMD) "main1.a -r qwe" "ewq"
	@echo "TASK 1: TEST WITH RIGHT INPUT PASSED. -r WORKS CORRECTLY"

SUCCESS_test_command_args_4_CMD:
	@$(SUCCESS_CMD) "main1.a -u qwerty" "qWeRtY"
	@echo "TASK 1: TEST WITH RIGHT INPUT PASSED. -u WORKS CORRECTLY"

SUCCESS_test_command_args_5_CMD:
	@$(SUCCESS_CMD) "main1.a -n 03jgi439ug8oe,./,.3276" "0343983276jgiugoe,./,."
	@echo "TASK 1: TEST WITH RIGHT INPUT PASSED. -n WORKS CORRECTLY"

SUCCESS_test_command_args_6_CMD:
	@$(SUCCESS_CMD) "main1.a -c fhuk 13 ush fhu" "fhukfhuush"
	@echo "TASK 1: TEST WITH RIGHT INPUT PASSED. -c WORKS CORRECTLY"

test_task_2: SUCCESS_test_1_FILE SUCCESS_test_2_FILE SUCCESS_test_3_FILE


SUCCESS_test_1_FILE:
	@$(SUCCESS_IFILE_OFILE) "main2.a" "console_input_file_for_task_2.txt" "console_output_file_for_task_2.txt"
	@echo "TASK 2: TEST WITH RIGHT INPUT PASSED. The geometric mean and pow are calculated correctly (2 ^ 6 = 64)"

SUCCESS_test_2_FILE:
	@$(SUCCESS_IFILE_OFILE) "main2.a" "console_input_file_for_task_2_1.txt" "console_output_file_for_task_2_1.txt"
	@echo "TASK 2: TEST WITH RIGHT INPUT PASSED. The geometric mean and pow are calculated correctly (3^3 = 27)"

SUCCESS_test_3_FILE:
	@$(SUCCESS_IFILE_OFILE) "main2.a" "console_input_file_for_task_2_2.txt" "console_output_file_for_task_2_2.txt"
	@echo "TASK 2: TEST WITH RIGHT INPUT PASSED. The geometric mean and pow are calculated correctly (1.5^5 = 7.593750)"


test_task_4: SUCCESS_test_to_find_if_convex_FILE SUCCESS_test_to_find_if_convex_1FILE SUCCESS_test_to_find_poly_in_x_FILE SUCCESS_test_to_find_Kaprenkar_FILE

SUCCESS_test_to_find_if_convex_FILE:
	@$(SUCCESS_IFILE_OFILE) "main4.a" "4console_input1.txt" "4console_output1.txt"
	@echo "TASK 4: TEST WITH A TRIANGLE PASSED. IT'S CONVEX"

SUCCESS_test_to_find_if_convex_1FILE:
	@$(SUCCESS_IFILE_OFILE) "main4.a" "4console_input2.txt" "4console_output2.txt"
	@echo "TASK 4: TEST WITH NOT CONVEX FIGURE PASSED. IT'S NOT CONVEX"

SUCCESS_test_to_find_poly_in_x_FILE:
	@$(SUCCESS_IFILE_OFILE) "main4.a" "4console_input3.txt" "4console_output3.txt"
	@echo "TASK 4: TEST WITH FINDING POLY PASSED. THE ANSWER IS RIGHT"

SUCCESS_test_to_find_Kaprenkar_FILE:
	@$(SUCCESS_IFILE_OFILE) "main4.a" "4console_input4.txt" "4console_output4.txt"
	@echo "TASK 4: TEST WITH FINDING KAPRENKAR NUMS PASSED"

test_task_8: SUCCESS_16NS_SUM SUCCESS_10NS_SUM SUCCESS_2NS_SUM 

SUCCESS_16NS_SUM:
	@$(SUCCESS_IFILE_OFILE) "main8.a" "8console_input1.txt" "8console_output1.txt"
	@echo "TASK 8: TEST WITH A SUM OF 3 16 num sys numbers passef (FF - FF + 10FA = 10FA)"

SUCCESS_10NS_SUM:
	@$(SUCCESS_IFILE_OFILE) "main8.a" "8console_input2.txt" "8console_output2.txt"
	@echo "TASK 8: TEST WITH A SUM OF 3 10 num sys numbers passef (1 + 9 + 10 = 20)"

SUCCESS_2NS_SUM:
	@$(SUCCESS_IFILE_OFILE) "main8.a" "8console_input3.txt" "8console_output3.txt"
	@echo "TASK 8: TEST WITH A SUM OF 3 2 num sys numbers passef (1 + 1 + 10 = 100)"

test_task_9: SUCCESS_2_NS SUCCESS_8_NS

SUCCESS_2_NS:
	@$(SUCCESS_IFILE_OFILE) "main9.a" "9console_input1.txt" "9console_output1.txt"
	@echo "TASK 9: TEST WITH TWO representable AND ONE NOT representable PASSED IN 2nd NUM SYS"

SUCCESS_8_NS:
	@$(SUCCESS_IFILE_OFILE) "main9.a" "9console_input2.txt" "9console_output2.txt"
	@echo "TASK 9: TEST WITH TWO NOT representable AND ONE representable PASSED IN 8nd NUM SYS"

test: test_task_1 test_task_2 test_task_4 test_task_8 test_task_9# Здесь перечисляются все написанные вами тесты. Запуск тестов производится по команде make test
	