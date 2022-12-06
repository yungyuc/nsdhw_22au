CXX := g++
SRCS := mod.cpp
TARGET := _matrix$(shell python3-config --extension-suffix)
INCLUDES := $(shell python3 -m pybind11 --includes) \
			$(shell python3-config --includes) \
			-I/usr/include/mkl
LIBRARYS := -L/usr/lib/x86_64-linux-gnu/mkl -lblas
CXXFLAGS := -O3 -Wall -shared -std=c++17 -fPIC $(INCLUDES) $(LIBRARYS)

$(TARGET): $(SRCS)
	$(CXX) $^ -o $(TARGET) $(CXXFLAGS)

.PHONY: test, clean
test: $(TARGET)
	python3 -m pytest

clean:
	rm -rf $(TARGET) $(OBJS) __pycache__ .pytest_cache performance.txt
