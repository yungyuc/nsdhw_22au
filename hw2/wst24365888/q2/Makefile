CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -O2 -shared -fPIC
PYBIND_INCLUDE = $(shell python3 -m pybind11 --includes)
PY_CONFIG = $(shell python3-config --includes)
TARGET = _vector$(shell python3-config --extension-suffix)

.PHONY: all test clean

all: $(TARGET)

$(TARGET) : wrapper.cpp _vector.cpp 
	@$(CXX) wrapper.cpp _vector.cpp -o $(TARGET) $(CXXFLAGS) $(PYBIND_INCLUDE) $(PY_CONFIG)

test:
	@python3 -m pytest -v

clean:
	@rm -f $(TARGET)
	@rm -rf .pytest_cache __pycache__