CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17

PROGRAMS = 1_binary_int 2_encoder 3_logical_values_array 4_complex

all: $(PROGRAMS)

$(PROGRAMS): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean