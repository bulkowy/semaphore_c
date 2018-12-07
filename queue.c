#include "queue.h"
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

int indepRand()
{
	FILE * F = fopen("/dev/urandom", "r");
	if(!F)
	{
		printf("Cannot open urandom...\n");
		abort();
	}
	unsigned int Ret;
	unsigned int X = fread((char *) &Ret, 1, sizeof(unsigned int), F);
	fclose(F);
	
	return Ret;
}

int* init_queue(){
    int shmid = shmget(KEY+1, (MAXSIZE+2)*sizeof(int), IPC_CREAT|0600);
    if (shmid == -1){
		perror("Utworzenie segmentu pamieci wspoldzielonej");
		exit(1);
    }

	int *buf = (int*)shmat(shmid, NULL, 0);
    if (buf == NULL){
		perror("Przylaczenie segmentu pamieci wspoldzielonej");
		exit(1);
    }

    for(int i = 0; i < MAXSIZE; i++) buf[i] = FREE;
    buf[HEAD] = 0;
    buf[TAIL] = 0;
    return buf;
}


void free_queue(int* q){
    free(q);
}

int peak_head(int *q){
    if(q[HEAD] == q[TAIL]) return INT_MIN;
    return q[ q[HEAD] ];
}

int get(int *q){
    int tmp = q[ q[HEAD] ];

    if(q[HEAD] == q[TAIL]) return INT_MIN;

    q[ q[HEAD] ] = FREE;
    q[HEAD] = (q[HEAD] + 1) % MAXSIZE;
    return tmp;
}

void put(int *q, int value){

    q[ q[TAIL] ] = value;
    q[TAIL] = (q[TAIL] + 1) % MAXSIZE;
    return;
}

int count_occurences(int *q, int mod){
    int count = 0;
    for(int i = q[HEAD]; i != q[TAIL]; i = (i+1)%30){
        if( q[i] % 2 == mod )
            count++;
    }

    return count;
}

int count_size(int *q){
    int count = 0;
    for(int i = q[HEAD]; i != q[TAIL]; i = (i+1)%30){
        count++;
    }
    return count;
}

int can_produce_even(int *q){
    return (count_occurences(q, 0) < 10);
}

int can_produce_odd(int *q){
    return (count_occurences(q, 1) < count_occurences(q, 0));
}

int can_eat_even(int *q){
	return (count_size(q) > 3 && peak_head(q)%2 == 0);
}

int can_eat_odd(int *q){
	return (count_size(q) > 7 && peak_head(q)%2 == 1);
}