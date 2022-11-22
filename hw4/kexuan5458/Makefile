CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -Werror -shared -std=c++14 -fPIC
PYBIND_INCLUDE = `python3 -m pybind11 --includes`
PYINCLUDE = `python3-config --includes`
TARGET = _matrix`python3-config --extension-suffix`

MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
IBLAS = -lblas
PYFLAGS = -lpython3.8

.PHONY: matrix, test, clean

matrix: _matrix.cpp
	$(CXX)  $(CXXFLAGS) $(MKL_INC) -I/usr/include/python3.8 $(PYBIND_INCLUDE) _matrix.cpp -o $(TARGET) $(PYFLAGS) $(IBLAS)

test: main
	python3 -m pytest -v -s
clean:
	rm -rf *.so __pycache__ .pytest*