CXX = g++
PYBIND = $(shell python3 -m pybind11 --includes)
PYBIND_SUFFIX = $(shell python3-config --extension-suffix)

.PHONY: default
default: angle_bind

angle.o: angle.h angle.cpp
	$(CXX) -c angle.cpp -fPIC -o angle.o

angle_bind.o: pybind_angle.cpp angle.h
	$(CXX) -c pybind_angle.cpp -fPIC $(PYBIND) -o angle_bind.o

angle_bind: angle_bind.o angle.o
	$(CXX) -O3 -Wall -shared -std=c++11 angle_bind.o angle.o -o angle$(PYBIND_SUFFIX)

.PHONY: clean
clean:
	rm -f *.o *.so test