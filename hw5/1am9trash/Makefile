CC = g++
C_FLAGS = -O3 -Wall -shared -std=c++17 -fPIC

INCLUDE = $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) -Iextern/pybind11/include -I/usr/include/mkl
PYCONFIG = $(shell python3-config --includes)
PYSUFFIX = $(shell python3-config --extension-suffix)
MKL_FLAGS = -I/usr/include/mkl -lblas

.PHONY: matrix test clean
default: matrix

matrix: _matrix.hpp mod.cpp
	$(CC) $(C_FLAGS) $(INCLUDE) $(PYBIND) mod.cpp -o _matrix$(PYSUFFIX) $(PYCONFIG) $(MKL_FLAGS)

test: matrix
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache
