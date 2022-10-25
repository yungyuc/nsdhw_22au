TARGET := _vector$(shell python3-config --extension-suffix)
INCLUDES := $(shell python3 -m pybind11 --includes)
CONFIG = `python3-config --includes`
CXXFLAGS := -O3 -Wall -shared -std=c++11 -fPIC $(INCLUDES) $(CONFIG)

$(TARGET): _vector.cpp
	c++ $(CXXFLAGS) $^ -o $(TARGET)

.PHONY: test
test: $(TARGET)
	python3 -m pytest

.PHONY: clean
clean:
	rm -rf *.so __pycache__ .pytest_cache