.PHONY: test
.PHONY: clean

build_file_name=_matrix$(shell python3-config --extension-suffix)
numpy=$(shell python3 -c "import numpy as np;print(np.get_include())")

$(build_file_name): makefile matrix.hpp matrix.cpp
	g++ -O3 -Wall -shared -std=c++17 -fPIC  `python3 -m pybind11 --includes`  -I$(numpy)  \
	matrix.cpp  -o $(build_file_name) -L/usr/lib/x86_64-linux-gnu -lmkl_rt  -I./ `python3-config --includes`

clean:
	rm -R -f *.so *.o __pycache__ .pytest_cache performance.txt

test: $(build_file_name)
	python3 unit_test.py -v
