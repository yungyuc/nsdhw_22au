.PHONY: run check clean

CC = g++
C_FLAGS =  pkg-config --cflags python3 -O3 -Wall -shared 

vector.o: vector.cpp
	$(CC) $(CC_FLAGS) vector.cpp -o vector.o

test: all
	pytest vector_test.py

clean:
	rm -rf angle.o __pycache__ .pytest_cache
