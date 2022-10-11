.PHONY: clean
.PHONY: check
.PHONY: run
q1: main.o line.o
	g++ main.o line.o -o q1 -Wall
main.o: main.cpp
	g++ -c main.cpp -o main.o -Wall
line.o: line.cpp line.h
	g++ -c line.cpp -o line.o -Wall
run: q1
	./q1
check: q1
	./q1 > result.txt
clean:
	rm -rf *.o q1 *.txt