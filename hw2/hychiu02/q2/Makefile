CXX = g++
CXX_FLAGS = -O3 -Wall -shared -std=c++11 -fPIC `python1 -m pybind11 --includes` -Iextern/pybind11/include

all: _vector.so
	@echo make successfully

_vector.so: _vector.cpp
	$(CXX) ${CXX_FLAGS} _vector.cpp -o _vector.so

test: all
	pytest test_angle.py

.PHONY: clean

clean:
	rm -rf *.so __pycache__ .pytest_cache
