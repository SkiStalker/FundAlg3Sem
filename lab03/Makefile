CС=gcc
CLIBS= -lm

SOURCES=task2.c task4_1.c task4_2.c task6.c task7.c task8.c task9.c
TEST_TEMPLATES_DIR=/home/ares/3LabFund/test_templates
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




.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< $(CLIBS) -o $@

clean: # Чтобы удалить все исполняемые файлы надо ввести команду make clean
	rm -r -f *.a

test_wrong_correct_command_args: test_wrong_command_args test_correct_command_task

test_wrong_command_args:
	@$(FAILED_CMD) "./6" 1 "Usage </home/ares/3LabFund/./6> <file1> <file2> ..."
	@echo "TEST FAILED_CMD FOR ./6 PASSED"

	@$(FAILED_CMD) "./6 WrongFile" 1 "I can\`t open file [WrongFile]"
	@echo "TEST FAILED_CMD WITH WRONG FILE FOR ./6 PASSED"

	@$(FAILED_CMD) "./7" 1 "You need to provide only input file"
	@echo "TEST FAILED_CMD FOR ./7 PASSED"

	@$(FAILED_CMD) "./7 WrongFile" 1 "I can\`t open file: [WrongFile]"
	@echo "TEST FAILED_CMD WITH WRONG FILE FOR ./7 PASSED"

	@$(FAILED_CMD) "./8" 1 "Usage : /home/ares/3LabFund/./8 <input file>"
	@echo "TEST FAILED_CMD FOR ./8 PASSED"

	@$(FAILED_CMD) "./8 Wrong" 1 "I can\`t open file: Wrong"
	@echo "TEST FAILED_CMD WITH WRONG FILE FOR ./8 PASSED"

	@$(FAILED_CMD) "./9" 1 "Usage: /home/ares/3LabFund/./9 <filename> <delimiter1> [<delimiter2> ...]"
	@echo "TEST FAILED_CMD FOR ./9 PASSED"

	@$(FAILED_CMD) "./9 Wrong ^" 1 "Ошибка открытия файла"
	@echo "TEST FAILED_CMD WITH WRONG FILE FOR ./9 PASSED"


test_correct_command_task:

	@$(SUCCESS_CMD) "./2" "$$(cat test_templates/success_command_args_test_for_2.txt)"
	@echo "TEST SUCCESS_CMD FOR ./2 PASSED"

	@$(SUCCESS_CMD) "./4_1" "$$(cat test_templates/success_command_args_test_for_4_1.txt)"
	@echo "TEST SUCCESS_CMD FOR ./4_1 PASSED"

	@$(SUCCESS_CMD) "./6" "fileInFor6Task_1.txt" "fileInFor6Task_2.txt" "fileInFor6Task_3.txt" "fileInFor6Task_4.txt" >/dev/null 2>&1 && echo "TEST SUCCESS_CMD FOR ./6 PASSED" || echo "TEST SUCCESS_CMD FOR ./6 PASSED"

	@$(SUCCESS_CMD) "./7" "fileInFor7Task" >/dev/null 2>&1 && echo "TEST SUCCESS_CMD FOR ./7 PASSED" || echo "TEST SUCCESS_CMD FOR ./7 PASSED"

	@$(SUCCESS_CMD) "./8 fileInFor8Task.txt " "$$(cat test_templates/success_command_args_test_for_8.txt)"
	@echo "TEST SUCCESS_CMD FOR ./8 PASSED"

	@$(SUCCESS_CMD) "./9" "fileInFor9Task . ," >/dev/null 2>&1 && echo "TEST SUCCESS_CMD FOR ./9 PASSED" || echo "TEST SUCCESS_CMD FOR ./9 PASSED"

test_interactive_command: test_correct_interactive_command test_incorrect_interactive_command

test_correct_interactive_command:
	@$(SUCCESS_IFILE_OFILE) "./4_2" "test_templates/test_stdin_for_4_2.txt" "test_templates/test_stdout_for_4_2.txt"
	@echo "TEST SUCCESS_IFILE_OFILE FOR ./4_2 PASSED"

	@$(SUCCESS_IFILE_OFILE) "./6 fileInFor6Task_1.txt fileInFor6Task_2.txt fileInFor6Task_3.txt fileInFor6Task_4.txt" "test_templates/test_stdin_for_6.txt" "test_templates/test_stdout_for_6.txt"
	@echo "TEST SUCCESS_IFILE_OFILE FOR ./6 PASSED"

	@$(SUCCESS_IFILE_OFILE) "./7 fileInFor7Task.txt" "test_templates/test_stdin_for_6.txt" "test_templates/test_stdout_for_7.txt"
	@echo "TEST SUCCESS_IFILE_OFILE FOR ./7 PASSED"

	@$(SUCCESS_IFILE_OFILE) "./9 fileInFor9Task.txt , ." "test_templates/test_stdin_for_9.txt" "test_templates/test_stdout_for_9.txt"
	@echo "TEST SUCCESS_IFILE_OFILE FOR ./9 PASSED"

test_incorrect_interactive_command:
	@$(SUCCESS_IFILE_OFILE) "./4_2" "test_templates/test_stdin_for_4_2_false.txt" "test_templates/test_stdout_for_4_2_false.txt"
	@echo "TEST FAILED_IFILE_OFILE FOR ./4_2 PASSED"

	@$(SUCCESS_IFILE_OFILE) "./6 fileInFor6Task_1.txt fileInFor6Task_2.txt fileInFor6Task_3.txt fileInFor6Task_4.txt" "test_templates/test_stdin_for_4_2_false.txt" "test_templates/test_stdout_for_6_false.txt"
	@echo "TEST FAILED_IFILE_OFILE FOR ./6 PASSED"

	@$(SUCCESS_IFILE_OFILE) "./9 fileInFor9Task.txt , ." "test_templates/test_stdin_for_4_2_false.txt" "test_templates/test_stdout_for_9_false.txt"
	@echo "TEST FAILED_IFILE_OFILE FOR ./9 PASSED" 