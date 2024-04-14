#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

char ruta[100];

// Declaración de funciones
void insertarFechaHora(FILE *archivo);
void comprobarIPs();
void mostrarAdaptadores();
void eliminarTextoArchivo(FILE *archivo, char *texto);


// Insertar la fecha y la hora en el documento de salida: producto2.txt (Especificar en la memoria donde se genera dicho archivo) (Menú punto 1)

void insertarFechaHora(FILE *archivo) {

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
}

void eliminarTextoArchivo(FILE *archivo, char *texto){

    // Obtener el tamaño del archivo en base a posición actual
    long int tam_archivo = ftell(archivo);
    // Mover el puntero al inicio del archivo
    rewind(archivo);
    // Reservar memoria para el contenido que vamos a leer del archivo añadiendo 1 byte para el carácter nulo
    char *contenido = (char *)malloc((size_t)(tam_archivo + 1));
    // Si no se pudo reservar memoria, informar del error y salir de la función
    if (contenido == NULL){
        // Informar del error
        printf("Error al reservar memoria para el contenido del archivo.\n");
        // Salir de la función
        return;
    }
    // Leer el contenido del archivo y almacenarlo en la variable contenido
    fread(contenido, 1, (size_t)tam_archivo, archivo);
    // Añadir un carácter nulo al final del contenido, emulando el fin de cadena
    contenido[tam_archivo] = '\0';
    // Reservar memoria para el contenido del archivo sin la cadena a eliminar añaadiendo 1 byte para el carácter nulo
    char *cont_sincadena = (char *)malloc((size_t)(tam_archivo + 1));
    if (cont_sincadena == NULL){
        // Informar del error
        printf("Error al reservar memoria para el contenido del archivo sin cadena.\n");
        // Liberar la memoria reservada para el contenido
        free(contenido);
        // Salir de la función
        return;
    }
    // Inicializar la variable cont_sincadena con una cadena vacía
    cont_sincadena[0] = '\0';

    // Variables para almacenar la posición actual y la posición de la cadena a eliminar
    char *posActual = contenido;
    char *posTexto;
    // Mientras se encuentre la cadena a eliminar en el contenido
    while ((posTexto = strstr(contenido, texto)) != NULL){
        // Volcar el contenido desde la posición actual hasta la posición de la cadena a eliminar 
        strncat(cont_sincadena, posActual, (size_t)(posTexto - posActual));
        // Mover la posición al final de la cadena a eliminar
        posActual = posTexto + strlen(texto);
    }
    // Volcar el contenido restante en la variable cont_sincadena
    strcat(cont_sincadena, posActual);

    // Volver al inicio del archivo
    rewind(archivo);
    // Vaciar el archivo (reducir a 0 bytes)
    ftruncate(fileno(archivo), 0);
    // Escribir el contenido sin la cadena a eliminar en el archivo
    fwrite(cont_sincadena, 1, strlen(cont_sincadena), archivo);
    // Liberar la memoria reservada para el contenido
    free(contenido);
    // Liberar la memoria reservada para el contenido sin la cadena
    free(cont_sincadena);
}



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

