CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++17 -fPIC
SRC = _matrix.cpp
EXE = _matrix

PYINCLUDE = /usr/include/python3.8 
PYBDINCLUDE = $(shell python3 -m pybind11 --includes)
PYCONFIG = $(shell python3-config --extension-suffix)

MKLINCLUDE = /usr/include/mkl
MKLFLAGS = -lblas

all: $(SRC)
	$(CXX) $(CXXFLAGS) -I$(MKLINCLUDE) -I$(PYINCLUDE) $(PYBDINCLUDE) $(SRC) -o $(EXE)$(PYCONFIG) $(MKLFLAGS)

test: all
	python3 -m pytest

clean:
	rm -rf $(EXE) __pycache__ .pytest* *.txt *.o *.so
