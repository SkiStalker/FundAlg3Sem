CC = gcc
CFLAGS = -Wall -Werror -Wextra
CLIBS = -lm

SOURCES = 1.c 2.c 3.c 5.c 10.c
TEST_TEMPLATES_DIR = ../../test_templates
EXECUTABLES = $(SOURCES:.c=.a)
BASH = bash

SUCCESS_CMD = $(BASH) $(TEST_TEMPLATES_DIR)/success_cmd
FAILED_CMD = $(BASH) $(TEST_TEMPLATES_DIR)/failed_cmd
SUCCESS_RFILE_CMD = $(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_cmd
SUCCESS_OFILE = $(BASH) $(TEST_TEMPLATES_DIR)/success_ofile
FAILED_OFILE = $(BASH) $(TEST_TEMPLATES_DIR)/failed_ofile
SUCCESS_RFILE_OFILE = $(BASH) $(TEST_TEMPLATES_DIR)/success_rfile_ofile
SUCCESS_IFILE_OFILE = $(BASH) $(TEST_TEMPLATES_DIR)/success_ifile_ofile
SUCCESS_IFILE_RFILE_OFILE = $(BASH) $(TEST_TEMPLATES_DIR)/success_ifile_rfile_ofile
FAILED_IFILE_OFILE = $(BASH) $(TEST_TEMPLATES_DIR)/failed_ifile_ofile
FAILED_IFILE_RFILE_OFILE = $(BASH) $(TEST_TEMPLATES_DIR)/failed_ifile_rfile_ofile

.PHONY: all test clean clean-win clean-unix

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< $(CLIBS) -o $@

clean-unix:
	rm -f *.a

clean-win:
	del /Q *.a

clean: clean-unix