#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "cola.h"

//Para definir de mejor manera cada cola en una array de colas
#define PRIO_ALTA 0
#define PRIO_MEDIA 1
#define PRIO_BAJA 2

typedef struct Ticket { //Struct para guardar los datos de los tickets 
    int ID;
    char horaReg[9];
    char descripcion[50];
    int prioridad;
} Ticket;

typedef struct Busqueda { //Struct Para busqueda de tickets
    int prioridad;
    bool esNulo;
    Ticket *ticketEncontrado;
} Busqueda;

//Funciones a implementar para el correcto funcionamiento del programa
void mostrarMenu();
Cola *inicializarColas();
void menuOpciones(Cola *arrayColas);
char *obtenerHoraActual();
void registrarTicket(int ID, char *descripcion, Cola *arrayColas);
bool asignarPrioridad(int ID, char *prio, Cola *arrayColas);
void mostrarCola(Cola *arrayColas);
void procesarTicket(Cola *arrayColas);
Busqueda *buscarID(int ID, Cola *arrayColas);


int main() {
    Cola *arrayColas = inicializarColas(); //Crea las diferentes colas de prioridad
    while(1) {
        mostrarMenu(); //Muestra las diferentes opciones al usuario
        getchar();
        menuOpciones(arrayColas); //Verifica la opcion ingresada
    }
}

void mostrarMenu() { //Muestra todas las opciones a elegir al usuario
    puts("--------------------------------------------------");
    puts("                Gestion de Tickets                ");
    puts("--------------------------------------------------");
    puts("1) Registrar Ticket");
    puts("2) Asignar Prioridad a Ticket");
    puts("3) Mostrar Lista de Tickets Pendientes");
    puts("4) Procesar siguiente Ticket");
    puts("5) Buscar Por ID");
    puts("6) Salir");
    puts("--------------------------------------------------");
    puts("Seleccione una opción: ");
}

void menuOpciones(Cola *arrayColas) { //Tomara la respuesta del usuario y ejecutara la instruccion pertinente
    int opcion;
    scanf("%d",&opcion); //Se asume que el usuario no comete errores
    getchar();
    switch (opcion) { //Se usa switch para separar segun el valor ingresado
        case 1: { //Registrar Ticket
            int IDTemp;
            char descTemp[30];
            printf("Ingrese la ID del ticket: "); //Obtener informacion del ticket
            scanf("%d", &IDTemp);
            getchar(); //Limpiar \n
            puts("");
            printf("Describa su problema: ");
            fgets(descTemp, sizeof(descTemp), stdin);
            registrarTicket(IDTemp, descTemp, arrayColas); //Registra los datos ingresados en un ticket con prioridad Baja
            break;
        }
        case 2: { //Asignar Prioridad
            int IDTemp;
            char prio[10];
            printf("Ingrese la ID del ticket: "); //Obtener informacion del ticket
            scanf("%d", &IDTemp);
            getchar();
            puts("");
            printf("Ingrese la prioridad deseada: "); //Obtener informacion del ticket
            fgets(prio, sizeof(prio), stdin);
            prio[strcspn(prio, "\n")] = '\0';
            for (int i = 0 ; prio[i] != '\0' ; i++)
                prio[i] = toupper(prio[i]);

            asignarPrioridad(IDTemp, prio, arrayColas);
            break;
        }
        case 3: { //Mostrar lista
            mostrarCola(arrayColas);
            break;
        }
        case 4: { //Procesar ticket
            procesarTicket(arrayColas);
            break;
        }
        case 5: { //Buscar ticket
            int IDTemp;
            printf("Ingrese la ID del ticket: "); //Obtener informacion del ticket
            scanf("%d", &IDTemp);
            getchar();
            buscarID(IDTemp, arrayColas);
            break;
        }
        case 6: //Fin del programa
            free(arrayColas);
            exit(EXIT_SUCCESS);
        default: { //En caso de que el numero ingresado no sea valido
            puts("Ingresa una opcion Valida"); 
            break;
        }
    }
}

