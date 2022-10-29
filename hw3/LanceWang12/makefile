CC = g++

MKLROOT = /opt/intel/compilers_and_libraries_2018.2.199/linux/mkl
CFlag = -std=c++11 -Wl,--no-as-needed -Wall -O3 \
		-I$(MKLROOT)/include -L$(MKLROOT)/lib/intel64 \
        -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core \
        -liomp5 -lpthread -lm \
		-fPIC -shared $(shell python3 -m pybind11 --includes) -I/usr/include/mkl
suffix = $(shell python3-config --extension-suffix)

all: _matrix$(suffix)

_matrix$(suffix): _matrix.cpp
	$(CC) $(CFlag) _matrix.cpp -o _matrix$(suffix)

test: all
	python3 -m pytest -q test_matrix.py

clean:
	rm -rf *.so
