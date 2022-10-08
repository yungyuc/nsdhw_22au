
CXX = g++
CFLAGS = --std=c++11


all: main.o line.o
	$(CXX) -o main main.o line.o $(CFLAGS)

line.o: line.h line.cpp
	$(CXX) -c line.cpp $(CFLAGS)

main.o: main.cpp line.h
	$(CXX) -c main.cpp $(CFLAGS)

run:
	./main

check:
	./main > result.txt

clean:
	rm -f main *.o main result.txt
