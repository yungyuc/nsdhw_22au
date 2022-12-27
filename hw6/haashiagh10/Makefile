##
# NSD HW6
#
# @file
# @version 0.1

CXX = g++

MODULE = _matrix
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)
LBS = -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): _matrix.cpp _matrix.hpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) -Iextern/pybind11/include _matrix.cpp -o $(SOFILE) $(LBS)

test: $(SOFILE)
	python3 -m pytest -v test_matrix.py

clean:
	rm -rf $(SOFILE) __pycache__ .pytest_cache performance.txt