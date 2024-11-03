
// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "entrada.h"

// Función para leer una entrada y eliminar el carácter de nueva línea
 char * entradaSinNL(char *destino, FILE *fuente, int tamBuffer) {
    // Leer la entrada
     printf("%i", tamBuffer);
     if (fgets(destino, tamBuffer, fuente) == NULL) {
         printf("Error al procesar cadena");
     }
    // Eliminar el carácter de nueva línea
    eliminarNL(destino);
    // Devolver el retorno
    return destino;
 }

// Función para leer una entrada y no eliminar el carácter de nueva línea
char * entradaConNL(char *destino, FILE *fuente,int tamBuffer) {
    // Leer la entrada y almacenarla en destino
    printf("%i", tamBuffer);
    if (fgets(destino, tamBuffer, fuente) == NULL) {
        printf("Error al procesar cadena");
    }    // Devolver la entrada
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

// Función para esperar a pulsar enter
void pausaEnter() {
    printf("Pulsa Enter para continuar...");
    getchar();
}