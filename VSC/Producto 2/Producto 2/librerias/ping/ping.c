// -- Comprobar e informar por pantalla sobre cuáles de una lista de máquinas referenciadas 
// por su IP escrita en un archivo de texto, contestan positivamente a un ping lanzado por la 
// aplicación que estás programando. Se recomienda generar un submenú donde se solicite el 
// nombre y ubicación de dicho archivo.*/
// -- Diseñar una función que lea el contenido de un archivo de texto que consiste en una 
// serie de direcciones IP cada una de las cuales ocupa una línea de éste. La ruta y nombre 
// del archivo de texto se preguntarán al usuario e inicialmente se mostrará el contenido de 
// este por pantalla. Finalmente lanzará un ping para cada una de las IP’s y determinará e 
// informará sobre cuáles de las anteriores responden afirmativamente a este comando. (Menú punto 2)*/
// -- Se almacenará el resultado del paso anterior debajo de la fecha y la hora en el archivo 
// producto2.txt, las IPs que han dado una respuesta positiva. Guardar el archivo y no borrar. (Menú punto 2)

#ifndef PING_C 
#define PING_C

// Inclusión de las librerías necesarias
#include "ping.h"
#include "../archivo/archivo.h"
#include "../entrada/entrada.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Función para comprobar ping a IPs
void pingIPs(FILE *archivoParam, char *rutaParam) {
    
    // Declaración de variables
    char ip[16];
    FILE *archivoIPs = 0;
    char comando[100];
    char lectura[100];
    char entradaProcesada[100];
    int totalIPs = 0;
    int numIPs = 0;
    char busqueda [200];
    char buffer[100];

    // Declaramos array para almacenar las IPs que responden al ping
    char ipPositivas[10][16] = { 0 };

    // Mientras no se haya seleccionado una ruta válida
    while (1){
        // Pedimos al usuario la ruta del archivo que contiene las IPs
        printf("Introduce la ruta del archivo que contiene las IPs a testear: ");
        // Leer la ruta del archivo
        strcpy_s(entradaProcesada, (size_t)entradaProcesada, entradaSinRetorno(buffer));
        // Comprobamos si el fichero indicado existe
        if (!existeArchivo(entradaProcesada) && vaciarEntrada(buffer)){
            // Si no existe, mostramos un mensaje de error
            printf("El archivo no existe. ¿Quieres crearlo e introducir las IP? (s = sí, otro = no) ");
            // Si el usuario no quiere crear el archivo
            if (getchar() != 's') {
                // Salir del bucle
                break;
            }
            // Si el usuario quiere crear el archivo
            else {
                
                // Si no se ha podido abrir el archivo
                if (abrirArchivo(entradaProcesada, "wt+", &archivoParam)) {
                    // Mostrar un mensaje de error
                    printf("Error al abrir el archivo ubicado en la ruta : %s\n", entradaProcesada);
                    // Salir del bucle
                    break;
                }
                // Si se ha podido abrir el archivo
                else {
                    // Mostrar un mensaje de éxito
                    printf("Archivo creado con éxito.\n\n");
                    while (1) {
                        // Pedir al usuario que introduzca las IPs a testear
                        printf("Introduce la IP a testear (o 'fin' para terminar): ");
                        strcpy_s(entradaProcesada,(size_t)entradaProcesada, entradaSinRetorno(buffer, stdin));
                        // Si el usuario introduce 'fin', salir del bucle
                        if (strcmp(ip, "fin") == 0) {
                            // Salir del bucle
                            break;
                        }
                        // Escribir la IP en el archivo
                        fprintf(archivoIPs, "%s\n", ip);
                        // Vaciar el buffer de entrada
                        vaciarEntrada(buffer);
                    }
                    // Mostrar un mensaje de éxito
                    printf("IPs introducidas en el archivo.\n\n");
                    // Cerrar el archivo
                    fclose(archivoIPs);
                    // Salir del bucle
                    break;
                }
            }
        }
        // Si el archivo existe
        else {
            // Si existe, mostramos un mensaje de éxito
            printf("Archivo encontrado.\n\n");
            // Salimos del bucle
            break;
        }
    }

    // Si no se ha podido abrir el archivo
    if (!abrirArchivo(entradaProcesada, "r", &archivoIPs)){
        // Salir de la función
        return;
    }
    // Si se ha podido abrir el archivo
    else {
        // Mostrar un mensaje con las IPs que se van a testear
        printf("--- Testearemos las siguientes IPs ---\n");
        // Mientras haya IPs en el archivo, incrementar el contador de IPs
        while (fscanf_s(archivoIPs, "%s", ip, 16) != EOF) {
            // Mostrar la IP que vamos a comprobar
            printf("%s\n", ip);
            // Incrementar el contador de IPs
            totalIPs++;
        }
        // Añadir un separador por pantalla 
        printf("-----------------------------------\n\n");
        // Volver al inicio del archivo
        rewind(archivoIPs);
        // Mostramos un encabezado
        printf("--- Testeando IPs ---\n");
        // Leer las IPs del archivo y comprobar si responden al ping
        while (fscanf_s( archivoIPs, "%s", ip, 16) != EOF) {
            // Mostrar la IP que vamos a comprobar
            printf("Testeando IP: %s ...\n", ip);
            // Crear el comando ping, de una sola prueba, con la IP escaneada del archivo
            sprintf_s(comando, (size_t)comando, "ping -n 1 %s", ip);
            // Ejecutar el comando ping
            FILE *ping = _popen(comando, "r");
            // Esperar antes de leer la respuesta
            Sleep(1000);
            // Mientras haya respuesta del comando ping
            while (fgets(lectura, sizeof(lectura), ping) != NULL) {
                // Concatenar la respuesta con la IP
                sprintf_s(busqueda,(size_t)busqueda, "Respuesta desde %s", ip);
                // Comprobar si la IP responde
                if (strstr(lectura, busqueda) != NULL) {
                    // Si la IP responde, guardarla en el array de IPs positivas
                    strcpy_s(ipPositivas[numIPs], (size_t)(ipPositivas), ip);
                    // Aumentar el contador de IPs positivas
                    numIPs++;
                    // Salir del bucle
                    break;
                }
            }
            // Cerrar la conexión con el comando ping
            _pclose(ping);
        }
        // Cerrar el archivo
        fclose(archivoIPs);
    }
    // Añadir un separador por pantalla 
    printf("-----------------------------------\n\n");
    // Ir al final del archivo
    fseek(archivoParam, 0, SEEK_END);
    // Obtener el tamaño del archivo en base a posición actual
    long int tam_archivo = ftell(archivoParam);
    // Mover el puntero al inicio del archivo
    rewind(archivoParam);
    // Reservar memoria para el contenido que vamos a leer del archivo añadiendo 1 byte para el carácter nulo
    char *contenido = (char *)malloc((size_t)(tam_archivo + 1));
    // Si no se pudo reservar memoria, informar del error y salir de la función
    if (contenido == NULL){
        // Informar del error
        printf("Error al reservar memoria para el contenido del archivo.\n");
        // Salir de la función
        return;
    }
    // Inicializar la variable contenido con una cadena vacía
    memset(contenido, 0, (size_t)(tam_archivo + 1));
    // Leer el contenido del archivo y almacenarlo en la variable contenido
    fread(contenido, 1, (size_t)contenido, archivoParam);
    // Añadir un carácter nulo al final del contenido, emulando el fin de cadena
    contenido[tam_archivo] = '\0';
    // Si no se ha podido vaciar el archivo
    if (!vaciarArchivo(rutaParam)){
        // Liberar la memoria reservada para el contenido
        free(contenido);
        // Salir de la función
        return;
    }
    // Si se ha podido vaciar el archivo
    else {
        // Añadir un separador al inicio de la entrada
        fprintf(archivoParam, "-----------------------------------\n");
        // Insertamos la fecha y hora en el archivo
        insertarFechaHora(archivoParam);
        // Añadimos un título al archivo
        fprintf(archivoParam, "---- IPs que responden al ping ----\n");
        // Recorremos el array de IPs positivas 
        for (int i = 0; i < numIPs; i++) {
            // Y las escribimos en el archivo
            fprintf(archivoParam, "%s\n", ipPositivas[i]);
        }
        // Añadir un separador al final de la entrada
        fprintf(archivoParam, "-----------------------------------\n");
        
        // Escribir el contenido al final del archivo
        fseek(archivoParam, 0, SEEK_END);
        // Si no se ha podido escribir el contenido en el archivo
        if(fwrite(contenido, 1, (size_t)contenido, archivoParam) != (size_t)tam_archivo){
            // Informar del error
            printf("Error al escribir el contenido en el archivo.\n\n");
            // Liberar la memoria reservada para el contenido
            free(contenido);
            // Salir de la función
            return;
        }

        // Liberar la memoria reservada para el contenido
        free(contenido);
        // Mostrar un mensaje de éxito
        printf("Archivo producto2.txt actualizado con las IPs que han dado respuesta.\n\n");
    }
}

