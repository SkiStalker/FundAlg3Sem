CC = gcc
CFLAGS = -Wall -Werror -Wextra

PROGRAMS = 6 8

all: $(PROGRAMS)

$(PROGRAMS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean