CC = g++
INCLUDE = `python3 -m pybind11 --includes`
POSFIX = `python3-config --extension-suffix`
CONFIG = `python3-config --includes`

all:*.so

*.so: _vector.cpp 
	$(CC) -O3 -Wall -shared -std=c++11 -fPIC $(INCLUDE) _vector.cpp -o _vector$(POSFIX) $(CONFIG)

test:
	python3 -m pytest

clean:
	rm -f *.so
