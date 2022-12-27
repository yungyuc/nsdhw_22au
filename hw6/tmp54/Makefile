CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -shared -fPIC
INCLUDES = $(shell /usr/bin/env python3-config --includes) \
	$(shell /usr/bin/env python3 -m pybind11 --includes) \
	-I/usr/include/mkl
LIBDIRS = -L/usr/lib/x86_64-linux-gnu/mkl
CXXLDFLAGS = -lblas

EXT = $(shell /usr/bin/env python3-config --extension-suffix)

SRC = _matrix.cpp
OBJ = $(patsubst %.cpp,%$(EXT),$(SRC))

.PHONY: all stub test clean

all: $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBDIRS) -o $@ $^ $(CXXLDFLAGS)

stub:
	PYTHONPATH=. pybind11-stubgen _matrix -o . --no-setup-py
	cp ./_matrix-stubs/__init__.pyi ./_matrix.pyi
	rm -r ./_matrix-stubs

test: all
	python3 -m pytest

clean:
	rm -rf $(OBJ) __pycache__ .pytest_cache *.pyi
