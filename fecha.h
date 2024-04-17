// Insertar la fecha y la hora en el documento de salida: producto2.txt (Especificar en la memoria donde se genera dicho archivo) (Menú punto 1)
#ifndef fecha_h
#define fecha_h

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <time.h>

// Declarar funciones
void insertarFechaHora(FILE *archivoParam);

// Declaración de variables
char fechaHoraStr[100];

// Función para insertar la fecha y la hora en el archivo
void insertarFechaHora(FILE *archivoParam) {
   
    // Obtener la fecha y hora actual
    time_t tiempo = time(NULL);
    // Convertir la fecha y hora a una estructura de tiempo
    struct tm *fechaHora = localtime(&tiempo);
    // Formatear la fecha y hora
    strftime(fechaHoraStr, sizeof(fechaHoraStr), "%d/%m/%Y %H:%M:%S", fechaHora);
    // Escribir la fecha y hora en el archivo
    fprintf(archivoParam, "Fecha y hora: %s\n", fechaHoraStr);
    // Mostrar la fecha y hora por pantalla
    printf("Fecha y hora: %s\n", fechaHoraStr);
    // Mostrar un mensaje de éxito
    printf("Fecha y hora insertadas en el archivo.\n\n");
}

#endif