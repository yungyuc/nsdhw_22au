CXX = g++

.PhONY: default
default: q1

line.o: line.cpp line.h
	$(CXX) -c line.cpp -o line.o

main.o: q1.cpp line.h
	$(CXX) -c q1.cpp -o main.o

q1: line.o main.o
	$(CXX) main.o line.o -o q1

.PHONY: run
run:
	./q1

.PHONY: clean
clean:
	rm -f *.o q1

.PHONY: check
check:
	./q1 > result.txt