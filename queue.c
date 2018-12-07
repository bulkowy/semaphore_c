#include "queue.h"
#include <stdlib.h>

unsigned int indepRand()
{
	FILE * F = fopen("/dev/urandom", "r");
	if(!F)
	{
		printf("Cannot open urandom...\n");
		abort();
	}
	unsigned int Ret;
	unsigned int X = fread((char *) &Ret, 1, sizeof(unsigned int), F);
	fclose(F);
	
	return Ret;
}

struct fifo_queue* init_queue(){
    struct fifo_queue* new_queue = (struct fifo_queue*)malloc(sizeof(struct fifo_queue));
    new_queue->head = NULLPTR;
    new_queue->tail = NULLPTR;
    return new_queue;
}


void free_queue(struct fifo_queue* q){
    struct queue_elem* elem = q->head;
    while(get(q) >= 0);
    free(q);
}

int peak_head(struct fifo_queue *q){
    if(q->head == NULLPTR) return INT_MIN;
    return q->head->value;
}

int get(struct fifo_queue *q){
    struct queue_elem* tmp = q->head;

    if(q->head == NULLPTR) return INT_MIN;

    if(q->head->next_elem == NULLPTR){
        q->head = NULLPTR;
        q->tail = NULLPTR;
        return tmp->value;
    }

    q->head = q->head->next_elem;
    q->head->prev_elem = NULLPTR;
    int val = tmp->value;
    free(tmp);
    return val;
}

void put(struct fifo_queue *q, int value){

    struct queue_elem *elem = (struct queue_elem*)malloc(sizeof(struct queue_elem));

    elem->next_elem = NULLPTR;
    elem->prev_elem = NULLPTR;
    elem->value = value;

    if(q->head == NULLPTR){
        q->head = elem;
        q->tail = elem;
        return;
    }

    q->tail->next_elem = elem;
    elem->prev_elem = q->tail;
    q->tail = elem;
    return;
}

int count_occurences(struct fifo_queue *q, int mod){
    struct queue_elem *tmp_elem = q->head;

    int count = 0;

    while(tmp_elem != NULLPTR){
        if( tmp_elem->value % 2 == mod )
            count++;
        tmp_elem = tmp_elem->next_elem;
    }

    return count;
}

int count_size(struct fifo_queue *q){
    struct queue_elem *tmp_elem = q->head;
    int count = 0;
    while(tmp_elem != NULLPTR){
        count++;
        tmp_elem = tmp_elem->next_elem;
    }
    return count;
}

int can_produce_even(struct fifo_queue *q){
    return (count_occurences(q, 0) < 10);
}

int can_produce_odd(struct fifo_queue *q){
    return (count_occurences(q, 1) < count_occurences(q, 0));
}

int can_eat_even(struct fifo_queue *q){
	return (count_size(q) > 3 && peak_head(q)%2 == 0);
}

int can_eat_odd(struct fifo_queue *q){
	return (count_size(q) > 7 && peak_head(q)%2 == 1);
}