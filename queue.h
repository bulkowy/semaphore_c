#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>

#define FREE -5             // value used for flagging buffer values as free
#define INT_MIN 0x80000000 
#define INT_MAX 0x7FFFFFFF
#define MAXSIZE 30          // max size of circular buffer
#define HEAD 30             // location in buffer for pointer to its head
#define TAIL 31             // location in buffer for pointer to its tail

int* init_queue();
void free_queue(int* q);
int peak_head(int *q);
int get(int *q);
void put(int *q, int value);
void print_queue(int *q);

int count_occurences(int *q, int mod);
int count_size(int *q);
int can_produce_even(int *q);
int can_produce_odd(int *q);
int can_eat_even(int *q);
int can_eat_odd(int *q);

#endif