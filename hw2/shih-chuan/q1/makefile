main: main.o
	g++ $^ -o $@

main.o: main.cpp
	g++ -c $< -o $@

.PHONY: run
run: main
	./main

.PHONY: check
check: main
	./main > result.txt

.PHONY: clean
clean:
	rm -f main main.o result.txt