CC=gcc
CFLAGS=-Wall -Wextra
TARGET=9
SRC=9.c

.PHONY: all clean test

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: all
	@echo "Running tests..."
	@echo "========================================================="
	@find tests -name "*.sh" -exec bash {} \;

clean:
	rm -f $(TARGET)
