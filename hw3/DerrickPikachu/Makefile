CXX = g++
PYBIND = -I/usr/include/python3.8
MKL = -I/usr/include/mkl -lmkl_rt
# MKL = -I/usr/include/mkl ${MKLFLAGS}
PYBIND_SUFFIX = $(shell python3-config --extension-suffix)
CXX_FLAG = -O3 -Wall -std=c++11 -lpthread -ldl -lm

.PHONY: default
default: matrix

matrix.o: matrix.h matrix.cpp
	$(CXX) $(CXX_FLAG) -c matrix.cpp -fPIC $(MKL) -o matrix.o

matrix_bind.o: matrix_bind.cpp
	$(CXX) $(CXX_FLAG) -c matrix_bind.cpp -fPIC $(PYBIND) -o matrix_bind.o

matrix: matrix.o matrix_bind.o
	$(CXX) $(CXX_FLAG) -shared matrix_bind.o matrix.o $(MKL) $(PYBIND) -o _matrix$(PYBIND_SUFFIX)

.PHONY: clean
clean:
	rm -rf *.o *.so __pycache__ .pytest_cache

.PHONY: test
test: matrix
	python3 -m pytest . -v