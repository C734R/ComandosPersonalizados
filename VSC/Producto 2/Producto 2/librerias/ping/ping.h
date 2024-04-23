#pragma once
#ifndef ping_h
#define ping_h

#include <stdio.h>
#include <stdbool.h>

//Definimos funciones
void pingIPs(FILE *archivoParam, char *rutaParam);
bool validarIP(const char *ip);

#endif
