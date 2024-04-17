// Diseñar una función cuya funcionalidad que muestre los adaptadores de red de la máquina local. 
// Se mostrará y almacenará en un archivo adaptador.txt la información para un adaptador de red; 
// él cual, se preguntará previamente al usuario (elegido por el usuario) su IP, máscara, y puerta 
// de enlace. El resto de configuraciones de red no se han de mostrar esta información. Para 
// realizar lo anterior la función lanzará un comando de dos que le proporcionará la información 
// que necesita junto con otra que no se considera relevante, y será capaz de extraer y mostrar 
// la que se ha detallado. (menú punto3)*/
#ifndef adaptador_h
#define adaptador_h

// Añadir librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "auxiliares.h"

// Declaración de variables
char _buffer[1024];
char _entradaProcesada[1024];
char comando[1024];

bool dentroBloque = false;
FILE *consola = NULL;
int i = 0;

// Declaración de funciones
void copiarAdaptadorRed(FILE *archivoParam);
void addAdaptadorRed(FILE *archivoParm);
bool mostrarAdaptadores(void);
bool encontrarAdaptador(char *nAdaptador);

// Función para copiar la información de un adaptador de red en un archivo
void copiarAdaptadorRed(FILE *archivoParam) {
    // Si no se han podido mostrar los adaptadores
    if (!mostrarAdaptadores()) {
        return;
    }
    // Si se han podido mostrar
    else {
        // Pedir el nombre del adaptador de red
        printf("Introduce el nombre del adaptador de red del que quieres guardar su información: ");
        // Leer el nombre del adaptador de red
        strcpy(_entradaProcesada, entradaSinRetorno(_buffer, stdin));
        // Mostrar un mensaje de éxito
        printf("Adaptador de red introducido: %s\n", _entradaProcesada);
        // Si no se ha encontrado el adaptador
        if (!encontrarAdaptador(_entradaProcesada)) {
            return;
        }
        // Si se ha podido ejecutar el comando ipconfig
        else {
            // Crear el comando para obtener la información del adaptador de red 
            sprintf(comando, "ipconfig | findstr /C:\"%s\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i", _entradaProcesada);
            // Ejecutar el comando definido
            consola = _popen(comando, "r");
            // Inicializar contador
            i = 0;
            // Mientras haya respuesta del comando ipconfig
            while (fgets(_buffer, sizeof(_buffer), consola) != NULL) {
                // Detectar el inicio de un bloque de adaptador
                if (strstr(_buffer, "Adaptador de Ethernet") != NULL || strstr(_buffer, "Adaptador de LAN inal") != NULL){
                    // Verificar si es el bloque del adaptador solicitado
                    if (strstr(_buffer, _entradaProcesada) != NULL) {
                        // Mostrar encabezado
                        printf("--- Datos del adaptador guardado ---\n");
                        // Establecer que estamos dentro del bloque del adaptador
                        dentroBloque = true;
                        // Añadir un separador en el archivo
                        fprintf(archivoParam, "-----------------------------------\n");
                        // Añadir un título en el archivo
                        fprintf(archivoParam, "--- Datos del adaptador guardado ---\n");
                        // Escribir la información en el archivo adaptador.txt
                        fprintf(archivoParam, "%s", _buffer); 
                        // Mostrar la línea por pantalla
                        printf("%s", _buffer); 
                    } 
                    // Si no es el bloque del adaptador solicitado
                    else {
                        // Establecer que no estamos dentro del bloque del adaptador
                        dentroBloque = false;
                    }
                }
                // Si estamos dentro del bloque del adaptador
                if (dentroBloque) {
                    // Si la línea contiene información relevante
                    if (strstr(_buffer, "IPv4") != NULL || strstr(_buffer, "subred") != NULL || strstr(_buffer, "enlace") != NULL) {
                        // Escribir la información en el archivo adaptador.txt
                        fprintf(archivoParam, "%s", _buffer);
                        // Mostrar la línea
                        printf("%s", _buffer);
                        i++;
                        if (i == 3){
                            break;
                        }
                    }
                }
            }

            // Añadir un separador en el archivo
            fprintf(archivoParam, "-----------------------------------\n\n");
            // Mostrar separador por pantalla
            printf("-----------------------------------\n\n");
            // Mostrar un mensaje de éxito
            printf("Información del adaptador de red guardada en adaptador.txt.\n\n");
            // Cerrar el archivo
            fclose(archivoParam);
            // Cerrar la conexión con el comando ipconfig
            _pclose(consola);
        }
    }
}

