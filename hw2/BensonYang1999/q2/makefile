CXX = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYINCLUDE = `python3-config --includes`

main:
	$(CXX) $(CFLAGS) $(PYINCLUDE) `python3 -m pybind11 --includes` angle.cpp -o angle`python3-config --extension-suffix`

clean:
	rm -rf *.so .pytest_cache* __pycache__*

test:
	python3 -m pytest
