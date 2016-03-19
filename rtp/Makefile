CC=g++
CFALGS=-Wall -g
BIN=librtp.a
#INCLUDE=include
#LIB=lib
OBJS=InetAddress.o Socket.o PollPoller.o Rio.o TcpConnection.o TcpServer.o Timer.o TimerThread.o
STARD=-std=c++11 -rdynamic
$(BIN):$(OBJS)
	ar -crv $@ -o $^
	chmod +x $@
	rm -f *.o
#	cp *.h $(INCLUDE)
#	cp $(BIN) $(LIB)
%.o:%.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(STARD)

.PHONY:clean
clean:
	rm -rf *.o $(BIN)
