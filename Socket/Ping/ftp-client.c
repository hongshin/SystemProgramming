#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

int 
main (int argc, char const *argv[]) 
{ 
	struct sockaddr_in serv_addr; 
	int sock_fd ;
	int s, len ;
	char buffer[1024] = {0}; 
	char * data ;

	char * cmdline ; // 'list' or 'get'
	char * filename ;

	if (argc == 4 && strcmp(argv[3], "list") == 0) {
		cmdline = strdup("#list") ;
	}
	else if (argc == 5 && strcmp(argv[3], "get") == 0) {
		cmdline = strdup(argv[4]) ;
	}
	else {
		//argv[1]: IP
		//argv[2]: port
		//argv[3]: command = {list, get}
		//argv[4]: filename
		fprintf(stderr, "Wrong number of arguments\n") ;
		exit(EXIT_FAILURE) ;
	}
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0) ;
	if (sock_fd <= 0) {
		perror("socket failed : ") ;
		exit(EXIT_FAILURE) ;
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(atoi(argv[2])); 
	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		perror("inet_pton failed : ") ; 
		exit(EXIT_FAILURE) ;
	} 

	if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failed : ") ;
		exit(EXIT_FAILURE) ;
	}
	
	data = cmdline ;
	len = strlen(cmdline) ;
	s = 0 ;
	while (len > 0 && (s = send(sock_fd, data, len, 0)) > 0) {
		data += s ;
		len -= s ;
	}
	shutdown(sock_fd, SHUT_WR) ;

	if (strcmp(cmdline, "#list") == 0) {
		char buf[1024] ;
		//data = 0x0 ;
		//len = 0 ;
		while ( (s = recv(sock_fd, buf, 1023, 0)) > 0 ) {
			buf[s] = 0x0 ;
			printf("%s", buf) ;
			/*
			if (data == 0x0) {
				data = strdup(buf) ;
				len = s ;
			}
			else {
				data = realloc(data, len + s + 1) ;
				strncpy(data + len, buf, s) ;
				data[len + s] = 0x0 ;
				len += s ;
			}*/

		}
		printf("\n") ;
	}
	else /* get file */ {
		// fopen(cmdline, "w") ;
		// while (recv(...)) { fwrite(...) }
		// fclose(...) ;
	}
} 

