#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include "queue.h"
#include "semaphore.h"
#include "processes.h"
#include <sys/wait.h>


int main(){
    int semid, shmid;
    shmid = shmget(KEY, 4*sizeof(int), IPC_CREAT|0600);
    if (shmid == -1){
		perror("Utworzenie segmentu pamieci wspoldzielonej");
		exit(1);
    }

	int *counter = (int*)shmat(shmid, NULL, 0);
    if (counter == NULL){
		perror("Przylaczenie segmentu pamieci wspoldzielonej");
		exit(1);
    }
    for(int i = 0; i < 4; i++) counter[i] = 0;
    semid = semget(KEY, N, IPC_CREAT|0600);
    if (semid == -1){
		perror("Utworzenie tablicy semaforow");
    	exit(1);
	}
    for( int idx = 0; idx < N; idx++ ){
        if (semctl(semid, idx, SETVAL, (int)1) == -1){
    	    perror("Nadanie wartosci semaforowi");
		    exit(1);
	    }
    }

    struct fifo_queue *my_queue = init_queue();

    create_sp(&A1, my_queue);
    create_sp(&A2, my_queue);
    create_sp(&B1, my_queue);
    create_sp(&B2, my_queue);

    /* put(my_queue, 3);
    put(my_queue, 5);
    put(my_queue, 6);
    put(my_queue, 7);
    put(my_queue, 2); */


    free_queue(my_queue);

    while(wait(NULL) > 0) {}
    return 0;
}