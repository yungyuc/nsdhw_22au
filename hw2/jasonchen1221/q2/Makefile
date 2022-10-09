.PHONY: test clean

CXX = g++
FLAGS = -O3 -Wall -shared -std=c++11 -fPIC

INCLUDE = `python3 -m pybind11 --includes`
POSFIX = `python3-config --extension-suffix`
CONFIG = `python3-config --includes`

_vector*.so: _vector.cpp
	$(CXX) $(FLAGS) $(INCLUDE) _vector.cpp -o _vector$(POSFIX) $(CONFIG)

test:
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest*