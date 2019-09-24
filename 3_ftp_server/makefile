CC = g++
CPPFLAGS = -g -fsanitize=address -std=c++17
IMPORTS = -Icommunication -Iutils
UTILS = communication/Communicator.hpp utils/Algs.hpp

all: client server

client: client.o 
	$(CC) $(CPPFLAGS) compiled/client.o  -o c.out

client.o: client/Client.cpp client/Client.hpp $(UTILS)
	$(CC) $(CPPFLAGS) $(IMPORTS) -c  client/Client.cpp -o compiled/client.o

server: server.o
	$(CC) $(CPPFLAGS) compiled/server.o  -o s.out

server.o: server/Server.cpp server/Server.hpp $(UTILS)
	$(CC) $(CPPFLAGS) $(IMPORTS) -c server/Server.cpp -o compiled/server.o

clean:
	rm *.out compiled/*