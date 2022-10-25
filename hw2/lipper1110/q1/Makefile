CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = main
RESULT = result.txt

run: $(TARGET)

$(TARGET): $(TARGET).cpp *.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

check: $(TARGET)
	./$(TARGET) > $(RESULT)

clean:
	rm $(TARGET) $(RESULT)
