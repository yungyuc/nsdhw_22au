source:=_vector
testfile:=test_vector_angle.py
*.so: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` $(source).cpp -o $(source)`python3-config --extension-suffix` $(shell python3-config --includes) -Iextern/pybind11/include
test:
	env PYTHONPATH=".:$PYTHONPATH" python3 -m pytest -v
.PHONY: clean
clean:
	rm -rf *.so