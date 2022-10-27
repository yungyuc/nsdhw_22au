CC := g++
OPT=-O0

CPPFILES=Line.cpp
OBJECTS=Line.o

BINARY=Line

all:${BINARY}

$(BINARY): $(OBJECTS)
	@$(CC) -o $@ $^
%.o:%.cpp
	@$(CC) -c -o $@  $^
run:
	./Line

check:
	@./Line > result.txt
	echo output written to result.txt
clean:
	rm -rf $(BINARY) $(OBJECTS) result.txt


