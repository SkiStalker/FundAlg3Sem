CC=C:/MinGW/bin/gcc.exe
CFLAGS= -Wall -Werror -Wextra
CLIBS= -lm

all: 1.exe 2.exe 3.exe 5.exe 10.exe

1.exe: 1.c
	$(CC) $(CFLAGS) -o $@ $<

2.exe: 2.c
	$(CC) $(CFLAGS) -o $@ $<

3.exe: 3.c
	$(CC) $(CFLAGS) -o $@ $<

5.exe: 5.c
	$(CC) $(CFLAGS) -o $@ $<

10.exe: 10.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	del /Q *.exe