#ifndef ARCHIVO_C
#define ARCHIVO_C

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdbool.h>

// Función para abrir un archivo de la ruta especificada en el modo especificado y almacenar el puntero al archivo
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam) {

    // Abrir el archivo en modo lectura
    fopen_s((FILE **)archivoParam, rutaParam, modoParam);
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
    FILE **archivo = 0;
    // Abrir el archivo en modo lectura
    fopen_s(archivo, rutaParam, "r");
    // Si no se ha podido abrir el archivo
    if (archivo == 0) {
        // Devolver falso
        return false;
    }
    // Si se ha podido abrir el archivo
    else {
        // Cerrar el archivo
        fclose(*archivo);
        // Devolver verdadero
        return true;
    }
}

bool vaciarArchivo(char *rutaParam) {
    FILE **archivo = 0;
    // Abrir el archivo en modo escritura
    fopen_s(archivo,rutaParam, "w");
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
        fclose(*archivo);
        // Devolver verdadero
        return true;
    }
}

#endif