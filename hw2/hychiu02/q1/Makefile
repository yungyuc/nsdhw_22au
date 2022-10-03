PROGRAM_NAME = main

CXX = g++

run: all
	./${PROGRAM_NAME}

check: all
	./${PROGRAM_NAME} > result.txt 2>&1

all: Line.o main.o
	$(CXX) Line.o main.o -o main

Line.o: Line.cpp Line.hpp
	$(CXX) -c Line.cpp -o Line.o

main.o: main.cpp Line.hpp
	$(CXX) -c main.cpp -o main.o

.PHONY: run check clean 

clean:
	rm -f *.o main result.txt