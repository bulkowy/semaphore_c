#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#define KEY 55352
#define N 5

#define MUTEX 0
#define SP_A1 1
#define SP_A2 2
#define SP_B1 3
#define SP_B2 4

int semaphore_P(int semid, int semnum);
int semaphore_V(int semid, int semnum);

#endif