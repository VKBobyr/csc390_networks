CC = g++
CPPFLAGS = -g -I.
all: client server
server: server.o
server.o: server.cpp
client: client.o
client.o: client.cpp
clean:
	rm server
	rm client
	rm *.o
