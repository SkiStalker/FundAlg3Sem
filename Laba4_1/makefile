# Имя исполняемого файла
TARGET = encoder

# Исходные файлы
SRCS = task1.c

# Объектные файлы
OBJS = $(SRCS:.c=.o)

# Компилятор
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Wextra -std=c11 -O2

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Правило для компиляции каждого .c файла в .o файл
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для очистки
clean:
	rm -f $(OBJS) $(TARGET)

# Правило для полного удаления всех сгенерированных файлов
distclean: clean
	rm -f *~

# Правило для перекомпиляции всего проекта
rebuild: clean all

# Указываем, что целевые объекты могут зависеть от файлов, которые не создаются этим Makefile
.PHONY: all clean distclean rebuild