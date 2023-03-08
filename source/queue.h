#define QUEUE_EMPTY NULL


typedef struct node{ //Estructura de unitaria de la cola
	char value; // Valor almacenado en la posición de la cola
	struct node *next; //Dirección del siguiente valor de la cola
}node;

typedef struct{
	node *head; // primer valor de la cola para extraer
	node *tail; // ultimo valor recibido por la cola
	int q_tam; // tamaño actual de la cola
}queue;

//Inicializacion de la cola
void queue_init(queue *q){

	q->head = NULL; 
	q->tail = NULL;
	q->q_tam=0;
}

/*Funcion para agregar un dato a la cola*/
char queue_in(queue *q, char value){
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
