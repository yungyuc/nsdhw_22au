NUMPY = -I$(shell python3 -c 'import numpy;print(numpy.get_include())')
PYBIND = $(shell python3 -m pybind11 --includes)
PYTHON_INC = $(shell python3-config --includes)
MKL = -lblas -I/usr/include/mkl
CXX = g++
EXT_SFX = $(shell python3-config --extension-suffix)

.PHONY: all
all:
	$(CXX) -O3 -Wall -shared -std=c++17 -fPIC $(PYBIND) matrix.cpp  ../mod.cpp -o _matrix$(EXT_SFX) $(PYTHON_INC) $(MKL) $(NUMPY) -I.

.PHONY: debug
debug: 
	$(CXX) main.cpp matrix.cpp -o main.exe -std=c++17 $(MKL)
	./main.exe

.PHONY: clean
clean:
	rm -rf *.exe *.so __pycache__ .pytest*
