#ifndef _PROCESSES_H
#define _PROCESSES_H

void create_sp(void (*sp_func)());
void A1();
void A2();
void B1();
void B2();

int get_svalue(int semid);

int test_wakeup(int *queue, int semid, int *proc_waiting, int sp_idx);

#endif