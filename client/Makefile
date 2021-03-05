CC = g++
LFLAGS = 
CFLAGS =-c -Wall -std=c++11 -O2
OBJS = client.o utilities.o
EXE = client

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LFLAGS)

client.o: client.cpp
	g++ client.cpp -o client.o $(CFLAGS)

utilities.o: utilities.o utilities.h
	g++ utilities.cpp -o utilities.o $(CFLAGS)

clean:
	@rm -rf *.o