Cola *inicializarColas() {
    Cola *arrayColas = (Cola *)malloc(3 * sizeof(Cola)); //Se crea un arreglo dinamico para almacenar las 3 colas
    if (arrayColas == NULL) {
        puts("No se pudo asignar memoria correctamente");
        exit(EXIT_FAILURE);
    }
    for (int i = 0 ; i < 3 ; i++) { //Se inicializan las 3 colas de prioridad
        arrayColas[i].head = NULL;
        arrayColas[i].tail = NULL;
    }
    return arrayColas;
}

char *obtenerHoraActual() { //Retorna la Hora local, en formato HH:MM:SS
    time_t hora;
    struct tm *horaPtr; //Variables usadas para obtener la hora
    char *horaStr = malloc(9); //Puntero que se retornara y almacenara la hora
    if (horaStr == NULL) { //Se revisa si se pudo asignar la memoria
        puts("No se pudo asignar memoria correctamente");
        exit(EXIT_FAILURE);
    }
    if (time(&hora) == -1) { //Se revisa si se pudo obtener la hora
        puts("Error al obtener la hora de registro");
        exit(EXIT_FAILURE);
    }
    horaPtr = localtime(&hora); //Se obtiene la hora local, en formato completo
    strftime(horaStr, sizeof(horaStr),"%T", horaPtr); //Se formatea la hora con %T (HH:MM:SS)
    
    return(horaStr); 
}

void registrarTicket(int codigo, char *desc, Cola *arrayColas) {
    Ticket *nuevoTicket = (Ticket *)malloc(sizeof(Ticket)); //Asignacion de memoria para el ticket
    if (nuevoTicket == NULL) { //Se revisa si se pudo asignar la memoria
        puts("No se pudo asignar memoria correctamente");
        exit(EXIT_FAILURE);
    }
    nuevoTicket -> ID = codigo; //Se rellena el nuevo ticket
    strcpy(nuevoTicket -> descripcion, desc);
    char *hora = obtenerHoraActual();
    strcpy(nuevoTicket -> horaReg, hora); //Obtiene la hora actual en formato HH:MM:SS
    free(hora);
    nuevoTicket -> prioridad = PRIO_BAJA; //Se asigna la prioridad por defecto (Prioridad Baja)
    enqueue(&arrayColas[PRIO_BAJA], nuevoTicket); //Se encola en la cola por defecto
}

Busqueda *buscarID(int IDBuscada, Cola *arrayColas) {
    int IDtop;
    Busqueda *informacion = malloc(sizeof(Busqueda)); // Se asigna memoria para el struct y se define un valor para guardar la id del primer elemento
    if (informacion == NULL) { //Se revisa si se pudo asignar la memoria
        puts("No se pudo asignar memoria correctamente");
        exit(EXIT_FAILURE);
    }
    informacion -> esNulo = true; //Definira si existe el valor en las colas o no
    for(int i = 0; i < 3 ; i++) { // Se recorren las 3 colas de prioridad
        Ticket *actual = top(&arrayColas[i]); 
        if (actual != NULL) {  // Revisa si la cola tiene elementos
            IDtop = actual -> ID;
            do {
                if (actual -> ID == IDBuscada) { // Si el ticket deseado es encontrado, se cambian los valores del struct informacion
                    informacion -> prioridad = i;
                    informacion -> ticketEncontrado = actual;
                    informacion -> esNulo = false; 
                } // No se retorna nada aqui ya que se tiene que volver al estado original de la cola
                enqueue(&arrayColas[i], actual); // enqueue y dequeue usados para recorrer la cola
                dequeue(&arrayColas[i]);
                actual = top(&arrayColas[i]); //Actualiza el valor del ticket actual
            } while (actual -> ID != IDtop);
        }
        if (informacion -> esNulo == false) //Si se encontro un elemento al terminar de recorrer una cola, evitara que intente revisar otra cola para ahorrar tiempo.
            return informacion;
    }
    return informacion; //Retornara esNulo true, ya que no se pudo encontrar el valor
}

