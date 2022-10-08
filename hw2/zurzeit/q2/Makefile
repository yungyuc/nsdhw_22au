build:
	mkdir build
	cmake -S . -B build/
	make -C build/
	find ./build -name "*.so" -type f| xargs cp -vt .
	rm -rf pybind11
test:
	pytest test_q2.py
clean:
	rm -r build
	rm *.so
	ls