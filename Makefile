CXX = g++
FLAGS = -Wall -g -fsanitize=address -O3 -fopenmp
SRC = main.cpp aco.cpp
TARGET = x

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(FLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
