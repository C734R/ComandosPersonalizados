// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "ping.h"
#include "archivo.h"

// Función para comprobar ping a IPs
void pingIPs(FILE *archivoParam, char *rutaParam) {
    
    // Declaración de variables
    char ip[16];
    FILE *archivoIPs;
    char comando[100];
    char lectura[100];
    char entradaProcesada[100];
    int totalIPs = 0;
    int numIPs = 0;
    char busqueda [200];
    char buffer[100];

    // Declaramos array para almacenar las IPs que responden al ping
    char ipPositivas[10][16];

    // Mientras no se haya seleccionado una ruta válida
    while (1){
        // Pedimos al usuario la ruta del archivo que contiene las IPs
        printf("Introduce la ruta del archivo que contiene las IPs a testear: ");
        // Leer la ruta del archivo
        strcpy(entradaProcesada, entradaConNL(buffer, stdin, sizeOf(buffer)));
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
                    archivoIPs = archivoParam;
                    while (1) {
                        // Pedir al usuario que introduzca las IPs a testear
                        printf("Introduce la IP a testear (o 'fin' para terminar): ");
                        strcpy(entradaProcesada, entradaSinNL(buffer, stdin));
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
        printf("--- Testearemos las siguientes IPs ---\n");
        // Listamos y contamos las IP existentes
        while (fscanf(archivoIPs, "%s", ip) != EOF) {
            printf("%s\n", ip);
            totalIPs++;
        }
        printf("-----------------------------------\n\n");
        // Volver al inicio del archivo
        rewind(archivoIPs);
        // Mostramos un encabezado
        printf("--- Testeando IPs ---\n");
        // Leer las IPs del archivo y comprobar si responden al ping
        while (fscanf(archivoIPs, "%s", ip) != EOF) {
            printf("Testeando IP: %s ...\n", ip);
            // Crear y ejecutar el comando ping, de una sola prueba,
            // con la IP escaneada del archivo y esperamos un segundo a los resultados
            sprintf(comando, "ping -n 1 %s", ip);
            FILE *ping = _popen(comando, "r");
            // Esperar antes de leer la respuesta
            Sleep(1000);
            // Mientras haya datos de respuesta del comando ping concatenamos las respuestas con la IP
            while (fgets(lectura, sizeof(lectura), ping) != NULL) {
                
                sprintf(busqueda,"Respuesta desde %s",ip);
                if (strstr(lectura, busqueda) != NULL) {
                    // Si la IP responde, guardarla en el array de IPs positivas
                    strcpy(ipPositivas[numIPs], ip);
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
    fread(contenido, 1, (size_t)tam_archivo, archivoParam);
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
        fprintf(archivoParam, "-----------------------------------\n");
        insertarFechaHora(archivoParam);
        fprintf(archivoParam, "---- IPs que responden al ping ----\n");
        for (int i = 0; i < numIPs; i++) {
            fprintf(archivoParam, "%s\n", ipPositivas[i]);
        }
        fprintf(archivoParam, "-----------------------------------\n");
        
        // Escribir el contenido al final del archivo
        fseek(archivoParam, 0, SEEK_END);
        // Si no se ha podido escribir el contenido en el archivo informamos, liberamos memoria y salimos
        if(fwrite(contenido, 1, (size_t)tam_archivo, archivoParam) != (size_t)tam_archivo){
            printf("Error al escribir el contenido en el archivo.\n\n");
            free(contenido);
            return;
        }
        // Liberar la memoria reservada para el contenido y mensaje de éxito
        free(contenido);
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