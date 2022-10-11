CXX = g++
CFLAGS = -std=c++17 -O2 -Wall -Wextra -Wshadow

OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
EXE = main

.PHONY: all clean run

all: $(OBJ) $(EXE)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJ) $(EXE)

run:
	./$(EXE)

check:
	./$(EXE) > result.txt
