#pragma once
#ifndef adaptador_h
#define adaptador_h

// Añadir librerías necesarias
#include <stdbool.h>
#include <stdio.h>


// Declaración de funciones
void copiarAdaptadorRed(FILE *archivoParam);
void addAdaptadorRed(FILE *archivoParm);
bool mostrarAdaptadores();
bool encontrarAdaptador(char *nAdaptador);

#endif