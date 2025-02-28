.PHONY: all clean error

GCC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -fsanitize=address -fsanitize=leak
MY_ERROR_CFLAGS=-Wall -Wextra -pedantic -fsanitize=address -fsanitize=leak

all: static_and_linking.c functions.c functions.h # Goog example demonstrating the work of the linker
	$(GCC) $(CFLAGS) $^ -o $(<:.c=.a)

error: static_and_linking.c functions.c functions.h # The pitfalls of the linker's work and common mistakes
	$(GCC) $(MY_ERROR_CFLAGS) -D MY_ERROR $^ -o s$(<:.c=.a)

clean:
	rm *.a
