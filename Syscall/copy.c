#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
copy (FILE * src, FILE * dst)
{
	char buf[1024] ;
	size_t n_read, n_write ;
	
	while (n_read = fread(buf, 1, 1024, src)) {
		n_write = fwrite(buf, 1, n_read, dst) ;
		if (n_read != n_write) {
			perror("fail to write on a file: ") ;
			exit(EXIT_FAILURE) ;
		}
	}
}

int 
main (int argc, char ** argv)
{
	FILE * src, * dst ;

	if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments\n") ;
		exit(EXIT_FAILURE) ;
	}

	src = fopen(argv[1], "r") ;
	if (src == 0x0) {
		perror("fail to open a file: ") ;
		exit(EXIT_FAILURE) ;
	}

	dst = fopen(argv[2], "w") ;
	if (dst == 0x0) {
		perror("fail to open a file: ") ;
		exit(EXIT_FAILURE) ;
	}

	copy(src, dst) ;

	fclose(src) ;
	fclose(dst) ;

	return 0 ;
}
