#include "s_queue.h"
QUEUE *queue_init(void)
{
    QUEUE *q;
    q = malloc(sizeof(QUEUE));

    q->size = 0;
    q->last_deq = -1;
    q->last_enq = -1;

    q->front = NULL;
    q->last = NULL;

    return q;
}

void enq(QUEUE *q, const int d)
{


    if(q->size++ == 0){
        q->front = malloc(sizeof(_NODE));
        q->front->dat = d;
        q->last = q->front;
        q->last_enq = q->front->dat;

    }else{

        q->last_enq = q->last->dat;
        q->last->next = malloc(sizeof(_NODE));
        q->last = q->last->next;
        q->last->dat = d;

    }

}

int deq(QUEUE *q)
{
    if(q->size != 0){
        _NODE *tmp = q->front;


        q->front = q->front->next;
        q->last_deq = tmp->dat;
        free(tmp);

        q->size--;

        return q->last_deq;
    }
    return -1;

}

int  last_deq(const QUEUE *q)
{
    return q->last_deq;
}

int  last_enq(const QUEUE *q)
{
    return q->last_enq;
}

size_t q_size(const QUEUE *q)
{
    return q->size;
}
