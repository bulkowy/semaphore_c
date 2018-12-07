#include "processes.h"
#include <unistd.h>
#include "queue.h"


void create_sp(void (*sp_func)(struct fifo_queue* que), struct fifo_queue *q){
	int ForkResult = fork();
	if(ForkResult == 0)
	{
		sp_func(q);
		exit(0);
	}
}

void A1(struct fifo_queue *queue){
    int semid;
}
void A2(struct fifo_queue *queue){return;}
void B1(struct fifo_queue *queue){return;}
void B2(struct fifo_queue *queue){return;}