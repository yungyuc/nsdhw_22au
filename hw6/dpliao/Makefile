CXX = g++
CFLAGS = -shared -std=c++17 -O3 -Wall -fPIC
PWD = $(shell pwd)

LIBS = -I/usr/include/python3.8 -I/usr/include/pybind11 -L/usr/include/mkl/intel64 -I/usr/include/mkl/ -lmkl_rt -lpthread -lm -ldl -lblas -lpython3.8

_matrix:
	$(CXX) $(CFLAGS) $(MKL_Flags) `python3 -m pybind11 --includes` mod.cpp -o _matrix`python3-config --extension-suffix` $(LIBS)

.PHONY: test clean

test: _matrix
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache