#include "processes.h"
#include <unistd.h>


void create_sp(void (*sp_func)())
{
	int ForkResult = fork();
	if(ForkResult == 0)
	{
		sp_func();
		exit(0);
	}
}

void A1();
void A2();
void B1();
void B2();