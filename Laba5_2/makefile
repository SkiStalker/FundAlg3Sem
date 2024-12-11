# Имя исполняемого файла
TARGET = encoder

# Исходные файлы
SRCS = task2.cpp

# Объектные файлы
OBJS = $(SRCS:.cpp=.o)

# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Правило для компиляции каждого .cpp файла в .o файл
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

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