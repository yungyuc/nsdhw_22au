CXX = g++
CFLAGS = -g -Wall -std=c++11

main: Line.o main.o
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf main *.o *.txt

run:
	./main

check:
	./main > result.txt
