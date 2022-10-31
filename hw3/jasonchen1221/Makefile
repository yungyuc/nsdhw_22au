CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
SRC = _matrix.cpp

PYFLAGS = $(shell python3 -m pybind11 --includes)
PYBD11 = -I /usr/include/python3.8 -I /usr/include/pybind11 -lpython3.8 `python3 -m pybind11 --includes`

BLAS = -I /usr/include/mkl/ -L /usr/lib/x86_64-linux-gnu/mkl/ -lblas

EXE = _matrix$(shell python3-config --extension-suffix)

all: $(SRC)
	$(CXX) $(CXXFLAGS) $(PYFLAGS) $(PYBD11) $(BLAS) $(SRC) -o $(EXE)

run: $(SRC)
	$(CXX) $(CXXFLAGS) $(PYFLAGS) $(PYBD11) $(BLAS) $(SRC) -o $(EXE)

test: $(SRC)
	$(CXX) $(CXXFLAGS) $(PYBD11) $(SRC) -o $(EXE) $(BLAS)
	python3 -m pytest

clean:
	rm -rf $(EXE) __pycache__ .pytest* *.txt *.o


