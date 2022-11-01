CXX=g++
FLAG = -O3 -Wall -shared -std=c++11 -fPIC
INCLUDE = $(shell python3 -m pybind11 --includes)
SUFFIX = $(shell python3-config --extension-suffix)
CONFIG = $(shell python3-config --includes)
mklinc=/usr/include/mkl


.PHONY: all test clean

all: _matrix

_matrix: _matrix.cpp
	$(CXX) $(FLAG) $(INCLUDE) $< -o $@$(SUFFIX) $(CONFIG) -I$(mklinc) -lblas

test:
	$(CXX) $(FLAG) $(INCLUDE) $< -o $@$(SUFFIX) $(CONFIG) -I$(mklinc) -lblas
	python3 -m pytest -v

clean:
	rm -rf __pycache__ .pytest_cache *.so
