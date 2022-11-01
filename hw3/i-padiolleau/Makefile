default: _vector.so
	@echo building...

_vector.so: main_hw3.cpp
	c++ -O3 -Wall -shared -std=c++11 -fPIC -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8 `python3 -m pybind11 --includes` main_hw3.cpp -o main_hw3$(python3-config --extension-suffix)

test: 
	python3 -m test_main_hw3.py

clean:
	rm -rf *.so _pycache_ .pytest_cache