CXX = g++
CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC 
COMPILE_SOURCE = -I./ ../mod.cpp
OBJECT = -o _matrix

MKL_INCLUDE = -I/usr/include/mkl
MKL_LIBRARY = -L/usr/lib/x86_64-linux-gnu/ -lblas

PYTHON_CONFIG = `python3-config --extension-suffix`
PYTHON_INCLUDE = -I/usr/include/python3.8
PYBIND_INCLUDE = `python3 -m pybind11 --includes`

NUMPY = -I`python3 -c "import numpy; print(numpy.get_include())"`



all:../mod.cpp
	$(CXX) $(CFLAGS) $(PYBIND_INCLUDE) $(NUMPY) $(COMPILE_SOURCE) $(OBJECT)$(PYTHON_CONFIG) $(PYTHON_INCLUDE) $(MKL_LIBRARY) $(MKL_INCLUDE)

test: 
	python3 -m pytest


.PHONY: clean
clean:
	rm -rf _matrix*