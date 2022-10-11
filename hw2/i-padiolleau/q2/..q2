all : clean build

.PHONY : clean build

clean:
	@echo "Cleaning up..."
	@rm -rvf *.o
	@rm -rvf *.out
	@rm -rvf *.txt

run : build r

build :
	@echo "Compilation...creating object"
	@g++ q1_file.cpp -o main.o

r : 
	@./main.o

check :
	@rm -rvf *.txt
	@echo "result.txt created"
	@./main.o > result.txt
