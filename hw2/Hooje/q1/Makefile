CC = g++
CFLAGS = -g -Wall -std=c++11

.PHONY: all run check clean 

all : main

main: line.o main.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main *.o *.txt

run:
	./main

check:
	./main > result.txt