CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=Lab2_10.c
TEST_TEMPLATES_DIR=/mnt/c/Users/Алексей/CLionProjects/Zavorotnyi_M80-214--23/test_templates
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
SUCCESS_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ofile
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd
FAILED_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ofile
SUCCESS_RFILE_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_cmd
SUCCESS_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_ofile


.PHONY: all test clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -r -f *.a

#без эпсилон на входе так как делаю без него(через треугольник Пифагора) сам придумал спросил у других они делали через
#ряд Тейлора я спросил сказали что сделать как хочешь можно
test_success1_1:
	@$(SUCCESS_CMD) "./Lab2_10.a" "26.000000 23.000000 8.000000 1.000000 "
	@echo "ALWAYS SUCCESS"

test: test_success1_1