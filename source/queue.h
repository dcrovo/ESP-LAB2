#include "Utilities.h"
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define QUEUE_EMPTY NULL


typedef struct node{ //Estructura de unitaria de la cola
	uint8_t value; // Valor almacenado en la posición de la cola
	struct node *next; //Dirección del siguiente valor de la cola
}node;

typedef struct{
	node *head; // primer valor de la cola para extraer
	node *tail; // ultimo valor recibido por la cola
	int q_tam; // tamaño actual de la cola
	int32_t size;
}queue;


//Inicializacion de la cola
void queue_init(queue *q, int32_t Buff_size);

/*Funcion para agregar un dato a la cola*/
char queue_in(queue *q, uint8_t value);

uint8_t queue_out(queue *q);

char queue_full(queue *q);

char queue_empty(queue *q);