// Función para validar una dirección IP
bool validarIP(const char *ip) {

    // Declaración de variables
    int num, puntos = 0;
    const char *puntero_ip = ip;

    // Si la ip es nula
    if (ip == NULL) {
        // Devolver falso
        return false;
    }
    // Mientras haya un carácter en la dirección IP
    while (*puntero_ip) {
        // Si no es un dígito
        if (*puntero_ip < '0' || *puntero_ip > '9') {
            // Si es un punto
            if (*puntero_ip == '.') {
                // Aumentar el contador de puntos
                puntos++;
                // Si hay más de 3 puntos o el último carácter es un punto
                if (puntos > 3 || *(puntero_ip - 1) == '.') {
                    // Devolver falso
                    return false;
                }
            } 
            // Si no es ni punto ni dígito
            else {
                // Devolver falso
                return false;
            }
        }
        // Mover el puntero al siguiente carácter
        puntero_ip++;
    }
    // S termina en punto o si la cantidad de puntos no es exactamente 3
    if (*(puntero_ip - 1) == '.' || puntos != 3) {
        // Devolver falso
        return false;
    }
    // Para cada segmento de la dirección IP
    for (int i = 0; i < 4; i++) {
        // Convertir el segmento a un número
        num = atoi(ip);
        // Si el número no está en el rango de 0 a 255
        if (num < 0 || num > 255) {
            // Devolver falso
            return false;
        }
        // Si no es el último segmento
        if (i < 3) {
            // Mover el puntero al siguiente segmento
            ip = strchr(ip, '.') + 1;
        }
    }
    // Si todo es correcto evolver verdadero
    return true;
}

#endif