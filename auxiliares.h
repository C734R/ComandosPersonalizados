
#ifndef AUXILIARES_H
#define AUXILIARES_H

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Declarar funciones
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam);
char * entradaSinRetorno (char * destino, FILE * fuente);
bool vaciarEntrada(const char *str);
bool eliminarNL(char *str);
bool existeArchivo(char *rutaParam);
bool vaciarArchivo(char *rutaParam);

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

// Función para leer una entrada y vaciar el buffer de entrada
 char * entradaSinRetorno(char *destino, FILE *fuente) {
    // Declarar una variable para almacenar el retorno
    char * retorno = (char *) malloc(sizeof(destino));
    // Leer la entrada
    fgets(destino, (size_t)(destino), fuente); 
    // Eliminar el carácter de nueva línea
    eliminarNL(destino);
    // Volcar en retorno
    strcpy(retorno, destino);
    // Devolver el retorno
    return retorno;
 }

char * entradaConRetorno(char *destino, FILE *fuente) {
    // Leer la entrada
    fgets(destino, (size_t)(destino), fuente);
    // Devolver la entrada
    return destino;
}

// Función para vaciar el buffer de entrada
bool vaciarEntrada (const char *str) {
    // Si el último carácter de la cadena no es un salto de línea
    if(str[strlen(str) - 1] != '\n'){
      //Limpiamos el búfer
      while(getchar() != '\n');
    }
    return true;
}

bool eliminarNL (char *str) {
    // Eliminar el carácter de nueva línea
    str[strlen(str) - 1] = '\0';
    return true;
}

#endif