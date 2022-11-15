all: _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` -lblas -I/usr/include/mkl -I/usr/include/python3.8

test:all
	python3 -m pytest

clean:
	rm -rf *.so __pycache__