all:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` -lblas -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu

test:
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt




