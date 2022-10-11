build:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` q2.cpp -o _vector`python3-config --extension-suffix` `python3-config --includes`
test:
	python3 -m pytest test_q2.py
clean:
	rm *.so
