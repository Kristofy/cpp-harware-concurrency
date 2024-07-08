CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic 
SRC = $(wildcard *.cpp)
BIN = tmp

gen: $(BIN)
	@bear -- ./$(BIN)

$(BIN): $(SRC)
	@$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)
