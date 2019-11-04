#include <stdio.h>

int main (int argc, char ** argv) 
{
	printf("argc: %d\n", argc) ;
	while (*argv != 0x0) {
		printf("%s\n", *argv) ;
		argv += 1 ;
	}
}
