#.PHONY: debug
#debug: 
#	g++ main.cpp -o main.exe -std=c++17 -lblas -I/usr/include/mkl
#	./main.exe

.PHONY: test
test: build
	python3 -m pytest

.PHONY: build
build:
	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` main.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` -lblas -I/usr/include/mkl
	# g++ main.cpp -o main.exe -std=c++17
clean:
	rm -rf *.exe *.so __pycache__ .pytest*

