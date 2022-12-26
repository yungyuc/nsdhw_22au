CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -shared -fPIC `python3 -m pybind11 --includes` -I/usr/include/mkl $(shell python3-config --includes) 
source = $(wildcard *.cpp)
files = $(addprefix $(obj_d)/, $(addsuffix .o, $(basename $(notdir $(source)))))
exe = _matrix$(shell python3-config --extension-suffix)
obj_d = obj
LIB = -L/usr/lib/x86_64-linux-gnu/mkl -lblas

all: create_obj_dir $(exe)

create_obj_dir:
	mkdir -p $(obj_d)

$(obj_d)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(exe): $(files)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

test: all
	python3 -m pytest -q test_matrix.py

clean:
	rm -rf $(exe) $(obj_d)