#ifndef _PROCESSES_H
#define _PROCESSES_H

void create_sp(void (*sp_func)(struct fifo_queue *que), struct fifo_queue *q);
void A1(struct fifo_queue *queue);
void A2(struct fifo_queue *queue);
void B1(struct fifo_queue *queue);
void B2(struct fifo_queue *queue);

int get_svalue(int semid);

int test_wakeup(struct fifo_queue *queue, int semid, int *proc_waiting, int sp_idx);

#endif