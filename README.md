# Sistema de Gestión de Tickets de Servicio Tecnico

## Descripción

Esta aplicacion permite gestionar diferentes peticiones de servicio tecnico (tickets). La aplicacion permite registrar tickets, asignar prioridades, ver los tickets pendientes, procesar el siguiente ticket en la lista segun su prioridad y buscar algun ticket por su ID. Creada con el fin de facilitar la gestion en centros de servicio tecnico.

## Cómo compilar y ejecutar
Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack** de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** archivo `.zip` en una carpeta de tu elección.
2. **Abre el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. **Compila el código**
    - Abre el archivo principal (`main.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando:
        
        ```bash
        gcc *.c -o gestionTickets
        ```
        
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
        
        ```
        ./gestionTickets
        ```
        

## Funcionalidades

### Funcionando correctamente:
Asumiendo que el usuario nunca comete errores y entrega datos pertinentes a la app (ej: entrega un numero del 1 al 6 en un menu de 6 opciones, no entrega palabras cuando se piden numeros, etc)

- Registrar tickets, con su ID, la descripcion del problema, la hora de ingreso (obtenida automaticamente)
- Asignar o modificar la prioridad de los diferentes tickets.
- Ver la lista de tickets pendientes, ordenada por prioridad y hora de registro.
- Procesar el siguiente ticket, respetando el orden de prioridad.

### A mejorar:

- Mejorar el formato del texto mostrado en pantalla.
- Aceptar todo tipo de inputs, aceptando solo los que sean validos.

## Ejemplo de uso

**Paso 1: Registrar un Nuevo Ticket**

Se registra un nuevo ticket de servicio tecnico.

```
Opción seleccionada: 1) Registrar Ticket
Ingrese la ID del ticket: 1001
Describa su problema: Problema con la impresora
```

La aplicacion agregara esta informacion a un ticket con la prioridad por defecto "Baja" y con la hora actual de registro. La prioridad se puede ajustar mas tarde si se desea.

**Paso 2: Asignar Prioridad a un Ticket**

```
Opción seleccionada: 2) Asignar Prioridad a Ticket
Ingrese la ID del ticket: 1001
Seleccione el nuevo nivel de prioridad (Alto, Medio, Bajo): Medio
```

La aplicacion actualizara el nivel de prioridad del ticket con ID 1001 a Medio

**Paso 3: Ver los tickets pendientes**

El usuario podra ver todos los tickets (id, descripcion, hora y prioridad), separados segun su prioridad.

```
Opción seleccionada: 3) Mostrar Lista de Tickets Pendientes
```

La lista muestra el ticket con ID 1001 en la posicion mas alta, indicando que es el siguiente en la cola.

**Paso 4: Procesar el siguiente ticket**

```
Opción seleccionada: 4) Procesar siguiente Ticket
```
El sistema muestra la informacion del ticket con mayor prioridad y lo elimina de la lista.

**Opcion 5: Buscar un ticket por su ID**

```
Opción seleccionada: 5) Buscar Por ID
```
El sistema muestra la informacion del ticket con la ID proporcionada, sin alterar el orden de los otros tickets.

**Opcion 6: Salir del programa**

```
Opción seleccionada: 6) Salir
```
El programa limpia todo lo que tenga almacenado en ese momento y luego se cierra.


