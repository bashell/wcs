.PHONY: clean

CXX = g++
CXXFLAGS = -Wall -g -std=c++11
LIBS = -lmysqlclient -lpthread
BIN = makeDB
OBJS = makeDB.o MyDatabase.o MyConfiguration.o StringUtils.o

$(BIN):$(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS)

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(BIN)
