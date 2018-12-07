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



main(){
	int my_counter = START_A1;
    while(1){
		printf("A1 Czekam przed CS");
        opusc(semid, CS);
		if(!can_produce_even(buf)){
			printf("PROCES A1 zawiesza sie\n");
			++counter[PROD_E];
			podnies(semid, CS);
			opusc(semid, PROD_E);
			--counter[PROD_E];
        }
		//getAll(buf);
		printf("PROCES A1 dodaje: %d\n", my_counter);
		push(buf, my_counter);
		my_counter += 2;
		my_counter = my_counter % 100;

		if(can_produce_odd(buf) && counter[PROD_O] > 0) {podnies(semid, PROD_O); printf("PROCES A1 -> 2\n");}
		else if(can_eat_even(buf) && counter[CONS_E] > 0) {podnies(semid, CONS_E); printf("PROCES A1 -> 3\n");}
		else if(can_eat_odd(buf) && counter[CONS_O] > 0) {podnies(semid, CONS_O); printf("PROCES A1 -> 4\n");}
		else {podnies(semid, CS); printf("PROCES A1 -> 5\n");}
		//podnies(semid, CS);
	}
}