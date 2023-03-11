
#include "queue.h"



//Inicializacion de la cola
void queue_init(queue *q, int32_t Buff_size){

	q->head = NULL;
	q->tail = NULL;
	q->q_tam=0;
	q->size = Buff_size;
};

/*Funcion para agregar un dato a la cola*/
char queue_in(queue *q, uint8_t value){
	//Se crea un nuevo nodo
	node *newnode = malloc(sizeof(node));
	if(newnode == NULL) return FALSE;

	newnode->value = value; // Se le asigna el valor recibido al nuevo nodo
    newnode->next = NULL;
    //Si ya hay un valor se asigna el nuevo nodo como siguiente
	if(q->tail != NULL){
		q->tail->next=newnode;
	}
	q->tail = newnode;

	// Se iguala el valor de la cabeza al nuevo nodo si está vacia
	if(q->head == NULL){
		q->head = newnode;
	}
	q->q_tam++; //Se incrementa en 1 el tamaño de la cola
	return TRUE;
}

char queue_out(queue *q){

	/*Si la cabeza está vacia no hay datos en la cola*/
	if(q->head == NULL) return QUEUE_EMPTY;

	/* Varible temportal para guardar la cabeza*/
	node *tmp = q->head;

	//Se guarda el valor que se va a retornar
	char result = tmp->value;

	//Se elimina el valor que se saco y se asigna la cabeza al siguiente valor
	q->head = q->head->next;
	// Si no hay datos en la cola se iguala a NULL
	if(q->head == NULL){
		q->tail = NULL;
	}

	free(tmp);
	q->q_tam--;//Se disminuye en 1 el tamaño de la cola
	return result;

}

char queue_full(queue *q){
	if(q->q_tam >= ((q->size) * 0.75)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

char queue_empty(queue *q){
	if(q->q_tam == ((q->size) * 0.25)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

