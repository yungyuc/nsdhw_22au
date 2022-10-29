CXX = g++
CXX_FLAGS = -Wall -Wextra -Werror -std=c++11 -O2 -shared -fPIC
PYBIND_INCLUDE = $(shell python3 -m pybind11 --includes)
PY_CONFIG = $(shell python3-config --includes)
MKL_INCLUDE = /usr/include/mkl
MKL_FLAGS = -lblas
TARGET = _matrix$(shell python3-config --extension-suffix)

.PHONY: all clean check run

all: $(TARGET)

$(TARGET): _matrix.cpp
	$(CXX) $^ -o $@ $(CXX_FLAGS) $(PYBIND_INCLUDE) $(PY_CONFIG) -I$(MKL_INCLUDE) $(MKL_FLAGS)

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache performance.txt

test: $(TARGET)
	python3 -m pytest -s -v

run: $(TARGET)
	./$(TARGET)