
CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC
INC = -I./ -I /usr/include/python3.8 -I /usr/include/numpy  -I /usr/include  -I`python3 -c 'import numpy;print(numpy.get_include())'`
MKL = -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lmkl_rt

test:all
	python3 -m pytest

clean:
	rm -rf *.so __pycache__  .pytest_cache __pycache__



all:
	cp ./matrix.hpp ../matrix.hpp
	g++  $(CFLAGS) `/usr/bin/python3-config --includes` `python3 -m pybind11 --includes` ../mod.cpp -o _matrix.so $(MKL) $(INC)
	rm ../matrix.hpp
	
