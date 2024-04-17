#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares.h"
#include "ping.h"
#include "adaptador.h"
#include "fecha.h"

// Programa principal
int main() {

    // Declaración de variables
    char opcion[10];
    FILE *archivo = NULL;
    int opcion_int = -1;
    char buffer[1024];
    char entradaProcesada[1024];

    // Bucle mientras no se seleccione la opción de salir
    do {
        // Menú principal
        printf("Bienvenid@ al programa.\n");
        printf("--- Menú Principal ---\n");
        printf("1. Insertar fecha y hora.\n");
        printf("2. Comprobar IPs.\n");
        printf("3. Mostrar adaptadores de red y registrar seleccionado.\n");
        printf("4. Mostrar adaptadores de red y añadir uno diferente.\n");
        printf("5. Vaciar archivo.\n");
        printf("0. Salir del programa.\n");
        printf("Selecciona una opción: ");

        // Leer la opción
        entradaConRetorno(opcion, stdin);

        // Convertir la opción a un número
        opcion_int = atoi(opcion);

        vaciarEntrada(opcion);

        // Mientras la opción no sea un número
        while (opcion_int < 0 || opcion_int > 5){
            // Registrar opción inválida
            strcpy_s(opcion, sizeof(opcion), "-1");
            // Salir del bucle
            break;
        }

        // Convertir la opción a un número
        switch (opcion_int){
            case 1:
                // Si se ha podido abrir el archivo en modo escritura y lectura al inicio del archivo
                if (abrirArchivo("C:/temp/producto2.txt", "rt+", &archivo)){
                    // Llamamos a la función para insertar la fecha y hora en el archivo
                    insertarFechaHora(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                };               
                break;
            case 2:
                // Si se ha podido abrir el archivo en modo escritura y lectura al inicio del archivo
                if (abrirArchivo("C:/temp/producto2.txt", "at+", &archivo)){
                    // Llamamos a la función para comprobar las IPs
                    pingIPs(archivo, "C:/temp/producto2.txt");
                    // Cerramos el archivo
                    fclose(archivo);
                }
                break;
            case 3:
                // Si se ha podido abrir el archivo en modo escritura y lectura al inicio del archivo
                if (abrirArchivo("C:/temp/adaptador.txt", "wt", &archivo)){
                    // Llamamos a la función para mostrar los adaptadores de red, pasando el archivo como parámetro
                    copiarAdaptadorRed(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                }
                //
                break;
            case 4:
                // Abrimos el archivo
                if (abrirArchivo("C:/temp/adaptador.txt", "wt", &archivo)){
                    // Llamamos a la función para mostrar los adaptadores de red, pasando el archivo como parámetro
                    addAdaptadorRed(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                }
                break;
            case 5:
                // Solicitamos la ruta del archivo a vaciar
                printf("Introduce la ruta del archivo que deseas vaciar: ");
                strcpy(entradaProcesada, entradaSinRetorno(buffer, stdin));
                // Comprobamos si el fichero indicado existe 
                if (!existeArchivo(entradaProcesada) && vaciarEntrada(buffer)){
                    // Si no existe, mostramos un mensaje de error
                    printf("El archivo no existe.\n\n");
                    // Salimos del bucle
                    break;
                }
                // Si el archivo existe
                else {
                    // Vaciamos el archivo
                    vaciarArchivo(entradaProcesada);
                }
                break;
            case 0:
                printf("Saliendo de la aplicación...\n");
                break;
            default:
                printf("Opción inválida. Por favor, seleccionz una opción válida.\n\n");
                break;
        }
    } 
    while (opcion_int != 0);

    return 0;
}



