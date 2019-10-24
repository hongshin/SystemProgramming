#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>

pthread_mutex_t 	m = PTHREAD_MUTEX_INITIALIZER ;
pthread_cond_t  	c1 = PTHREAD_COND_INITIALIZER ;
pthread_cond_t  	c2 = PTHREAD_COND_INITIALIZER ;

char * message = 0x0 ;

void *
print_message_function1 (void *ptr)
{
	char * last = 0x0 ;
	while(1) {
		pthread_mutex_lock(&m) ;
		if (message != 0x0 && (last == 0x0 || strcmp(message, last) != 0)) {
			for (int i = 0 ; i < 3 ; i++)
				printf("%s\n", message) ;
			
			if (last != 0x0)
				free(last) ;
			last = message ;
		}
		pthread_mutex_unlock(&m) ;
	}
}

void *
print_message_function2 (void *ptr)
{
	while (1) {
		char buf[1024] ;
		scanf("%s", buf) ;
		pthread_mutex_lock(&m) ;
		message = strdup(buf) ;
		pthread_mutex_unlock(&m) ;
	}
}


int
main()
{
	pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, print_message_function1, 0x0);
	pthread_create( &thread2, NULL, print_message_function2, 0x0);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL); 

	exit(0);
}


