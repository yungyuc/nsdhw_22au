CXX = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
INCLUDE = `python3 -m pybind11 --includes`
CONFIG = `python3-config --extension-suffix`
PY = -I /usr/include/python3.8
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas

all: matrix test

matrix: _matrix.cpp
	$(CC)  $(CFLAGS) $(INCLUDE) _matrix.cpp -o _matrix$(POSFIX)  -lblas $(MKLINCLUDE) $(CONFIG) 

test: matrix
	python3  test_file.py -v
clean:
	rm -f *.so 
	rm -rf __pycache* .pytest*