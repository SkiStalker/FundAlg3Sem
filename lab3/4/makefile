
SOURCES = solution.c realization.c string_realization.c

CC = gcc
CFLAGS = -Wall -Wextra -g


TARGET = a.out

DIALOG = dialog.txt

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET) < $(DIALOG)

clean:
	rm -f $(TARGET)
