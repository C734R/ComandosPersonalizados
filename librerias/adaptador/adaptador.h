#ifndef adaptador_h
#define adaptador_h

// Añadir librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../entrada/entrada.h"

// Declaración de funciones
void copiarAdaptadorRed(FILE *archivoParam);
void addAdaptadorRed(FILE *archivoParm);
bool mostrarAdaptadores(void);
bool encontrarAdaptador(char *nAdaptador);

#endif