CC = $(CMD_PREFIX)g++
DES_DIR = build

.PHONY: build run check clean 
default: run


run: build
	./$(DES_DIR)/line.out

check: build
	./$(DES_DIR)/line.out > result.txt

build: line.cpp
	mkdir -p $(DES_DIR)
	$(CC) line.cpp -o $(DES_DIR)/line.out

clean:
	rm -f main *.o result.txt