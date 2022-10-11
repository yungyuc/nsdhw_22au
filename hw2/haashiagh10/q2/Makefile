CXX = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
INCLUDE = `python3 -m pybind11 --includes`
CONFIG = `python3-config --extension-suffix`
PY = -I /usr/include/python3.8

all:
	$(CXX) $(CFLAGS) $(INCLUDE) _vector.cpp -o _vector$(CONFIG) $(PY)

test:
	python3 -m pytest

clean:
	rm *.so
	rm -rf __pycache__ .pytest_cache
