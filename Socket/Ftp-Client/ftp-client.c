#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

int 
main (int argc, char *argv[]) 
{ 
	struct sockaddr_in serv_addr; 
	int sock_fd ;
	int s, len ;
	char buffer[1024] = {0}; 
	char * data ;

	char * cmdline ; // 'list' or 'get'
	char * filename ;
	char * ipaddr ;
	char * portnum ;

	//argv[1]: IP:port
	//argv[2]: command = {list, get}
	//argv[3]: filename

	ipaddr = strtok(argv[1], ":") ;
	portnum = strtok(0x0, ":") ;

	if (argc == 3 && strcmp(argv[2], "list") == 0) {
		cmdline = strdup("#list") ;
	}
	else if (argc == 4 && strcmp(argv[2], "get") == 0) {
		cmdline = strdup(argv[3]) ;
	}
	else {
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
	serv_addr.sin_port = htons(atoi(portnum)); 
	if (inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0) {
		perror("inet_pton failed : ") ; 
		exit(EXIT_FAILURE) ;
	} 

	printf("connecting %s:%s\n", ipaddr, portnum) ;

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
		char buf[1024] ;
		size_t n_recv, n_write ;

		FILE * dst = fopen(cmdline, "w") ;

		if (dst == 0x0) {
			perror("fail to open the file.\n") ;
			exit(EXIT_FAILURE) ;
		}

		while ( (n_recv = recv(sock_fd, buf, 1024, 0)) > 0 ) {
			n_write = fwrite(buf, sizeof(char), n_recv, dst) ;
			if (n_recv != n_write) {
				perror("fail to write on the file. \n") ;
				exit(EXIT_FAILURE) ;
			}
		}

		fclose(dst) ;
	}
} 

