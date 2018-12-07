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
    /*
     * Create subprocess using `fork` from `unistd.h`
     * and make it execute passed `sp_func` function
     * 
     * @params
     * `sp_func` --> SubProcess_Function
     */
	
    int ForkResult = fork();

    // If subprocess forked successfully execute `sp_func`
	if(ForkResult == 0){
		sp_func();
		exit(0);
	}
}

int *bind_data(int *semid, int *shmid){
    /*
     * Assign to `semid` and `shmid` shared memory addresses
     * for created semaphores and buffers respectively
     * 
     * @params
     * `semid` - Address for Semaphores
     * `shmid` - Address for Buffer counting sleeping SubProcesses
     * 
     * @returns
     * `proc_waiting` - Pointer to first elem of `shmid` Buffer
     */

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

    int *proc_waiting = (int*)shmat(*shmid, NULL, 0);
    if (proc_waiting == NULL){
        perror("shmat error");
        return NULL;
    }

    return proc_waiting;
}

int *bind_queue(){
    /*
     * After finding shared memory segment for circular buffer
     * return pointer to its first element
     * 
     * @returns
     * `buf` - pointer to first element of circular buffer
     */
    
    int shmid = shmget(KEY+1, (MAXSIZE+2)*sizeof(int), 0600);
    if (shmid == -1){
		perror("shmget error");
		exit(1);
    }

	int *buf = (int*)shmat(shmid, NULL, 0);
    if (buf == NULL){
		perror("shmat error");
		exit(1);
    }

    return buf;
}

int test_wakeup(int *queue, int semid, int *proc_waiting, int sp_idx){
    /*
     * Function testing which checks whether subprocess might wakeup
     * OTHER subprocess that went asleep because of not meeting
     * its requirements for job
     * 
     * For every `if`:
     * CHECK WHETHER:
     * - subprocess, which calls `test_wakeup`, is not trying to wakeup
     *   its subprocess group: `!(sp_idx == SP_{A1,A2,B1,B2})`
     * - currently investigated subprocess group is meeting requirements
     * - if there is any subprocess from group waiting for signal
     * 
     * IF TRUE:
     * - up semaphore related to chosen subprocess group
     * 
     * IF FALSE:
     * - check next subprocess group
     * 
     * IF NO SUBPROCESS GROUP MET REQUIREMENTS:
     * - leave critical section
     */

    if(!(sp_idx == SP_A1) && can_produce_even(queue) 
    && (proc_waiting[SP_A1] > 0))
        semaphore_V(semid, SP_A1);

    else if(!(sp_idx == SP_A2) && can_produce_odd(queue) 
    && (proc_waiting[SP_A2] > 0))
        semaphore_V(semid, SP_A2);

    else if(!(sp_idx == SP_B1) && can_eat_even(queue) 
    && (proc_waiting[SP_B1] > 0))
        semaphore_V(semid, SP_B1);
    
    else if(!(sp_idx == SP_B2) && can_eat_odd(queue) 
    && (proc_waiting[SP_B2] > 0))
        semaphore_V(semid, SP_B2);
    
    else semaphore_V(semid, MUTEX);
}

void A1(){
    /*
     * Subprocess algorithm for group A1
     * 
     * Every 0.5s, if possible, creates next EVEN element
     * and put it in shared circural buffer
     */
    int *queue = bind_queue();
    int semid, shmid;
    int *proc_waiting = bind_data(&semid, &shmid);
    if (proc_waiting == NULL) return;
    int counter = 0;

    while(SP_A1){
        semaphore_P(semid, MUTEX);          // enter critical section
        if( !can_produce_even(queue) ){     // check if requirements are met
            ++proc_waiting[SP_A1];
            semaphore_V(semid, MUTEX);      // leave critical section
            printf("(A1) - Went Sleep\n");  
            semaphore_P(semid, SP_A1);      // block A1 subprocesses

            --proc_waiting[SP_A1];
        }
        put(queue, counter);                // create EVEN element
        printf("(A1) - Produced: %d\n", counter);
        counter = ( counter + 2 ) % 100;    // update value for next element

        test_wakeup(queue, semid, proc_waiting, SP_A1);
        usleep(500000);
    }
}

void A2(){
    /*
     * Subprocess algorithm for group A1
     * 
     * Every 0.5s, if possible, creates next EVEN element
     * and put it in shared circural buffer
     */
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
    /*
     * Subprocess algorithm for group A1
     * 
     * Every 0.5s, if possible, creates next EVEN element
     * and put it in shared circural buffer
     */
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
    /*
     * Subprocess algorithm for group A1
     * 
     * Every 0.5s, if possible, creates next EVEN element
     * and put it in shared circural buffer
     */
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