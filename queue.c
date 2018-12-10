#include "queue.h"
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

int* init_queue(){
    /*
     * Initialization of Circular Buffer (Queue)
     * 
     * Assigns shared memory and fills AVAILABLE cells with `FREE` value
     * And sets 2 last elems with zeros as pointer to HEAD/TAIL
     */

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
    /*
     * @returns
     * - `q[ q[HEAD] ] - value at head of Circular buffer
     */

    if(q[HEAD] == q[TAIL]) return INT_MIN;
    return q[ q[HEAD] ];
}

int get(int *q){
    /*
     * Marks first element in queue as FREE,
     * Shifts HEAD pointer one place further
     * And returns value from freed cell
     */

    int tmp = q[ q[HEAD] ];

    if(q[HEAD] == q[TAIL]) return INT_MIN;

    q[ q[HEAD] ] = FREE;
    q[HEAD] = (q[HEAD] + 1) % MAXSIZE;
    return tmp;
}

void put(int *q, int value){
    /*
     * Puts given `value` at TAIL of circular buffer
     * And shifts TAIL pointer one place further
     */
    q[ q[TAIL] ] = value;
    q[TAIL] = (q[TAIL] + 1) % MAXSIZE;
    return;
}

void print_queue(int *q){
    /*
     * Prints contents of given circular buffer
     */
    printf("Table contents: [ ");

    for(int i = q[HEAD]; i != q[TAIL]; i = (i+1)%30)
        printf("%d ", q[i]);

    printf("]\n");
}

int count_occurences(int *q, int mod){
    /*
     * Returns amount of EVEN/ODD elems in circular buffer
     * 
     * @params
     * `mod` - if == 0 - counts EVEN
     *         if == 1 - counts ODD
     */
    int count = 0;
    for(int i = q[HEAD]; i != q[TAIL]; i = (i+1)%30){
        if( q[i] % 2 == mod )
            count++;
    }

    return count;
}

int count_size(int *q){
    /*
     * Return amount of elems in circular buffer
     */
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