void comprobarIPs() {
    
    // Declaración de variables
    char ruta[100];
    char ip[16];
    FILE *archivo;
    char comando[100];
    char lectura[100];
    int totalIPs = 0;
    int numIPs = 0;
    char busqueda [200];
    // Declaramos array para almacenar las IPs que responden al ping
    char ipPositivas[10][16];

    // Pedimos al usuario la ruta del archivo que contiene las IPs
    printf("Introduce la ruta del archivo que contiene las IPs a testear: ");
    scanf("%s", ruta);

    // Abrir el archivo en modo lectura
    archivo = fopen(ruta, "r");

    // Si no se ha podido abrir el archivo
    if (archivo == 0) {
        // Mostrar un mensaje de error 
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", ruta);
        // Salir de la función
        return;
    }
    // Si se ha podido abrir el archivo
    else {
        // Si se pudo abrir el archivo, mostrar un mensaje de éxito
        printf("Archivo abierto con éxito.\n");
        // Mostrar un mensaje con las IPs que se van a testear
        printf("Testearemos las siguientes IPs:\n");
        // Mientras haya IPs en el archivo, incrementar el contador de IPs
        while (fscanf(archivo, "%s", ip) != EOF) {
            // Mostrar la IP que vamos a comprobar
            printf("%s\n", ip);
            // Incrementar el contador de IPs
            totalIPs++;
        }

        // Volver al inicio del archivo
        rewind(archivo);

        // Leer las IPs del archivo y comprobar si responden al ping
        while (fscanf(archivo, "%s", ip) != EOF) {
            // Mostrar la IP que vamos a comprobar
            printf("Testeando IP: %s ...\n", ip);
            // Crear el comando ping, de una sola prueba, con la IP escaneada del archivo
            sprintf(comando, "ping -n 1 %s", ip);
            // Ejecutar el comando ping
            FILE *ping = _popen(comando, "r");
            // Esperar antes de leer la respuesta
            sleep(1);
            // Mientras haya respuesta del comando ping
            while (fgets(lectura, sizeof(lectura), ping) != NULL) {
                // Concatenar la respuesta con la IP
                sprintf(busqueda,"Respuesta desde %s",ip);
                // Comprobar si la IP responde
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
        fclose(archivo);
    }

    // Sustiruir la ruta del archivo e IPs por la ruta del archivo del proucto2.txt
    strcpy(ruta, "C:/temp/producto2.txt");

    // Abrir el archivo en modo lectura y escritura
    archivo = fopen("C:/temp/producto2.txt", "rt+");

    // Comprobar si se pudo abrir el archivo
    if (archivo == 0) {
        // Si no se pudo abrir el archivo, mostrar un mensaje de error y volver
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", ruta);
        return;
    }
    else {
        // Insertamos la fecha y hora en el archivo
        insertarFechaHora(archivo);
        // Añadimos un título al archivo
        fprintf(archivo, "IPs que responden al ping:\n");
        // Recorremos el array de IPs positivas 
        for (int i = 0; i < numIPs; i++) {
            // Y las escribimos en el archivo
            fprintf(archivo, "%s\n", ipPositivas[i]);
        }
        // Cerrar el archivo
        fclose(archivo);
        // Mostrar un mensaje de éxito
        printf("Archivo producto2.txt actualizado con las IPs que han dado respuesta.\n");
    }
}




void mostrarAdaptadores() {
    

}


int main() {

    // Declaración de variables
    int opcion;
    FILE *archivo;
    // Bucle mientras no se seleccione la opción de salir
    do {
        // Menú principal
        printf("Bienvenid@ al programa para la obtención de DNS con menor ping.\n");
        printf("--- Menú Principal ---\n");
        printf("1. Insertar fecha y hora.\n");
        printf("2. Comprobar IPs.\n");
        printf("3. Mostrar adaptadores de red.\n");
        printf("0. Salir del programa.\n");
        printf("Selecciona una opción: ");
        // Mientras la opción no sea un número, seguir pidiendo la opción
        while (scanf("%d", &opcion) != 1) {
            printf("Por favor, pulsa enter y seleccione una opción válida:");
            // Limpiar el buffer de entrada y descartar cualquier carácter que no sea un número
            while (getchar() != '\n');
        }

        switch (opcion) {
            case 1:
                // Especificamos la ruta del archivo
                char ruta[] = "C:/temp/producto2.txt";
                // Abrimos el archivo en modo escritura y lectura al final del archivo
                archivo = fopen(ruta, "at+");
                // Si no se ha podido abrir el archivo
                if (archivo == 0) {
                    // Mostramos un mensaje de error
                    printf("Error al crear o acceder al archivo ubicado en la ruta : %s\n", ruta);
                }
                // Si se ha podido abrir el archivo
                else {                 
                    // Mostramos un mensaje de éxito
                    printf("Archivo creado o accedido con éxito.\n");
                    // Llamamos a la función para insertar la fecha y hora en el archivo
                    insertarFechaHora(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                }
                break;
            case 2:
                comprobarIPs();
                break;
            case 3:
                mostrarAdaptadores();
                break;
            case 4:
                // Especificamos la ruta del archivo
                char ruta[] = "C:/temp/producto2.txt";
                // Abrimos el archivo en modo escritura y lectura al inicio del archivo
                archivo = fopen("C:/temp/producto2.txt", "rt+");
                // Si no se ha podido abrir el archivo
                if (archivo == 0) {
                    // Mostramos un mensaje de error
                    printf("Error al crear o acceder al archivo ubicado en la ruta : %s\n", ruta);
                }
                // Si se ha podido abrir el archivo
                else {
                    // Llamamos a la función para eliminar la fecha y hora si ya existen
                    eliminarTextoArchivo(archivo, "Fecha y hora:");
                    // Cerramos el archivo
                    fclose(archivo);
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
    while (opcion != 0);

    return 0;
}



