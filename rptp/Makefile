.PHONY:clean

CXX=g++
CXXFLAGS=-Wall -g
BIN=librptp.a
LIB=../lib/
OBJS=InetAddress.o Socket.o PollPoller.o Rio.o TcpConnection.o TcpServer.o Timer.o TimerThread.o
STARD=-std=c++11 -rdynamic
$(BIN):$(OBJS)
	ar -crv $@ -o $^
	chmod +x $@
	cp $(BIN) $(LIB)
%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(STARD)

clean:
	rm -rf *.o $(BIN)
