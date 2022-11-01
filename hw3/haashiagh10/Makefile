.PHONY: build
build:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` -lblas -I/usr/include/mkl

.PHONY: test
test: build
	python3 -m pytest test_matrix1.py

.PHONY: clean
clean:
	rm -rf *.so