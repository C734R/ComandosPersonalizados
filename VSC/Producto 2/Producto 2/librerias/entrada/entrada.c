#ifndef ENTRADA_C
#define ENTRADA_C

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "entrada.h"

// Función para leer una entrada y eliminar el carácter de nueva línea
 char * entradaSinRetorno(char *destino) {
	// Leer la entrada y almacenarla en destino
	fgets(destino, (size_t)destino, stdin);
	// Eliminar el carácter de nueva línea
	eliminarNL(destino);
	// Devolver destino
	return destino;
 }

// Función para leer una entrada y no eliminar el carácter de nueva línea
char * entradaConRetorno(char *destino) {
    // Leer la entrada y almacenarla en destino
    fgets(destino, (size_t)destino, stdin);
    // Devolver destino
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