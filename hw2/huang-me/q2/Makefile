FLAGS=-O3 -Wall -shared -std=c++11 -fPIC
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`

all: _vector

_vector: _vector.cpp
	clang++ $< -o $@$(PYCONFIG) $(FLAGS) $(PYBIND) -I /usr/include/python3.8

test:
	python3 -m pytest -v

.PHONY: clean
clean:
	rm -rf __pycache__ *.so
