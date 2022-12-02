CXX = g++
CXX_FLAGS = -Wall -O3 --std=c++17 -shared -fPIC
PYBIND_INCLUDE = $(shell python3 -m pybind11 --includes)
PY_CONFIG = $(shell python3-config --includes)
NUMPY_INCLUDE = `python3 -c 'import numpy; print(numpy.get_include())'`
MKL_INCLUDE = /usr/include/mkl
MKL_FLAGS = -lblas
TARGET = _matrix$(shell python3-config --extension-suffix)

.PHONY: all clean

all: $(TARGET)

$(TARGET): ../mod.cpp matrix.hpp
	$(CXX) $^ -o $@ -I./ $(CXX_FLAGS) $(PYBIND_INCLUDE) $(PY_CONFIG) -I$(NUMPY_INCLUDE) -I$(MKL_INCLUDE) $(MKL_FLAGS)

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache