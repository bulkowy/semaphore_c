#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#define NULLPTR 0
#define INT_MIN 0x80000000
#define INT_MAX 0x7FFFFFFF

unsigned int indepRand();

struct queue_elem{
    int value;
    struct queue_elem* next_elem;
    struct queue_elem* prev_elem;
};

struct fifo_queue{
    struct queue_elem* head;
    struct queue_elem* tail;
};
struct fifo_queue* init_queue();
void free_queue(struct fifo_queue* q);
int peak_head(struct fifo_queue *q);
int get(struct fifo_queue *q);
void put(struct fifo_queue *q, int value);

int count_occurences(struct fifo_queue *q, int mod);
int count_size(struct fifo_queue *q);
int can_produce_even(struct fifo_queue *q);
int can_produce_odd(struct fifo_queue *q);
int can_eat_even(struct fifo_queue *q);
int can_eat_odd(struct fifo_queue *q);

#endif