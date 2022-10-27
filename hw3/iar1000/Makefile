CC = g++
CC_FLAGS = -O3 -Wall -shared -std=c++11 -fPIC
MKL_INCL = -I/usr/include/mkl
BLAS_INCL = -lblas
PY_INCL = -I/usr/include/python3.8
PY_FLAG = -lpython3.8
PYBIND_INCL = `python3 -m pybind11 --includes`
SPEC = `python3-config --extension-suffix`

MKL_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_DIR}/libmkl_def.so \
			${MKL_DIR}/libmkl_avx2.so \
			${MKL_DIR}/libmkl_core.so \
			${MKL_DIR}/libmkl_intel_lp64.so \
			${MKL_DIR}/libmkl_sequential.so \
			-lpthread -lm -ldl \
			-lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

# 	${CXX} ${CXXFLAGS} -I/usr/include/mkl ${PYBIND} _matrix.cpp -o _matrix${PYCONFIG} -I/usr/include/python3.8 ${MKL_LIBS}

# mac: c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
# linux: c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)

all: _matrix.so
	@echo build successfull

mac: 
	$(CC) -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix`

_matrix.so: _matrix.cpp
	export PRELOAD_MKL=1
	$(CC) $(CC_FLAGS) $(MKL_INCL) $(PY_INCL) $(PYBIND_INCL) _matrix.cpp -o _matrix$(SPEC) ${PY_FLAG} ${MKL_LIBS}

test: all
	python3 -m pytest -s -v

clean:
	rm -rf *.so *.o __pycache__ .pytest_cache
