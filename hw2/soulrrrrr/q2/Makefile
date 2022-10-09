CXX=g++
INCLUDE = $(shell python3 -m pybind11 --includes)
FLAG = -O3 -Wall -shared -std=c++11 -fPIC
SUFFIX = $(shell python3-config --extension-suffix)
CONFIG = $(shell python3-config --includes)

.PHONY: all test clean

all: _vector

_vector: _vector.cpp
	$(CXX) $(FLAG) $(INCLUDE) $< -o $@$(SUFFIX) $(CONFIG)

test:
	python3 -m pytest -v

clean:
	rm -rf __pycache__ .pytest_cache *.so