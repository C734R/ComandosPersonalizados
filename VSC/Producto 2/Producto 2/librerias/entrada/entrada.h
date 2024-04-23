#pragma once
#ifndef ENTRADA_H
#define ENTRADA_H

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdbool.h>

// Declarar funciones
char * entradaSinRetorno (char * destino);
char * entradaConRetorno (char * destino);
bool vaciarEntrada(const char *entrada);
bool eliminarNL(char *entrada);

#endif