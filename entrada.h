
#ifndef ENTRADA_H
#define ENTRADA_H

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Declarar funciones
char * entradaSinRetorno (char * destino, FILE * fuente);
bool vaciarEntrada(const char *entrada);
bool eliminarNL(char *entrada);

// Función para leer una entrada y eliminar el carácter de nueva línea
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

// Función para leer una entrada y no eliminar el carácter de nueva línea
char * entradaConRetorno(char *destino, FILE *fuente) {
    // Leer la entrada y almacenarla en destino
    fgets(destino, (size_t)(destino), fuente);
    // Devolver la entrada
    return destino;
}

// Función para vaciar el buffer de entrada
bool vaciarEntrada (const char *entrada) {
    // Si el último carácter de la cadena no es un salto de línea
    if(entrada[strlen(entrada) - 1] != '\n'){
      //Limpiamos el búfer
      while(getchar() != '\n');
    }
    // Devolver verdadero
    return true;
}

// Función para eliminar el carácter de nueva línea
bool eliminarNL (char *entrada) {
    // Eliminar el carácter de nueva línea
    entrada[strlen(entrada) - 1] = '\0';
    // Devolver verdadero
    return true;
}

#endif