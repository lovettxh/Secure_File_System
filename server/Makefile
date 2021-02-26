CC = g++
LFLAGS = 
CFLAGS =-c -Wall -std=c++11 -O2
OBJS = server.o cmd_line.o utilities.o
EXE = server

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LFLAGS)

utilities.o: utilities.cpp utilities.h
	g++ utilities.cpp -o utilities.o $(CFLAGS)

cmd_line.o: cmd_line.cpp cmd_line.h utilities.o
	g++ cmd_line.cpp -o cmd_line.o $(CFLAGS)

server.o: cmd_line.cpp cmd_line.h server.cpp server.h utilities.o
	g++ server.cpp -o server.o $(CFLAGS)


clean:
	@rm -rf *.o