void addAdaptadorRed(FILE *archivoParm) {
    
    // Declaración de variables
    bool bAdaptador = false;

    // Mientras el nombre de adaptador introducido exista
    while (1){
        // Si no se han podido mostrar los adaptadores
        if (!mostrarAdaptadores()) {
            // Salir del bucle
            break;
        }
        // Si se han podido mostrar
        else {
            // Pedir el nombre del adaptador de red
            printf("Introduce el nombre del adaptador de red que quieres añadir: ");
            // Leer el nombre del adaptador de red
            strcpy(_entradaProcesada, entradaSinRetorno(_buffer, stdin));
            // Mostrar un mensaje de éxito
            printf("Adaptador de red introducido: %s\n", _entradaProcesada);
            // Si se encuentra el adaptador
            if(encontrarAdaptador(_entradaProcesada)){
                // Mostrar un mensaje de error
                printf("El adaptador ya existe. Introduce otro nombre.\n\n");
                // Registramos que el adaptador ya existe
                bAdaptador = false;
            }
            // Si no se encuentra el adaptador
            else {
                // Registramos que no existe el adaptador
                bAdaptador = true;
                // Salimos del bucle
                break;
            }   
        }
    }
    // Si no existe el adaptador
    if (bAdaptador){
        // Añadir separador en el archivo
        fprintf(archivoParm, "-----------------------------------\n\n");
        // Registrar el nombre del adaptador en el archivo adaptador.txt
        fprintf(archivoParm, "Adaptador de Red %s\n", _entradaProcesada);
        // Añadir un separador en el archivo
        fprintf(archivoParm, "-----------------------------------\n\n");
        // Mostrar un mensaje de éxito
        printf("Nombre del adaptador registrado con éxito.\n\n");

        // Mientras los carácteres introducidos no cumplan con el formato de una IP
        while (1){
            // Solicitar la IP
            printf("Introduce la IP: ");
            strcpy(_entradaProcesada, entradaSinRetorno(_buffer, stdin));
            // Si la IP no es válida
            if (!validarIP(_entradaProcesada)){
                // Mostrar un mensaje de error
                printf("La IP introducida no es válida. Introduce una IP válida.\n");
            }
            // Si la IP es válida
            else {
                // Registrar la IP en el archivo adaptador.txt
                fprintf(archivoParm, "Dirección IPv4: %s\n", _entradaProcesada);
                // Salir del bucle
                break;
            }
        }
        // Mientras los carácteres introducidos no cumplan con el formato de una máscara
        while (1){
            // Solicitar la máscara
            printf("Introduce la máscara de subred: ");
            strcpy(_entradaProcesada, entradaSinRetorno(_buffer, stdin));
            // Si la máscara no es válida
            if (!validarIP(_entradaProcesada)){
                // Mostrar un mensaje de error
                printf("La máscara introducida no es válida. Introduce una máscara válida.\n");
            }
            // Si la máscara es válida
            else {
                // Registrar la máscara en el archivo adaptador.txt
                fprintf(archivoParm, "Máscara: %s\n", _entradaProcesada);
                // Salir del bucle
                break;
            }
        }
        // Mientras los carácteres introducidos no cumplan con el formato de una puerta de enlace
        while (1){
            // Solicitar la puerta de enlace
            printf("Introduce la puerta de enlace: ");
            // Leer la puerta de enlace
            strcpy(_entradaProcesada, entradaSinRetorno(_buffer, stdin));
            // Si la puerta de enlace no es válida
            if (!validarIP(_entradaProcesada)){
                // Mostrar un mensaje de error
                printf("La puerta de enlace introducida no es válida. Introduce una puerta de enlace válida.\n");
            }
            // Si la puerta de enlace es válida
            else {
                // Registrar la puerta de enlace en el archivo adaptador.txt
                fprintf(archivoParm, "Puerta de enlace: %s\n", _entradaProcesada);
                // Salir del bucle
                break;
            }
        }
        // Añadir un separador en el archivo
        fprintf(archivoParm, "-----------------------------------\n\n");
        // Volver al inicio del archivo
        rewind(archivoParm);
        // Mostar el contenido del archivo adaptador.txt
        while (fgets(_buffer, sizeof(_buffer), archivoParm) != NULL) {
            // Mostrar el contenido del archivo
            printf("%s", _buffer);
        }
        // Mostrar un mensaje de éxito
        printf("Se han registrado con éxito los datos del adaptador introducidos.\n\n");
        // Cerrar el archivo
        fclose(archivoParm);
    }
}

