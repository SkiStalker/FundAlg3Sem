CС=gcc

SOURCES=9.c
TEST_TEMPLATES_DIR=./test_templates
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash

.PHONY: all test clean

all: $(EXECUTABLES) 

%.a : %.c
	$(CC) $< -o $@ -lm

clean: 
	rm -r -f *.a


