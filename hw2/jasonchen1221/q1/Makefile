.PHONY: run check clean

CXX = g++

Line.exe: line.o main.o
	$(CXX) line.o main.o -o Line.exe

line.o: line.cpp line.hpp
	$(CXX) -c line.cpp -o line.o

main.o: main.cpp line.hpp
	$(CXX) -c main.cpp -o main.o

run:
	./Line.exe

check:
	make
	./Line.exe > ./result.txt

clean:
	rm -rf *.o *.exe *.txt

