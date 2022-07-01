all: new_client new_server

client: new_client.c
	gcc -o new_client new_client.o

server: server.c
	gcc -o new_server new_server.o
	
clean:
	rm new_server new_client
