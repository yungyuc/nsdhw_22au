CC = g++
C_FLAGS = -O3 -Wall -shared -std=c++17 -fPIC

INCLUDE = $(shell python3 -m pybind11 --includes)
PYCONFIG = $(shell python3-config --includes)
PYSUFFIX = $(shell python3-config --extension-suffix)
MKL_FLAGS = -I/usr/include/mkl -lblas

.PHONY: matrix test clean
default: matrix

matrix: _matrix.h _matrix.cpp
	$(CC) $(C_FLAGS) $(INCLUDE) $(PYBIND) _matrix.cpp -o _matrix$(PYSUFFIX) $(PYCONFIG) $(MKL_FLAGS)

test: matrix
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache