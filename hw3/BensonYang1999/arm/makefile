CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
MKLFLAGS = -lblas
PYINCLUDE = /usr/include/python3.10
PYFLAGS = -lpython3.10

main:
	$(CXX) $(CXXFLAGS) -I$(PYINCLUDE) `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` $(PYFLAGS) $(MKLFLAGS)
	
test: main
	python3 -m pytest arm_test_matrix.py
	
clean:
	rm -rf *.out *.so .pytest_cache* __pycache__*

statistic: clean
	$(CXX) -O3 -Wall -std=c++11 -fPIC statistic.cpp -o statistic.out $(MKLFLAGS)
	./statistic.out | tee performance_arm.txt
