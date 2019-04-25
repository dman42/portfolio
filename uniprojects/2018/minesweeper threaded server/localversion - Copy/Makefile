CC=c99
CFLAGS = -Wall -pedantic
LDFLAGS = -pthread

all: server client

server: server.o communication.o
	$(CC) -o server server.o communication.o $(LDFLAGS) $(CFLAGS)

server.o : server.c communication.h
	$(CC) -c server.c $(LDFLAGS) $(CFLAGS)
	
client: client.o communication.o
	$(CC) -o client client.o communication.o $(LDFLAGS) $(CFLAGS)

client.o : client.c communication.h
	$(CC) -c client.c $(LDFLAGS) $(CFLAGS)
	
clean :
	rm client.o server.o communication.o
	
.PHONY: clean all