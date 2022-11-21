.PHONY: test
test: build
	python3 -m pytest validate.py

.PHONY: build
build:
	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` main.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` -lblas -I/usr/include/mkl

clean:
	rm -rf *.exe *.so

