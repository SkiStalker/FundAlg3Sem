CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lm
TARGET=Task3
SRC=Task3.c

.PHONY: all clean test


all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)


test: all
	@echo "Running tests..."
	@echo "========================================================="
	@find tests -name "*.sh" -exec bash {} \;

clean:
	rm -f $(TARGET)