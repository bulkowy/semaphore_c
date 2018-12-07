#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#define NULLPTR 0
#define FREE -5
#define INT_MIN 0x80000000
#define INT_MAX 0x7FFFFFFF
#define MAXSIZE 30
#define HEAD 30
#define TAIL 31

int indepRand();

int* init_queue();
void free_queue(int* q);
int peak_head(int *q);
int get(int *q);
void put(int *q, int value);

int count_occurences(int *q, int mod);
int count_size(int *q);
int can_produce_even(int *q);
int can_produce_odd(int *q);
int can_eat_even(int *q);
int can_eat_odd(int *q);

#endif