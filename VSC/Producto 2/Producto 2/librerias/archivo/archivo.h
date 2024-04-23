#pragma once
#ifndef ARCHIVO_H
#define ARCHIVO_H

// Inclusión de las librerías necesarias
#include <stdio.h>

// Declarar funciones
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam);
bool existeArchivo(char *rutaParam);
bool vaciarArchivo(char *rutaParam);

#endif