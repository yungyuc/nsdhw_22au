FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
#MKLROOT ?= /home/ben/anaconda3/envs/nsd
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_def.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	-lpthread -lm -ldl -fPIC `python3 -m pybind11 --includes`

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 -shared\
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

BINS := _matrix

.PHONY: default
default: ${BINS}

%: %.cpp
	${CXX} ${CXXFLAGS} -o _matrix`python3-config --extension-suffix` _matrix.cpp

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS} *.so *.txt __pycache__

.PHONY: test
test: 
	make;pytest -v