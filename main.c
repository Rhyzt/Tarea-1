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
    char descripcion[100];
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
void verificarOpcion(int *num);
void printearTicket(Ticket *ticket);
char *obtenerHoraActual();
void vaciarColas(Cola *arrayColas);
void registrarTicket(int ID, char *descripcion, Cola *arrayColas);
bool asignarPrioridad(int ID, char *prio, Cola *arrayColas);
void mostrarCola(Cola *arrayColas);
void procesarTicket(Cola *arrayColas);
void esperarAccion();
Busqueda *buscarID(int ID, Cola *arrayColas);


void main() {
    //freopen("entrada.txt", "r", stdin); //Usado para debuggear
    Cola *arrayColas = inicializarColas(); //Crea las diferentes colas de prioridad
    while(1) { //Usado para que el programa no termine, ya que este debe ser cerrado usando una opcion la que dara un exit
        mostrarMenu(); //Muestra las diferentes opciones al usuario
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
    puts("Seleccione una opcion: ");
}

void vaciarColas(Cola *arrayColas) { //Vacia las colas, usado para cuando se termina el programa
    for (int i = 0 ; i < 3 ; i++) { //Recorre el array de Colas
        Ticket *actual = top(&arrayColas[i]);
        while (actual != NULL) { // Recorre las colas, haciendo dequeue y free a cada elemento
            dequeue(&arrayColas[i]);
            free(actual);
            actual = top(&arrayColas[i]);
        }
    }
}

void esperarAccion() {
    puts("Presione enter para continuar");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    puts(""); 
}

void verificarOpcion(int *num) {
    char str[3];
    while (1) {
        fgets(str, 3, stdin);
        if (str[strlen(str) - 1] != '\n') { //Se revisa si el usuario escribio mas de 2 caracteres
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF); // Limpiar stdin para leer correctamente el proximo input
        }
        else {
            if (isdigit(str[0]) && str[1] == '\n') { //En caso de que el numero ingresado no sea valido
                *num = str[0] - '0';
                if (*num > 0 && *num < 7) break;
            }
        }
        puts("Ingresa una opcion Valida");
        esperarAccion();
    }
}

int verificarNumero() {
    char str[11];
    while (1) {
        fgets(str, 11, stdin);
        if (str[strlen(str) - 1] != '\n') { //Se revisa si el usuario escribio mas de 9 caracteres
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF); // Limpiar stdin para leer correctamente el proximo input
            puts("Solo se aceptan hasta un maximo de 9 digitos");
        }
        str[strcspn(str, "\n")] = '\0'; // Cambia el salto de linea por el caracter nulo
        bool flag = true;
        if (strlen(str) == 0) { // Revisa si la cadena esta vacia
            flag = false;
            puts("No se ingreso nada");
        }
        else {
            for (int i = 0 ; str[i] != '\0' ; i++) { //Revisa si la cadena ingresada son solamente numeros
                if (!isdigit(str[i])) {
                    puts("Solo se aceptan numeros");
                    flag = false;
                    break;
                }
            }
        }
        if (flag) {
            long num = strtol(str, NULL, 10);
            return (int)num;
        }

    }
}

void menuOpciones(Cola *arrayColas) { //Tomara la respuesta del usuario y ejecutara la instruccion pertinente
    int num;
    verificarOpcion(&num); //Verifica que la opcion ingresada sea valida
    switch (num) { //Se usa switch para separar segun el valor ingresado
        case 1: { //Registrar Ticket
            int IDTemp;
            char descTemp[100];
            printf("Ingrese la ID del ticket: "); //Obtener informacion del ticket
            IDTemp = verificarNumero();
            puts("");
            printf("Describa su problema: ");
            fgets(descTemp, sizeof(descTemp), stdin);
            descTemp[strcspn(descTemp, "\n")] = '\0';

            registrarTicket(IDTemp, descTemp, arrayColas); //Registra los datos ingresados en un ticket con prioridad Baja
            puts("");
            break;
        }
        case 2: { //Asignar Prioridad
            int IDTemp;
            char prio[10];
            printf("Ingrese la ID del ticket: "); //Obtener informacion del ticket
            IDTemp = verificarNumero();
            puts("");
            printf("Seleccione el nuevo nivel de prioridad (Alto, Medio, Bajo): "); //Obtener informacion del ticket
            fgets(prio, sizeof(prio), stdin);
            prio[strcspn(prio, "\n")] = '\0';
            for (int i = 0 ; prio[i] != '\0' ; i++)
                prio[i] = toupper(prio[i]);
            asignarPrioridad(IDTemp, prio, arrayColas);
            break;
        }
        case 3: { //Mostrar lista
            mostrarCola(arrayColas);
            esperarAccion();
            break;
        }
        case 4: { //Procesar ticket
            procesarTicket(arrayColas);
            esperarAccion();
            break;
        }
        case 5: { //Buscar ticket
            int IDTemp;
            printf("Ingrese la ID del ticket: "); //Obtener informacion del ticket
            IDTemp = verificarNumero();
            Busqueda *search = buscarID(IDTemp, arrayColas);
            if (search -> ticketEncontrado)
                printearTicket(search -> ticketEncontrado);
            else
                puts("Ticket no Encontrado");
                puts("");
            free(search);
            esperarAccion();
            break;
        }
        case 6: { //Fin del programa
            vaciarColas(arrayColas);
            free(arrayColas);
            exit(EXIT_SUCCESS);
        }
    }
    puts("");
}

