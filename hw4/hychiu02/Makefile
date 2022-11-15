CXX           := g++
CXXFLAGS      := -O3 -Wall -shared -std=c++17 -fPIC
MKLINCLUDE    := /usr/include/mkl
MKLFLAGS      := -lblas
PYINCLUDE     := /usr/include/python3.8
PYBINDINCLUDE := `python3 -m pybind11 --includes`
PYCONFIG      := `python3-config --extension-suffix`


test: all
	python3 -m pytest

.PHONY: all, clean

all:
	$(CXX) $(CXXFLAGS) -I$(MKLINCLUDE) -I$(PYINCLUDE) ${PYBINDINCLUDE} _matrix.cpp -o _matrix${PYCONFIG} $(MKLFLAGS)

clean:
	rm -rf *.o *.so .pytest_cache __pycache__

statistic: clean
	$(CXX) -O3 -Wall -std=c++11 -fPIC statistic.cpp -I$(MKLINCLUDE) -o statistic.out $(MKLFLAGS)
	./statistic.out | tee performance.txt