all: _vector.so

_vector.so: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector.so

clean:
		rm -rf *.so __pycache__

test:
	python3 -m pytest
