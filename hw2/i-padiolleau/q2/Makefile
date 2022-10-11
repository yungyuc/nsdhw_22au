default: _vector.so
	@echo building...

_vector.so: _vector.cpp
	c++ -O3 -Wall -shared -std=c++11 -fPIC -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8 `python3 -m pybind11 --includes` _vector.cpp -o _vector$(python3-config --extension-suffix)

test: all
	python3 -m pytest

clean:
	rm -rf *.so _pycache_ .pytest_cache