#include "processes.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include "queue.h"
#include "semaphore.h"


void create_sp(void (*sp_func)(struct fifo_queue* que), struct fifo_queue *q){
	int ForkResult = fork();
	if(ForkResult == 0)
	{
		sp_func(q);
		exit(0);
	}
}

int *bind_data(int *semid, int *shmid){

    int *proc_waiting;

    *semid = semget(KEY, N, 0600);
    if (*semid == -1){
        perror("semget error");
        return NULL;
    }

    *shmid = shmget(KEY, 4*sizeof(int), 0600);
    if (*shmid == -1){
        perror("shmget error");
        return NULL;
    }

    proc_waiting = (int*)shmat(*shmid, NULL, 0);
    if (proc_waiting == NULL){
        perror("shmat error");
        return NULL;
    }

    return proc_waiting;
}

void A1(struct fifo_queue *queue){
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    if (proc_waiting == NULL) return;


}
void A2(struct fifo_queue *queue){
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    if (proc_waiting == NULL) return;
}

void B1(struct fifo_queue *queue){
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    if (proc_waiting == NULL) return;
}

void B2(struct fifo_queue *queue){
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    if (proc_waiting == NULL) return;
}