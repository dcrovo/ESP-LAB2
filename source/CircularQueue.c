
#include "CircularQueue.h"

void initQueue(Queue *queue, QNumT capacity)
{
	queue->data =  (char*) malloc(capacity*sizeof(char));
	queue->front = queue-> size = 0;
	queue->rear = capacity - 1;
	queue->capacity = capacity;
	
};
char queueIsEmpty(Queue *queue)
{
	return(queue->size == 0);
};
char queueIsFull(Queue *queue)
{
	return(queue->size == queue->capacity);
};
char dequeue(Queue *queue)
{
	if(queueIsEmpty(queue))
		return FALSE;
	char item = *(queue->data+queue->front);
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size = queue->size - 1;
	return item;
};
void enqueue(Queue *queue, char element)
{
	if(queueIsFull(queue))
		return;
	queue->rear = (queue->rear + 1) % queue->capacity;
	volatile char *dataSlot = queue->data+queue->rear;
	*dataSlot = element;

	queue->size = queue->size + 1;

};
char queueFull_75(Queue *queue) //No esta definida en queue.h
{
	return((float)(queue->size)/(float)(queue->capacity) >= 0.75);
};
char queue_25(Queue *queue) 
{
	return((float)(queue->size)/(float)(queue->capacity) == 0.25);
};
void deleteQueue(Queue *queue)
{
	free(queue->data);
};