Cola *inicializarColas() {
    Cola *arrayColas = (Cola *)malloc(3 * sizeof(Cola)); //Se crea un arreglo dinamico para almacenar las 3 colas
    if (arrayColas == NULL) {
        puts("No se pudo asignar memoria correctamente");
        exit(EXIT_FAILURE);
    }
    for (int i = 0 ; i < 3 ; i++) { //Se inicializan las 3 colas de prioridad
        arrayColas[i] = *createQueue();
    }
    return arrayColas;
}

void printearTicket(Ticket *ticket) {
    printf("======= Ticket #%d =======\n", ticket->ID); //Printea la informacion de un ticket (ID, descripcion, hora, prioridad)
    printf("Descripcion: %s\n", ticket -> descripcion);
    printf("Hora: %s\n", ticket->horaReg);
    printf("Prioridad: ");
    if (ticket -> prioridad == 0) puts("Alta");
    else {
        if (ticket -> prioridad == 1) puts("Media");
        else puts("Baja");
    }
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
    strftime(horaStr, 9,"%H:%M:%S", horaPtr); //Se formatea la hora (HH:MM:SS) y se guarda en horaStr como string
    
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
    char *hora = obtenerHoraActual(); //Obtiene la hora actual en formato HH:MM:SS 
    strcpy(nuevoTicket -> horaReg, hora); // La hora es copiada dentro del ticket
    free(hora); //Libera la memoria usada despues de ser copiada
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
    informacion -> ticketEncontrado = NULL;
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
            else {
                if (strcmp(prio, "ALTO") == 0)
                    nuevaPrioridad = PRIO_ALTA;
                else {
                    puts("No se ingreso una prioridad valida.");
                    free(informacion);
                    esperarAccion();
                    return false;
                }
            }
        }

        if (informacion -> prioridad == nuevaPrioridad) {
            free(informacion); //Se libera la memoria de la busqueda
            return true; //Si ya pertenece a esta prioridad, la funcion no hace nada
        }
        Ticket *actual = top(&arrayColas[informacion -> prioridad]); //Se asigna un puntero para recorrer la cola correspondiente del dato
        int IDtop = actual -> ID;
        enqueue(&arrayColas[nuevaPrioridad], informacion -> ticketEncontrado); // Se agrega el ticket a la cola correspondiente
        informacion -> ticketEncontrado -> prioridad = nuevaPrioridad; // Se cambia la prioridad del struct del ticket
        char *hora = obtenerHoraActual();
        strcpy(informacion -> ticketEncontrado -> horaReg,hora); // Se cambia la hora del ticket cambiado de prioridad
        free(hora);
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
    puts("No existe un ticket con esta ID"); //Se avisa que no se encontro un ticket
    esperarAccion();
    return false; // Si no existe el ticket, no es necesario hacer la comprobacion dentro de colas ya que directamente no se podria encontrar
}

void mostrarCola(Cola *arrayColas) {
    const char *prioridades[] = {"Alta", "Media", "Baja"};
    bool flag = false; // Para revisar si se encontro al menos una lista con tickets
    for(int i = 0; i < 3 ; i++) { // Se recorren las 3 colas de prioridad
        if (top(&arrayColas[i]) != NULL) {
            flag = true; // Se encontro al menos un ticket
            printf("---- Prioridad %s ----\n", prioridades[i]);// Separador por prioridad
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
    if (!flag)
        puts("No se encontro ningun ticket");
}

void procesarTicket(Cola *arrayColas) {
    Ticket *actual;
    if ((actual = top(&arrayColas[PRIO_ALTA])) != NULL) { // Revisar cada cola desde la alta a la baja hasta encontrar alguna que tenga un ticket
        printearTicket(actual); // Se printea el ticket y luego se borra 
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
                puts("No hay tickets pendientes"); // Si no se encontraron tickets, arroja un mensaje
                puts("");
            }
        }
    }
}
