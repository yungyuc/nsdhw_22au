.PHONY: clean
.PHONY: test
build_file_name=_vector$(shell python3-config --extension-suffix)

$(build_file_name): angle.cpp angle.h
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` angle.cpp -o $(build_file_name)
clean: 
	rm -rf *.so __pycache__ .pytest_cache
test:
	python3 unit_test.py -v