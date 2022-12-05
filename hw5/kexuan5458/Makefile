CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -shared -std=c++14 -fPIC
PYBIND_INCLUDE = `python3 -m pybind11 --includes`
PYINCLUDE = `python3-config --includes`
TARGET = _matrix`python3-config --extension-suffix`
NUMPYINCLUDE = `python3 -c 'import numpy; print(numpy.get_include())'`

MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
IBLAS = -lblas
PYFLAGS = -lpython3.8

.PHONY: main, test, clean

main: 
	$(CXX) $(CXXFLAGS) $(MKL_INC) $(PYINCLUDE) -I$(NUMPYINCLUDE) -I./ $(PYBIND_INCLUDE) ../mod.cpp -o $(TARGET) $(PYFLAGS) $(IBLAS)

test: main
	  python3 -m pytest -v -s
clean:
	  rm -rf *.so __pycache__ .pytest*