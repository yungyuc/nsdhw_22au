all: matrix.hpp
	g++ -O3 -Wall -shared  -fPIC `python3 -m pybind11 --includes`  ../mod.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` -lblas -I/usr/include/mkl -I/usr/include/python3.8 -I./ -I`python3 -c "import numpy; print(numpy.get_include())"`

test:
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ 
