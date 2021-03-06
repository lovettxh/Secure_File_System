CC = g++
LFLAGS = 
CFLAGS =-c -Wall -std=c++11 -O2
OBJS = client.o utilities.o login.o
EXE = client

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LFLAGS)

client.o: client.cpp login.o utilities.o
	g++ client.cpp -o client.o $(CFLAGS)

login.o: login.cpp login.h utilities.o
	g++ login.cpp -o login.o $(CFLAGS)

utilities.o: utilities.cpp utilities.h
	g++ utilities.cpp -o utilities.o $(CFLAGS)

clean:
	@rm -rf *.o