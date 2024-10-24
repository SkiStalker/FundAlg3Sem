CС=gcc
CFLAGS= -Wall -Werror -Wextra

SOURCES=lb2task8.c # Тут вы перечислятете файлы с вашими лабами по 1 файлу на задание
EXECUTABLES=$(SOURCES:.c=.a)
BASH=bash

.PHONY: all test clean

all: $(EXECUTABLES) # Чтобы собрать все лабы надо ввести команду make

%.a : %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean: # Чтобы удалить все исполняемые файлы надо ввести команду make clean
	rm -r -f *.a
