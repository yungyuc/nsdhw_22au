CXX = g++
CFLAGS = -std=c++17 -O2 -Wall -Wextra -shared -fPIC $(shell /usr/bin/env python3 -m pybind11 --includes)
SUFFIX = $(shell /usr/bin/env python3-config --extension-suffix)
SRC = _vector.cpp
OBJ = _vector$(SUFFIX)

.PHONY: all test clean

all: $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CFLAGS) -o $@ $^

test: all
	python3 -m pytest

clean:
	rm -r $(OBJ) .pytest_cache __pycache__
