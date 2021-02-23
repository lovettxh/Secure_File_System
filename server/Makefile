CC = g++
LFLAGS = 
CFLAGS =-c -Wall -std=c++11 -O2
OBJS = server.o cmd_line.o
EXE = server

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LFLAGS)

cmd_line.o: cmd_line.cpp cmd_line.h
	g++ cmd_line.cpp -o cmd_line.o $(CFLAGS)

server.o: cmd_line.cpp cmd_line.h server.cpp server.h
	g++ server.cpp -o server.o $(CFLAGS)


clean:
	@rm -rf *.o