#include <stdlib.h>
#include "cola.h"

typedef struct Ticket Ticket;
 
//Crear un nodo y asignarle un elemento
Nodo *createNode(Ticket *elemento) {
    Nodo *nodo = (Nodo *)malloc(sizeof(Nodo));
    if (nodo == NULL)
        exit(EXIT_FAILURE);
    nodo -> data = elemento;
    nodo -> next = NULL;
    return nodo;
}

//Crear una cola vacia
Cola *createQueue() {
    Cola *queue = (Cola *)malloc(sizeof(Cola));
    if (queue == NULL)
        exit(EXIT_FAILURE);
    queue -> head = NULL;
    queue -> tail = NULL;
    return queue;
};

//Insertar un elemento en la cola (al final)
void enqueue(Cola *queue, void *elemento) {
    Nodo *nuevoNodo = createNode(elemento);
    if (queue -> tail == NULL) {
        queue -> head = nuevoNodo;
        queue -> tail = nuevoNodo;
    }
    else {
        queue -> tail -> next = nuevoNodo;
        queue -> tail = nuevoNodo;
    }
}

//Eliminar el primer elemento de la cola
void dequeue(Cola *queue) {
    if (queue -> tail == NULL)
        return;
    Nodo *aux = queue -> head;
    queue -> head = aux -> next;
    if (queue -> head == NULL)
        queue -> tail = NULL;
    free(aux->data);
    free(aux);
}

//Retornar el primer elemento de la cola
void *top(Cola *queue) {
    if (queue -> head == NULL)
        return NULL;
    return (queue -> head -> data);
}


