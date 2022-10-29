.PHONY: run check clean

CXX = g++

all: main.o line.o 
	$(CXX) -o line main.o line.o

line.o: line.cpp line.hpp
	$(CXX) -c line.cpp 

main.o: main.cpp line.hpp
	$(CXX) -c main.cpp 

run:
	./line

check:
	./line > result.txt

clean:
	rm -rf line *.o *.txt
