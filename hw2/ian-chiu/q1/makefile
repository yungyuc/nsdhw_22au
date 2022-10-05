CXX := g++
SRCS := main.cpp Line.cpp
OBJS := $(SRCS:%.cpp=%.o)
TARGET := q1

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: check
check: $(TARGET)
	./$(TARGET) > result.txt

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) result.txt
