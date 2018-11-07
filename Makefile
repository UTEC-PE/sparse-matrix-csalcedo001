CXX = g++
CXXFLAGS = -std=c++17
SRC = main.cpp
HEADS = matrix.h node.h

all: run

main: $(SRC) $(HEADS)
	$(CXX) $(CXXFLAGS) $< -o $@

run: main
	./main
