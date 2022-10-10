CXX = g++
CFLAGS = -g -Wall -Werror -std=c++11
.PHONY: run check clean

main: line.o main.o
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@


run:
	./main

check:
	./main > result.txt

clean:
	rm -rf main *.o *.txt