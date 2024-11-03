// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdbool.h>
#include "archivo.h"

// Función para abrir un archivo de la ruta especificada en el modo especificado y almacenar el puntero al archivo
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam) {

    // Abrir el archivo en modo lectura
    *archivoParam = fopen(rutaParam, modoParam);
    // Si no se ha podido abrir el archivo
    if (*archivoParam == 0) {
        // Mostrar un mensaje de error
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", rutaParam);
        // Devolver falso
        return false;
    }
    // Si se ha podido abrir el archivo
    else {
        // Mostrar un mensaje de éxito
        printf("Archivo abierto con éxito.\n\n");
        // Devolver verdadero
        return true;
    }
}

bool existeArchivo(char *rutaParam) {
    // Abrir el archivo en modo lectura
    FILE *archivo = fopen(rutaParam, "r");
    // Si no se ha podido abrir el archivo
    if (archivo == 0) {
        // Devolver falso
        return false;
    }
    // Si se ha podido abrir el archivo
    else {
        // Cerrar el archivo
        fclose(archivo);
        // Devolver verdadero
        return true;
    }
}

bool vaciarArchivo(char *rutaParam) {
    // Abrir el archivo en modo escritura
    FILE *archivo = fopen(rutaParam, "w");
    // Si no se ha podido abrir el archivo
    if (archivo == 0) {
        // Mostrar un mensaje de error
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", rutaParam);
        // Devolver falso
        return false;
    }
    // Si se ha podido abrir el archivo
    else {
        // Mostrar un mensaje de éxito
        printf("Archivo vaciado con éxito.\n\n");
        // Cerrar el archivo
        fclose(archivo);
        // Devolver verdadero
        return true;
    }
}