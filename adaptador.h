#ifndef ADAPTADOR_H     // Si ADAPTADOR_H no ha sido definido
#define ADAPTADOR_H     // Definimos

// Añadir librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "entrada.h"

// Declaración de funciones
void copiarAdaptadorRed(FILE *archivoParam);
void addAdaptadorRed(FILE *archivoParm);
bool mostrarAdaptadores(void);
bool encontrarAdaptador(char *nAdaptador);

#endif      // Fin definición ADAPTADOR_H