bool asignarPrioridad(int IDBuscada, char *prio, Cola *arrayColas) {
    Busqueda *informacion = buscarID(IDBuscada, arrayColas); // Se obtiene la informacion del ticket a cambiar prioridad
    int nuevaPrioridad; // Guardara la nueva prioridad, para hacer el codigo mas compacto
    if (!(informacion -> esNulo)) { //Se revisa que el ticket exista
        if (strcmp(prio, "BAJO") == 0) //Se revisa que prioridad escogio el usuario y se asigna a una variable numerica
            nuevaPrioridad = PRIO_BAJA;
        else { 
            if (strcmp(prio, "MEDIO") == 0)
                nuevaPrioridad = PRIO_MEDIA;
            else
                nuevaPrioridad = PRIO_ALTA;
        }
        if (informacion -> prioridad == nuevaPrioridad) {
            free(informacion); //Se libera la memoria de la busqueda
            return true; //Si ya pertenece a esta prioridad, la funcion no hace nada
        }
        Ticket *actual = top(&arrayColas[informacion -> prioridad]); //Se asigna un puntero para recorrer la cola correspondiente del dato
        int IDtop = actual -> ID;
        enqueue(&arrayColas[nuevaPrioridad], informacion -> ticketEncontrado); // Se agrega el ticket a la cola correspondiente
        informacion -> ticketEncontrado -> prioridad = nuevaPrioridad; // Se cambia la prioridad del struct del ticket
        if (actual -> ID == IDBuscada) { // Se revisa si el primer dato de la cola es el buscado, ya que solo bastaria con hacer una instruccion
            dequeue(&arrayColas[informacion -> prioridad]);
        }
        else {
            do { // Se recorre la lista original para quitar el ticket de ahi, ya que fue cambiado de prioridad.
                if (actual -> ID != IDBuscada)
                    enqueue(&arrayColas[informacion -> prioridad], actual);
                dequeue(&arrayColas[informacion -> prioridad]);
                actual = top(&arrayColas[informacion -> prioridad]);
            } while (actual -> ID != IDtop);
        }
        free(informacion); //Se libera la memoria de la busqueda
        return true; //Se hizo el cambio con exito
    }
    free(informacion); //Se libera la memoria de la busqueda
    return false; // Si no existe el ticket, no es necesario hacer la comprobacion dentro de colas ya que directamente no se podria encontrar
}

void printearTicket(Ticket *ticket) {
    printf("======= Ticket #%d =======\n", ticket->ID);
    printf("Descripcion: %s\n", ticket -> descripcion);
    printf("Hora: %s\n", ticket->horaReg);
    printf("Prioridad: ");
    if (ticket -> prioridad == 0) puts("Alta");
    else {
        if (ticket -> prioridad == 1) puts("Media");
        else puts("Baja");
    }
}

void mostrarCola(Cola *arrayColas) {
    const char *prioridades[] = {"Alta", "Media", "Baja"};
    for(int i = 0; i < 3 ; i++) { // Se recorren las 3 colas de prioridad
        if (top(&arrayColas[i]) != NULL) {
            printf("---- Prioridad %s ----\n", prioridades[i]);
            Ticket *actual = top(&arrayColas[i]);
            int IDtop = actual -> ID;
            do {
                printearTicket(actual);
                puts("");
                enqueue(&arrayColas[i], actual); // Usado para recorrer la cola
                dequeue(&arrayColas[i]);
                actual = top(&arrayColas[i]); //Actualiza el valor del ticket actual
            } while (actual -> ID != IDtop);
            puts("");
        }
    }
}

void procesarTicket(Cola *arrayColas) {
    Ticket *actual;
    if ((actual = top(&arrayColas[PRIO_ALTA])) != NULL) {
        printearTicket(actual);
        dequeue(&arrayColas[PRIO_ALTA]);
        free(actual);
    }
    else {
        if ((actual = top(&arrayColas[PRIO_MEDIA])) != NULL) {
            printearTicket(actual);
            dequeue(&arrayColas[PRIO_MEDIA]);
            free(actual);
        }
        else {
            if ((actual = top(&arrayColas[PRIO_BAJA])) != NULL) {
                printearTicket(actual);
                dequeue(&arrayColas[PRIO_BAJA]);
                free(actual);
            }
            else {
                puts("No hay tickets pendientes");
            }
        }
    }
}

