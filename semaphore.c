#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaphore.h"

static struct sembuf buf;
unsigned int sp_waiting[4] = {0, 0, 0, 0};

int semaphore_P(int semid, int semnum){ 
    buf.sem_num = semnum;
    buf.sem_op = -1;
    semop(semid, &buf, 1);
}

int semaphore_V(int semid, int semnum){
		buf.sem_num = semnum; 
		buf.sem_op = 1; 
		semop(semid, &buf, 1);
}

void subprocess_do(int semnum);
int can_subprocess_do(int semnum);