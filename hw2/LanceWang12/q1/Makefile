CXX:=g++
exe:=main
result:=result.txt
obj:=line

$(exe): $(obj).cpp
	$(CXX) -o $(exe) $(obj).cpp
run:
	./$(exe)
check:
	./$(exe) > $(result)
.PHONY: clean
clean:
	rm -rf $(exe)
	rm -rf $(result)