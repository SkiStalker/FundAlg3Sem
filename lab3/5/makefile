
SOURCES = solution.c realization.c

CC = gcc
CFLAGS = -Wall -Werror -Wextra -g


TARGET = a.out

DIALOG = dialog.txt

INPUT = input.txt

OUTPUT = output.txt

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET) $(INPUT) $(OUTPUT) < $(DIALOG)

clean:
	rm -f $(TARGET)
