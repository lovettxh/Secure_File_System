CC = g++
LFLAGS = -lcrypto
CFLAGS =-c -Wall -std=c++11 -O2
OBJS = server.o cmd_line.o utilities.o directory.o security.o
EXE = server

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LFLAGS)

utilities.o: utilities.cpp utilities.h
	g++ utilities.cpp -o utilities.o $(CFLAGS)

cmd_line.o: cmd_line.cpp cmd_line.h utilities.o
	g++ cmd_line.cpp -o cmd_line.o $(CFLAGS)

directory.o: directory.cpp directory.h cmd_line.o utilities.o
	g++ directory.cpp -o directory.o $(CFLAGS)

security.o: directory.o security.cpp security.h
	g++ security.cpp -o security.o $(CFLAGS)

server.o: server.cpp directory.o security.o
	g++ server.cpp -o server.o $(CFLAGS)


clean:
	@rm -rf *.o