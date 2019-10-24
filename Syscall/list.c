#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

void list_items(char * p)
{
	DIR * d ;
	struct dirent * e ;

	d = opendir(p) ;
	if (d == 0x0)
		exit(EXIT_FAILURE) ;

	for (e = readdir(d) ; e != 0x0 ; e = readdir(d)) {
		if (e->d_type == DT_DIR /* directory */ ||
			e->d_type == DT_REG /* regular file */ ) {
			printf("%s\n", e->d_name) ;
		}
	}
}

int main(int args, char ** argv)
{
	if (args != 2)
		exit(EXIT_FAILURE) ;

	list_items(argv[1])  ;
}
