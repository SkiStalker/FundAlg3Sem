CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=Lab2_4.c
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



test_success1_1:
	@$(SUCCESS_CMD) "./Lab2_4.a" "No convexity 4.000000 G IS NOT CUPCER'S NUMBER B IS NOT CUPCER'S NUMBER 2 IS NOT CUPCER'S NUMBER "
	@echo "ALWAYS SUCCESS"

test: test_success1_1