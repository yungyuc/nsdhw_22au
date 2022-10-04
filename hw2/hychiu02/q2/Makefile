PROGRAM_NAME = main

CXX = g++

all: main.o
	$(CXX) main.o -o main

main.o: main.cpp
	$(CXX) -c main.cpp -o main.o

run: all
	./${PROGRAM_NAME}

.PHONY: clean

clean:
	rm -f *.o

