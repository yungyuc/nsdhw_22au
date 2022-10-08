CC=g++

.PHONY: all clean check run

all: main

main: main.o line.o
	@$(CC) -o main main.o line.o

%.o: %.cpp
	@$(CC) -c $<

clean:
	@rm -f *.o main

check: main
	@./main > result.txt

run: main
	@./main