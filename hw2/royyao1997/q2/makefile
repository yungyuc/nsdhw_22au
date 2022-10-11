CXX := g++
CXXFLAGS := -O3 -Wall -shared -std=c++11 -fPIC
TARGET = _vector.*.so

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) `python3 -m pybind11 --includes` $< -o _vector`python3-config --extension-suffix` `python3-config --includes`

test: vector_test.py
	python3 -m pytest vector_test.py

.PHONY: clean
clean:
	rm -rf *.o main *.so __pycache__ .pytest_cache