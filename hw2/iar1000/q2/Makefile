CC = c++
CC_FLAGS = -O3 -Wall -shared -std=c++11 -fPIC -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8 `python3 -m pybind11 --includes`
SPEC = `python3-config --extension-suffix`

# mac: c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
# linux: c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)

all: _vector.so
	@echo build successfull

_vector.so: _vector.cpp
	$(CC) $(CC_FLAGS) _vector.cpp -o _vector$(SPEC)

test: all
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache
