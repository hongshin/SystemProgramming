all: ping-server client 

ping-server: ping-server.c
	gcc -pthread -o ping-server ping-server.c

client: client.c
	gcc -o client client.c

clean:
	rm -rf client ping-server 
