CC = g++
C_FLAGS = -O3 -Wall -shared -std=c++11

.PHONY: test clean
default: test

_vector.so: _vector.cpp
	$(CC) $(C_FLAGS) -fPIC -I/usr/include/python3.8 `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`

test: _vector.so
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache
