CC = g++
INCLUDE = `python3 -m pybind11 --includes`
POSFIX = `python3-config --extension-suffix`
CONFIG = `python3-config --includes`
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
MKLCONFIG = -L/usr/lib/x86_64-linux-gnu -lblas -I/usr/include/mkl #-I/opt/intel/mkl


.PHONY: all, matrix, test, clean 

all: matrix test

matrix: _matrix.cpp
	$(CC)  $(CFLAGS) $(INCLUDE) _matrix.cpp -o _matrix$(POSFIX) $(CONFIG) $(MKLCONFIG)

test:
	python3  test_file.py -v
clean:
	rm -f *.so 
	rm -rf __pycache* .pytest*

