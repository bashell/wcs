.PHONY: clean

CXX = g++
CXXFLAGS = -Wall -g -std=c++11
BIN = makeDict
OBJS = makeDict.o makeDictCh.o makeDictEn.o

$(BIN):$(OBJS)
	$(CXX) -o $(BIN) $(OBJS)

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(BIN)
