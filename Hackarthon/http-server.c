#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>

void *
worker (void * arg)
{
	int conn ;
	char buf[1024] ;
	char * data = 0x0, * orig = 0x0 ;
	int len = 0 ;
	int s ;

	conn = *((int *)arg) ; 
	free(arg) ;

	data = strdup("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Lenght:19\r\n\r\n<html>Hello</html>\r\n\r\n") ;
	len = strlen(data) ;
	
	orig = data ;
	while (len > 0 && (s = send(conn, data, len, 0)) > 0) {
		data += s ;
		len -= s ;
	}
	shutdown(conn, SHUT_WR) ;
	if (orig != 0x0) 
		free(orig) ;

	return 0x0 ;
}

int 
main (int argc, char const *argv[]) 
{ 
	int listen_fd, new_socket ; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	
	if (argc != 2) {
		fprintf(stderr, "Wrong number of arguments") ;
		exit(EXIT_FAILURE) ;
	}

	char buffer[1024] = {0}; 

	listen_fd = socket(AF_INET /*IPv4*/, SOCK_STREAM /*TCP*/, 0 /*IP*/) ;
	if (listen_fd == 0)  { 
		perror("socket failed : "); 
		exit(EXIT_FAILURE); 
	}
	
	memset(&address, '0', sizeof(address)); 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY /* the localhost*/ ; 
	address.sin_port = htons(atoi(argv[1])); 

	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed : "); 
		exit(EXIT_FAILURE); 
	} 

	while (1) {
		if (listen(listen_fd, 16 /* the size of waiting queue*/) < 0) { 
			perror("listen failed : "); 
			exit(EXIT_FAILURE); 
		} 

		new_socket = accept(listen_fd, (struct sockaddr *) &address, (socklen_t*)&addrlen) ;
		if (new_socket < 0) {
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 

		pthread_t worker_tid ;
		int * worker_arg = (int *) malloc(sizeof(int)) ;
		*worker_arg = new_socket ;

		pthread_create(&worker_tid, 0x0, worker, (void *) worker_arg) ;
	}
} 

