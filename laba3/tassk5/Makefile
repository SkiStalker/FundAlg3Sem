CС=gcc
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

SOURCES=task5.c
EXECUTABLES=$(SOURCES:.c=.a)

.PHONY: clean

all: $(EXECUTABLES)

%.a : %.c
	$(CC) $(CFLAGS) $< $(CLIBS) -o $@

clean:
	rm -r -f *.a

