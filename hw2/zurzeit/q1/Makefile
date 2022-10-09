q1_code: q1_code.o
	g++ q1_code.o -o q1_code
q1_code.o: q1_code.cpp
	g++ -c q1_code.cpp
run:
	./q1_code
check:
	./q1_code > result.txt
clean:
	rm *.o q1_code