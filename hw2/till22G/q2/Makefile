CC = c++
FLAGS = -O3 -Wall -shared -std=c++11 -fPIC -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8 
PYINCLUDE = `python3 -m pybind11 --includes`
SPEC = `python3-config --extension-suffix`

run: _vector.so
	@echo run

_vector.so: calculate_angle.cpp
	$(CC) $(FLAGS) ${PZINCLUDE} calculate_angle.cpp -o _vector$(SPEC)

test:
	env PTHONPATH=".:PTHONTATH" python3 -m pytest -v 

clean:
	rm -r *.so __pycache__ .pytest_cache 
