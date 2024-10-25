# Определяем компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Werror -Wextra

# Определяем список программ
PROGRAMS = point2 point3 point4 point5

# Цель по умолчанию: сборка всех программ
all: $(PROGRAMS)

# Правила сборки каждой программы
$(PROGRAMS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Команда очистки (удаление скомпилированных файлов)
clean:
	rm -f $(PROGRAMS)

# Фиктивные цели для предотвращения конфликтов с файлами
.PHONY: all clean
