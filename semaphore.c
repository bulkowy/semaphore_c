#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaphore.h"

static struct sembuf buf;

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