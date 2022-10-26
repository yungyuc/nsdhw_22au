all:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix`
test:
	python3 -m pytest test_matrix.py

clean:
	rm -rf *.txt *.o *.so __pycache__