CC=g++
CCFLAGS=-O3 -Wall -m64 -shared -std=c++11 -fPIC
PATHS=-I/usr/include/mkl -I/usr/include/python3.8
#PYINCLUDE=`python3 -m pybind11 --includes`
PYINCLUDE=`python3-config --includes`
SPEC=`python3-config --extension-suffix`

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=$(MKL_LIB_DIR)/libmkl_def.so \
			$(MKL_LIB_DIR)/libmkl_avx2.so \
			$(MKL_LIB_DIR)/libmkl_core.so \
			$(MKL_LIB_DIR)/libmkl_intel_lp64.so \
			$(MKL_LIB_DIR)/libmkl_sequential.so \
			-lpthread -lm -ldl \
			-lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

run:
	$(CC) $(CCFLAGS) $(PATHS) $(PYINCLUDE) matrix.cpp -o _matrix$(SPEC) $(MKL_LIBS)

test: run
	env PTHONPATH=".:PTHONTATH" python3 -m pytest -v
	@touch performance.txt
	@python3 matrix_test.py >| performance.txt
clean:
	@rm -rf *.so __pycache__ .pytest_cache performance.txt
