#include <stdio.h>

int main( void ) 
{
	char *argv[4] = {"sequential_min_max", "1", "1000", NULL};

	if ( fork() == 0 ) 
  {
		execv( "/home/runner/oslab2019/lab3/src/sequential_min_max", argv );
  }

  wait(0);

	return 0;
}