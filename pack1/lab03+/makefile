TARGET = 3

CC = gcc

CFLAGS = -Wall -Wextra -g  

LIBS = -lm 

SRCS = 3.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)  

%.o: %.c
	$(CC) $(CFLAGS) -c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
