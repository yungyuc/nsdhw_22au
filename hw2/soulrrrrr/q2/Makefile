CXX=c++
INCLUDE := $(shell python3 -m pybind11 --includes)
FLAG := -O3 -Wall -shared -std=c++11 -fPIC
SUFFIX := $(shell python3-config --extension-suffix)

all: _vector

_vector: _vector.cpp
	$(CXX) $(FLAG) $(INCLUDE) $< -o $@$(SUFFIX)

test:
	pytest test_vector.py

.PHONY: clean
clean:
	rm -rf __pycache__ .pytest_cache *.so