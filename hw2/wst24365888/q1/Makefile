CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++11 -O2
TARGET=main

.PHONY: all clean check run

all: $(TARGET)

$(TARGET): main.o line.o
	@$(CXX) -o $(TARGET) main.o line.o $(CXXFLAGS)

%.o: %.cpp
	@$(CXX) -c $< $(CXXFLAGS)

clean:
	@rm -f *.o $(TARGET)

check: $(TARGET)
	@./$(TARGET) > result.txt

run: $(TARGET)
	@./$(TARGET)