CXX=g++
INCLUDE = $(shell python3 -m pybind11 --includes)
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
MKL_FLAGS = -I/usr/include/mkl -lblas
PYCONFIG = $(shell python3-config --includes)
SUFFIX = $(shell python3-config --extension-suffix)

.PHONY: all test clean

all: _matrix

_matrix: _matrix.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) $< -o $@$(SUFFIX) $(PYCONFIG) $(MKL_FLAGS)

test: all
	python3 -m pytest -v

clean:
	rm -rf __pycache__ .pytest_cache *.so