CC = g++
INCLUDE = `python3 -m pybind11 --includes`
POSFIX = `python3-config --extension-suffix`
CONFIG = `python3-config --includes`
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC


.PHONY: all, test, clean, vector

all: vector test

vector : _vector.cpp 
	$(CC)  $(CFLAGS) $(INCLUDE) _vector.cpp -o _vector$(POSFIX) $(CONFIG)

test:
	python3 -m pytest -v

clean:
	rm -f *.so 
	rm -rf __pycache* .pytest*