CXX=g++
CFLAGS= -Wall -Werror -Wextra -std=c++20

SOURCES=lb5task6.cpp # Тут вы перечисляете файлы с вашими лабами по 1 файлу на задание
TEST_TEMPLATES_DIR=../test_templates
EXECUTABLES=$(SOURCES:.cpp=.exe)
BASH=bash
SUCCESS_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
SUCCESS_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_ofile
FAILED_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd
FAILED_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/failed_ofile
SUCCESS_RFILE_CMD=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_cmd
SUCCESS_RFILE_OFILE=$(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_ofile

.PHONY: all test clean

all: $(EXECUTABLES) # Чтобы собрать все лабы надо ввести команду make

%.exe : %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean: # Чтобы удалить все исполняемые файлы надо ввести команду make clean
	rm -f *.exe