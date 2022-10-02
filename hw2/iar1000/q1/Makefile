
CC = g++
CFLAGS = -std=c++11 -Wc++11-extensions

all: main.o Line.o
	$(CC) $(CFLAGS) -o main main.o Line.o

main.o: main.cpp Line.cpp
	$(CC) $(CFLAGS) -c main.cpp Line.cpp 

Line.o: Line.cpp
	$(CC) $(CFLAGS) -c Line.cpp 

check:
	make && ./main > "result.txt"

clean: 
	rm main Line.o main.o result.txt

run:
	make && ./main