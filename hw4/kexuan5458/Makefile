CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -Werror -shared -std=c++11 -fPIC
PYBIND_INCLUDE = $(shell python3 -m pybind11 --includes)

PYBIND_INCLUDE = `python3 -m pybind11 --includes`
PYINCLUDE = `python3-config --includes`
TARGET = _matrix`python3-config --extension-suffix`


MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas

all:
	$(CXX) $(CXXFLAGS) $(PYBIND_INCLUDE) _matrix.cpp -o $(TARGET) $(PYINCLUDE) $(MKL_LIB) $(MKL_INC)

test:
	$(CXX) $(CXXFLAGS) $(PYBIND_INCLUDE) _matrix.cpp -o $(TARGET) $(PYINCLUDE) $(MKL_LIB) $(MKL_INC)
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache*
