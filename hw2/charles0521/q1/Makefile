CXXFLAGS = g++ --std=c++11


all: main.o Line.o
	$(CXXFLAGS) -o main main.o Line.o

Line.o: Line.h Line.cpp
	$(CXXFLAGS) -c Line.cpp

main.o: main.cpp Line.h
	$(CXXFLAGS) -c main.cpp

run:
	./main

check:
	./main > result.txt

clean:
	rm -rf main *.o result.txt
