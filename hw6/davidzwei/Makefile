CXX = g++
CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC
INCLUDE = `python3 -m pybind11 --includes`
CONFIG = `python3-config --extension-suffix`
PY = -I /usr/include/python3.8
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas

all: 
	$(CXX) $(CFLAGS) $(INCLUDE) _matrix.cpp -o _matrix$(CONFIG) $(PY) $(MKL_LIB) $(MKL_INC)

clean:
	rm -rf __pycache__ .pytest_cache _matrix*.so
