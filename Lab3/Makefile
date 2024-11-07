CC = gcc
CFLAGS = -Wall -Werror -Wextra

PROGRAMS = 4_1 4_2 7

all: $(PROGRAMS)

$(PROGRAMS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean