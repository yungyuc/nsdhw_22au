CC=g++
CCFLAGS=-O3 -Wall -shared -m64 -std=c++17 -fPIC -I./
PATHS=-I/usr/include/mkl -I/usr/include/python3.8
PYINCLUDE=`python3 -m pybind11 --includes`
NUMPY=`python3 -c 'import numpy;print(numpy.get_include())'`
SPEC=`python3-config --extension-suffix`

MKL_LIB_DIR=-I/usr/include/mkl
MKL_LIBS=-L/usr/lib/x86_64-linux-gnu -lblas -pthread -liomp5 -lm -lmkl_intel_thread -lmkl_intel_lp64 -ldl

run:
	$(CC) $(CCFLAGS) $(PATHS) $(PYINCLUDE) -I $(NUMPY) ../mod.cpp -o _matrix$(SPEC) $(MKL_LIBS)

test: run
	env PTHONPATH=".:PTHONTATH" python3 -m pytest -v
	@touch performance.txt
	@python3 matrix_test.py >| performance.txt
clean:
	@rm -rf *.so __pycache__ .pytest_cache performance.txt
