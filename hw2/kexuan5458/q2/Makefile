CXX=g++
CXXFLAGS:=-O3 -Wall -shared -std=c++11 -fPIC

PYINCLUDE = `python3-config --includes`
PYBIND_INCLUDE = `python3 -m pybind11 --includes`
TARGET = _vector`python3-config --extension-suffix`

main:
	$(CXX) $(CXXFLAGS) $(PYINCLUDE) $(PYBIND_INCLUDE) _vector.cpp -o $(TARGET)
.PHONY: test clean
test:
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache*