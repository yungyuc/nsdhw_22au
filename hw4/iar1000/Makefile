# ${CXX} ${CXXFLAGS} -I/usr/include/mkl ${PYBIND} _matrix.cpp -o _matrix${PYCONFIG} -I/usr/include/python3.8 ${MKL_LIBS}

# mac: c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
# linux: c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)

CXX=g++
CXXFLAGS = -O3 -Wall -shared -std=c++17 -fPIC
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`
MKLINCLUDE = -I/usr/include/mkl
CONFIG = `python3-config --includes`

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
			${MKL_LIB_DIR}/libmkl_avx2.so \
			${MKL_LIB_DIR}/libmkl_core.so \
			${MKL_LIB_DIR}/libmkl_intel_lp64.so \
			${MKL_LIB_DIR}/libmkl_sequential.so \
			-lpthread -lm -ldl \
			-lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5



all:
	$(CXX)  $(CXXFLAGS) $(PYBIND) _matrix.cpp -o _matrix$(PYCONFIG)  -lblas $(MKLINCLUDE) $(CONFIG) 
test: all
	python3 -m pytest -s -v

clean:
	rm -rf *.so *.o __pycache__ .pytest_cache performance.txt
