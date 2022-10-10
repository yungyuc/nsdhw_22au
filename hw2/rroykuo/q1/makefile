CC = g++
VER = -std=c++11
.PHONY: all clean install

all:main.cpp
	$(CC) $(VER) main.cpp -o main

run:main.cpp
	./main

check:main.cpp
	./main >> result.txt

clean:
	$(RM) main result.txt