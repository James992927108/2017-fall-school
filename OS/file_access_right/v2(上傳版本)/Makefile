all: server.c client.c myftp.h
	gcc server.c myftp.h -lpthread -o server
	gcc client.c myftp.h -lpthread -o client
clean:
	rm -f server client