bool mostrarAdaptadores(void){

    // Definimos el comando a ejecutar para obtener los adaptadores de red
    sprintf(comando,"ipconfig | findstr /C:\"Adaptador\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i");
    // Ejecutar el comando definido
    consola = _popen(comando, "r");
    // Si no se ha podido ejecutar el comando ipconfig
    if (consola == NULL) {
        // Mostrar un mensaje de error
        printf("Error al ejecutar el comando ipconfig.\n");
        // Devolver falso
        return false;
    }
    // Si se ha podido ejecutar el comando ipconfig
    else {
        // Mostrar un mensaje de éxito
        printf("Comando ipconfig ejecutado con éxito.\n\n");
        // Mostrar los adaptadores de red
        printf("--- Adaptadores de red ---\n");
        // Mostrar los datos de ipconfig
        while (fgets(_buffer, sizeof(_buffer), consola) != NULL) {
            printf("%s", _buffer);
        }
        // Añadir un separador por pantalla
        printf("-----------------------------------\n\n");
        // Cerrar la conexión con el comando ipconfig
        _pclose(consola);
        // Devolver verdadero
        return true;
    }
}

bool encontrarAdaptador(char *nAdaptador){

    // Crear el comando para obtener la información del adaptador de red que coincida con la IP, la máscara y la puerta de enlace introducidas
    sprintf(comando, "ipconfig | findstr /C:\"%s\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i", nAdaptador);
    // Ejecutar el comando definido
    consola = _popen(comando, "r");
    // Si no se ha podido ejecutar el comando ipconfig
    if (consola == NULL) {
        // Mostrar un mensaje de error
        printf("Error al ejecutar el comando ipconfig.\n");
        return false;
    }
    else {
        // Mostrar un mensaje de éxito
        printf("Comando ipconfig ejecutado con éxito.\n\n");
        // Recorerr las líneas de la salida del comando ipconfig
        while (fgets(_buffer, sizeof(_buffer), consola) != NULL) {
            // Detectar el inicio de un bloque de adaptador
            if (strstr(_buffer, "Adaptador de Ethernet") != NULL || strstr(_buffer, "Adaptador de LAN inal") != NULL){
                // Verificar si encontramos el adaptador solicitado
                if (strstr(_buffer, nAdaptador) != NULL) {
                    // Cerrar la conexión con el comando ipconfig
                    _pclose(consola);
                    // Registramos que el adaptador ya existe
                    return true;
                }
            }
        }
        // Cerrar la conexión con el comando ipconfig
        _pclose(consola);
        // Registramos que no existe el adaptador
        return false;
    }
}
#endif