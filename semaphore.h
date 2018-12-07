#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#define KEY 55352   // "random" KEY for shared memory alocation
#define N 5         // number of semaphores

#define MUTEX 0     // macro for MUTEX Semaphore location
#define SP_A1 1     // macro for SubProcess Group A1 Semaphore loc.
#define SP_A2 2     // macro for SubProcess Group A2 Semaphore loc.
#define SP_B1 3     // macro for SubProcess Group B1 Semaphore loc.
#define SP_B2 4     // macro for SubProcess Group B2 Semaphore loc.

int semaphore_P(int semid, int semnum);
int semaphore_V(int semid, int semnum);

#endif