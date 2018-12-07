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


void create_sp(void (*sp_func)()){
	int ForkResult = fork();

	if(ForkResult == 0)
	{
		sp_func();
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

int *bind_queue(){
    int shmid = shmget(KEY+1, (MAXSIZE+2)*sizeof(int), 0600);
    if (shmid == -1){
		perror("Utworzenie segmentu pamieci wspoldzielonej");
		exit(1);
    }

	int *buf = (int*)shmat(shmid, NULL, 0);
    if (buf == NULL){
		perror("Przylaczenie segmentu pamieci wspoldzielonej");
		exit(1);
    }

    return buf;
}

int get_svalue(int semid, int sp_idx){
    return semctl(semid, sp_idx, GETVAL, (int)0);
}

int test_wakeup(int *queue, int semid, int *proc_waiting, int sp_idx){
    if(!(sp_idx == SP_A1) && can_produce_even(queue) && (proc_waiting[SP_A1] > 0)){
        semaphore_V(semid, SP_A1);
        //printf("(IDX:%d) - I woke up SP_A1\n", sp_idx);
    }
    else if(!(sp_idx == SP_A2) && can_produce_odd(queue) && (proc_waiting[SP_A2] > 0)){
        semaphore_V(semid, SP_A2);
        //printf("(IDX:%d) - I woke up SP_A2\n", sp_idx);
    }
    else if(!(sp_idx == SP_B1) && can_eat_even(queue) && (proc_waiting[SP_B1] > 0)){
        semaphore_V(semid, SP_B1);
        //printf("(IDX:%d) - I woke up SP_B1\n", sp_idx);
    }
    else if(!(sp_idx == SP_B2) && can_eat_odd(queue) && (proc_waiting[SP_B2] > 0)){
        semaphore_V(semid, SP_B2);
        //printf("(IDX:%d) - I woke up SP_B2\n", sp_idx);
    }
    else semaphore_V(semid, MUTEX);
}

void A1(){
    int *queue = bind_queue();
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    if (proc_waiting == NULL) return;
    int counter = 0;

    while(SP_A1){
        semaphore_P(semid, MUTEX);
        if( !can_produce_even(queue) ){
            ++proc_waiting[SP_A1];
            semaphore_V(semid, MUTEX);
            printf("(A1) - Went Sleep\n");
            semaphore_P(semid, SP_A1);

            --proc_waiting[SP_A1];
        }
        put(queue, counter);
        printf("(A1) - Produced: %d\n", counter);
        counter = ( counter + 2 ) % 100;

        test_wakeup(queue, semid, proc_waiting, SP_A1);
        usleep(500000);
    }
}

void A2(){
    int *queue = bind_queue();
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    if (proc_waiting == NULL) return;
    int counter = 1;

    while(SP_A2){
        semaphore_P(semid, MUTEX);
        if( !can_produce_odd(queue) ){
            ++proc_waiting[SP_A2];
            semaphore_V(semid, MUTEX);

            printf("(A2) - Went Sleep\n");
            semaphore_P(semid, SP_A2);

            --proc_waiting[SP_A2];
        }
        put(queue, counter);
        printf("(A2) - Produced: %d\n", counter);
        printf("(A2) - HEAD: %d\n", peak_head(queue));
        counter = ( counter + 2 ) % 100;

        test_wakeup(queue, semid, proc_waiting, SP_A2);
        usleep(500000);
    }
}

void B1(){
    int *queue = bind_queue();
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    int val;
    if (proc_waiting == NULL) return;

    while(SP_B1){
        semaphore_P(semid, MUTEX);
        if( !can_eat_even(queue) ){
            ++proc_waiting[SP_B1];
            semaphore_V(semid, MUTEX);
            printf("(B1) - Went Sleep\n");
            semaphore_P(semid, SP_B1);

            --proc_waiting[SP_B1];
        }
        val = get(queue);
        printf("(B1) - Eaten: %d\n", val);

        test_wakeup(queue, semid, proc_waiting, SP_B1);
        usleep(500000);
    }
}

void B2(){
    int *queue = bind_queue();
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    int val;
    if (proc_waiting == NULL) return;

    while(SP_B2){
        semaphore_P(semid, MUTEX);
        if( !can_eat_odd(queue) ){
            ++proc_waiting[SP_B2];
            semaphore_V(semid, MUTEX);
            printf("(B2) - Went Sleep\n");
            semaphore_P(semid, SP_B2);

            --proc_waiting[SP_B2];
        }
        val = get(queue);
        printf("(B2) - Eaten: %d\n", val);

        test_wakeup(queue, semid, proc_waiting, SP_B2);
        usleep(500000);
    }
}