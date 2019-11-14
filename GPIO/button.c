#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wiringPi.h>

char 	flag = 0 ;

void
interrupt_handler () 
{
	flag++ ;
	printf("flag: %d\n", flag) ;
}

int
main () 
{
	if (wiringPiSetup() < 0)
		exit(EXIT_FAILURE) ;

	if (wiringPiISR(22, INT_EDGE_FALLING, &interrupt_handler) < 0) {
		fprintf(stderr, "unsable to setup ISR\n") ;
		exit(EXIT_FAILURE) ;
	}

	while (1) {
		while (digitalRead(22) == 1) {
			printf("button pressed %d\n", flag) ;
		}
	}
}

