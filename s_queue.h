#ifndef SQUEUE_H
#define SQUEUE_H

#include <stdlib.h>




typedef struct QUEUE QUEUE;
typedef struct _NODE _NODE;

struct QUEUE{
    size_t size;

    int last_deq;//last element dequeued
    int last_enq;//last element enqueued

    _NODE *front;
    _NODE *last;
};

struct _NODE{
    int dat;
    _NODE *next;
};

QUEUE *queue_init();

int  deq(QUEUE *q);
void enq(QUEUE *q, const int d);

int  last_deq(const QUEUE *q);
int  last_enq(const QUEUE *q);

size_t q_size(const QUEUE *q);




#endif
