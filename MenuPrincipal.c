#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_IP_LENGTH 16
#define MAX_FILENAME_LENGTH 100
#define MAX_MACHINES 100

// Insertar la fecha y la hora en el documento de salida: producto2.txt (Especificar en la memoria donde se genera dicho archivo) (Menú punto 1)

void insertarFechaHora() {

    // Declaramos un puntero de tipo FILE para trabajar con archivos
    FILE *archivo;
    // Declarar la ruta del archivo
    char ruta[100];
    strcpy(ruta, "C:/TEMP/producto2.txt");
    // Ruta donde se guardará el archivo
    archivo = fopen(ruta, "at+");
    // Comprobar si se pudo abrir el archivo
    if (archivo == 0) {
        // Si no se pudo abrir el archivo, mostrar un mensaje de error y salir de la función
        printf("Error al crear o acceder al archivo ubicado en la ruta : %s\n", ruta);
        return;
    }
    else {

        // Si se pudo abrir el archivo, mostrar un mensaje de éxito
        printf("Archivo creado o accedido con éxito.\n");
        // Mover el puntero al principio del archivo
        rewind(archivo);
        char texto[100];
        // Mientras no encuentre el texto "Fecha y hora:"
        do{
            long *puntero;
            // Leer una línea del archivo
            fgets(texto, sizeof(texto), archivo);
            puntero = strstr(texto, "Fecha y hora:");
            // Comprobar si la línea contiene el texto "Fecha y hora:"
            if (puntero != NULL) {
                // Mover el puntero al inicio de la línea que contiene el texto "Fecha y hora:"
                fseek(archivo, (long)puntero, SEEK_SET);
                // Borrar el contenido de la línea
                fprintf(archivo, "PruebaOK.\n");
                fflush(archivo);
                break;
            }
        } 
        while (!feof(archivo));
        //else {
        //    // Mover el puntero al final del archivo
        //    fseek(archivo, 0, SEEK_END);
        //}
        // Obtener la fecha y hora actual
        time_t tiempo = time(NULL);
        // Convertir la fecha y hora a una estructura de tiempo
        struct tm *fechaHora = localtime(&tiempo);
        // Crear un string para almacenar la fecha y hora formateada
        char fechaHoraStr[100];
        // Formatear la fecha y hora
        strftime(fechaHoraStr, sizeof(fechaHoraStr), "%d/%m/%Y %H:%M:%S", fechaHora);
        
        // Escribir la fecha y hora en el archivo
        fprintf(archivo, "Fecha y hora: %s\n", fechaHoraStr);
        // Cerrar el archivo
        fclose(archivo);
        }

}


// Comprobar e informar por pantalla sobre cuáles de una lista de máquinas referenciadas por su IP escrita en un archivo de texto, contestan positivamente a un ping lanzado por la aplicación que estás programando. Se recomienda generar un submenú donde se solicite el nombre y ubicación de dicho archivo.
// Diseñar una función que lea el contenido de un archivo de texto que consiste en una serie de direcciones IP cada una de las cuales ocupa una línea de éste. La ruta y nombre del archivo de texto se preguntarán al usuario e inicialmente se mostrará el contenido de este por pantalla. Finalmente lanzará un ping para cada una de las IP’s y determinará e informará sobre cuáles de las anteriores responden afirmativamente a este comando. (Menú punto 2)
// Se almacenará el resultado del paso anterior debajo de la fecha y la hora en el archivo producto2.txt, las IPs que han dado una respuesta positiva. Guardar el archivo y no borrar. (Menú punto 2)

void comprobarIPs() {
    
}

// char pedirNombreRuta() {

//     char ruta[MAX_FILENAME_LENGTH];
//     printf("Introduce la ruta del archivo que contiene las IPs a testear: ");
//     ruta = scanf("%s", &ruta);
//     return ruta;
    

// }

void mostrarAdaptadores() {
    

}


int main() {

    // Declaración de variables
    int opcion;

    // Bucle mientras no se seleccione la opción de salir
    do {
        // Menú principal
        printf("Bienvenid@ al programa para la obtención de DNS con menor ping.\n");
        printf("--- Menú Principal ---\n");
        printf("1. Insertar fecha y hora.\n");
        printf("2. Comprobar IPs.\n");
        printf("3. Mostrar adaptadores de red.\n");
        printf("0. Salir del programa.\n");
        printf("Seleccione una opción: ");
        // Mientras la opción no sea un número, seguir pidiendo la opción
        while (scanf("%d", &opcion) != 1) {
            printf("Por favor, pulsa enter y seleccione una opción válida:");
            // Limpiar el buffer de entrada y descartar cualquier carácter que no sea un número
            while (getchar() != '\n');
        }

        switch (opcion) {
            case 1:
                insertarFechaHora();
                break;
            case 2:
                comprobarIPs();
                break;
            case 3:
                mostrarAdaptadores();
                break;
            case 0:
                printf("Saliendo de la aplicación...\n");
                break;
            default:
                printf("Opción inválida. Por favor, seleccione una opción válida.\n\n");
                break;
        }
    } 
    while (opcion != 0);

    return 0;
}



