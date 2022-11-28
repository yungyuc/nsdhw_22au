CXX=clang++
CXXFLAGS=-O3 -Wall -m64 -shared -std=c++11 -fPIC
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so \
		-lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5
all:
	${CXX} $(CXXFLAGS) -I/usr/include/mkl $(PYBIND) _matrix.cpp -o _matrix$(PYCONFIG) -I/usr/include/python3.8 ${MKL_LIBS} 
test: all
	python3 -m pytest -v -s

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt
