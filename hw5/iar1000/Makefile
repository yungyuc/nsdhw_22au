CXX=g++
CXXFLAGS=-O3 -Wall -m64 -shared -std=c++17 -fPIC
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`
NUMPYINCLUDE = `python3 -c 'import numpy; print(numpy.get_include())'`
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
			${MKL_LIB_DIR}/libmkl_avx2.so \
			${MKL_LIB_DIR}/libmkl_core.so \
			${MKL_LIB_DIR}/libmkl_intel_lp64.so \
			${MKL_LIB_DIR}/libmkl_sequential.so \
			-lpthread -lm -ldl \
			-lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

all:
	${CXX} ${CXXFLAGS} -I/usr/include/mkl ${PYBIND} -I{NUMPYINCLUDE} -I./  ../mod.cpp -o _matrix${PYCONFIG} -I/usr/include/python3.8 ${MKL_LIBS}


test: all
	python3 -m pytest -s -v

clean:
	rm -rf *.so *.o __pycache__ .pytest_cache 
