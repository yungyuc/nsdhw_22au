
CC = g++
INCLUDE = `python3 -m pybind11 --includes`
POSFIX = `python3-config --extension-suffix`
CONFIG = `python3-config --includes`
MKLINCLUDE = -I/usr/include/mkl

.PHONY: all, matrix, test, clean 

all:
	$(CC) -O3 -Wall -shared -std=c++11 -fPIC $(INCLUDE) _matrix.cpp -o _matrix$(POSFIX) -lblas $(MKLINCLUDE) $(CONFIG) 

test:
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt




