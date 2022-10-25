all: _vector.so

_vector.so: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix` `python3-config --includes`

clean:
		rm -rf *.so __pycache__

test:
	python3 -m pytest
