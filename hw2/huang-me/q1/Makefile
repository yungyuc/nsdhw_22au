CXX:=clang++

all: main

main.o: main.cpp line.hpp
	$(CXX) $< -o $@ -c

line.o: line.cpp line.hpp
	$(CXX) $< -o $@ -c

main: main.o line.o
	$(CXX) $^ -o $@

run: main
	./main

check: main
	./main > result.txt

.PHONY: clean
clean:
	rm result.txt main *.o || true
