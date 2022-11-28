.PHONY: test
.PHONY: clean

build_file_name=_matrix$(shell python3-config --extension-suffix)

$(build_file_name): multiplication.cpp matrix.o makefile
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3-config --includes`  `python3 -m pybind11 --includes` \
	multiplication.cpp matrix.o  -o $(build_file_name) -L/usr/lib/x86_64-linux-gnu -lmkl_rt

matrix.o: matrix.h matrix.cpp
	g++ -O3 -Wall -std=c++11 -c matrix.cpp -o matrix.o

clean:
	rm -R -f *.so *.o __pycache__ .pytest_cache performance.txt

test: $(build_file_name)
	python3 unit_test.py -v