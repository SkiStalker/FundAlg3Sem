CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=1.c
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash


.PHONY: all test clean

all: $(EXECUTABLES) 

%.a : %.c
	$(CC) $(CFLAGS) $< $(CLIBS) -o $@

clean:
	rm -r -f *.a