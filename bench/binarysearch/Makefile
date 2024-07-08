CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -pedantic  -Wno-unused-variable -Wno-unused-function -mavx2
CXXLIBS = -lbenchmark -lpthread

BIN = benchmark
BIN_DBG = benchmark_dbg
BIN_TEST = benchmark_test


run: $(BIN)
	@bear -- ./$(BIN)

test: $(BIN_TEST)
	./$(BIN_TEST)

dbg: $(BIN_DBG)
	gdb ./$(BIN)

gen: $(BIN)
	@./$(BIN) --benchmark_format=csv | tee benchmark.csv

plt: benchmark.csv
	@python3 ../plt/plot.py -f benchmark.csv

$(BIN): main.cpp
	@$(CXX) $(CXXFLAGS) -o $@ $< $(CXXLIBS)

$(BIN_DBG): main.cpp
	$(CXX) $(CXXFLAGS) -g -o $(BIN_DBG) main.cpp $(CXXLIBS)

$(BIN_TEST): main.cpp
	$(CXX) $(CXXFLAGS) -DTEST -o $(BIN_TEST) main.cpp $(CXXLIBS)

clean:
	rm -f $(BIN) $(BIN_DBG) $(BIN_TEST) benchmark.csv
