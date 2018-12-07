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

	

    int *buf = init_queue();

    create_sp(&A1);
    create_sp(&A2);
    create_sp(&B1);
    create_sp(&B2);

    while(wait(NULL) > 0) {}


    /* int *buf = init_queue();
    printf("size = %d\n", count_size(buf));
    
    for(int i = 0; i < 20; i++){
        put(buf, i);
    }
    printf("size = %d\n", count_size(buf));
    printf("head = %d\n", peak_head(buf));
    for(int i = 0; i < 10; i++){
        printf("get (%d): %d\n", i, get(buf));
    }
    printf("head = %d\n", peak_head(buf)); */

    return 0;
}