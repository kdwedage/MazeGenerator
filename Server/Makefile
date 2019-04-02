CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
PROGS= server
OBJS= server.o recursiveBack.o util.o serialport.o

# executable targets
server: $(OBJS)
	$(CC) $(OBJS) -o server

# object targets
server.o: server.cpp
	$(CC) server.cpp -o server.o $(CFLAGS)

serialport.o: serialport.h serialport.cpp
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

recursiveBack.o: recursiveBack.cpp recursiveBack.h
	$(CC) recursiveBack.cpp -o recursiveBack.o $(CFLAGS)

util.o: util.cpp util.h
	$(CC) util.cpp -o util.o $(CFLAGS)

clean:
	@rm $(OBJS) $(PROGS)
