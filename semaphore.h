#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#define KEY 55352
#define N 5

// semaphore mutex = 1;
// semaphore sp_mutex[4] = {1, 1, 1, 1};
unsigned int sp_waiting[4] = {0, 0, 0, 0};

int semaphore_P(int semid, int semnum);
int semaphore_V(int semid, int semnum);

void subprocess_do(int sp_idx);
int can_subprocess_do(int sp_idx);

#endif