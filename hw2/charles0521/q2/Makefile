CONFIG = `python3-config --extension-suffix`
CXXFLAGS = g++ -O3 -Wall -shared -std=c++11 -fPIC
INCLUDE = `python3 -m pybind11 --includes`
PYBIND = `python3-config --extension-suffix`
PY = -I/usr/include/python3.8

all:
	$(CXXFLAGS) $(INCLUDE) _vector.cpp -o _vector$(PYBIND)
test:
	python3 -m pytest

clean:
	rm *.so
	rm -rf __pycache__ .pytest_cache