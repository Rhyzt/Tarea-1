#ifndef COLA_H
#define COLA_H

typedef struct Nodo {
    void* data;
    struct Nodo *next;
}Nodo;

typedef struct Cola {
    Nodo *head;
    Nodo *tail;
} Cola;

//Crear una cola vacia
Cola *createQueue();

//Insertar un elemento en la cola (al final)
void enqueue(Cola *queue, void *data);

//Eliminar el primer elemento de la cola
void dequeue(Cola *queue);

//Retornar el primer elemento de la cola
void *top(Cola *queue);

#endif