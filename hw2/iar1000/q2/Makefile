CC = c++
CC_FLAGS = -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`

# mac: c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
# linux: c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)

all: angles.so
	@echo build successfull

angles.so: angles.cpp
	$(CC) $(CC_FLAGS) angles.cpp -o angles.so

test: all
	pytest test.py

clean:
	rm angles.so
	rm -rf __pycache__ .pytest_cache