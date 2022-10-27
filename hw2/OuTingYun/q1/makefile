# declare variables
CC = g++

all: main

main: main.o line.o 
	$(CC) -o main main.o line.o
main.o: main.cpp line.h
	$(CC) -c main.cpp
line.o: line.cpp line.h
	$(CC)  -c line.cpp


clean:
	rm -f main.o line.o main


run:
	./main


check:
	./main >> ./result.txt