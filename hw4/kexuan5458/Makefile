CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -Werror -shared -std=c++19 -fPIC
PYBIND_INCLUDE = $(shell python3 -m pybind11 --includes)

PYBIND_INCLUDE = `python3 -m pybind11 --includes`
PYINCLUDE = `python3-config --includes`
TARGET = _matrix`python3-config --extension-suffix`

MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
IBLAS = -lblas

.PHONY: all, matrix, test, clean
all:
	matrix test
matrix: _matrix.cpp
	$(CXX)  $(CXXFLAGS) $(PYBIND_INCLUDE) _matrix.cpp -o $(TARGET) $(IBLAS) $(MKL_INC) $(PYINCLUDE) 
test: matrix
	python3 -m pytest -v -s 

clean:
	rm -rf *.so __pycache__ .pytest*
# python3  test_file.py -v