#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "MKL46Z4.h"
#include "Utilities.h"
#include <stdlib.h>
/*
*/
typedef unsigned int QNumT;
typedef struct Queue Queue;
struct Queue{
	volatile char *data;	/*Pointer to data*/
	QNumT capacity;			/*Capacity of the buffer in char*/
	QNumT front;			/*Input index */
	QNumT rear;				/*Output index*/
	QNumT size;
};


void initQueue(Queue *queue, QNumT len);
char dequeue(Queue *queue);
void enqueue(Queue *queue, char element);
char isEmpty(Queue *queue);
char isFull(Queue *queue);
char isAlmostFull(Queue *queue);
void deleteQueue(Queue *queue);

#endif
