CFLAGS = -Wall -Wextra -Werror -pedantic -g -fdiagnostics-color=always
LDFLAGS = -lm
SRC = functions.c task5.c
OBJ = $(SRC:.c=.o)
EXEC = final

all: $(EXEC)

$(EXEC): $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

valgrind: $(EXEC)
	valgrind -s --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(EXEC) $(ARGS)

clean:
	rm -f $(OBJ) $(EXEC)