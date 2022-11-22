BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas
MATRIX=_matrix$(shell python3-config --extension-suffix)
PYTHON_INCLUDES=$(shell python3-config --includes)
$(MATRIX): _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(PYTHON_INCLUDES) -Iextern/pybind11/include _matrix.cpp -o $(MATRIX) -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

test: $(MATRIX)
	python3 -m pytest -v

clean:
	rm -rf __pycache__ *.so