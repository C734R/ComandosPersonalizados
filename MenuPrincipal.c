#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

// Declaración de funciones
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam);
void insertarFechaHora(FILE *archivo);
void comprobarIPs(FILE *archivoParam);
void adaptadorRed( FILE *archivoParam);
void vaciarArchivo(FILE *archivoParam);

// Insertar la fecha y la hora en el documento de salida: producto2.txt (Especificar en la memoria donde se genera dicho archivo) (Menú punto 1)
void insertarFechaHora(FILE *archivoParam) {
    
    // Obtener la fecha y hora actual
    time_t tiempo = time(NULL);
    // Convertir la fecha y hora a una estructura de tiempo
    struct tm *fechaHora = localtime(&tiempo);
    // Crear un string para almacenar la fecha y hora formateada
    char fechaHoraStr[100];
    // Formatear la fecha y hora
    strftime(fechaHoraStr, sizeof(fechaHoraStr), "%d/%m/%Y %H:%M:%S", fechaHora);
    // Escribir la fecha y hora en el archivo
    fprintf(archivoParam, "Fecha y hora: %s\n", fechaHoraStr);
    // Mostrar la fecha y hora por pantalla
    printf("Fecha y hora: %s\n", fechaHoraStr);
    // Mostrar un mensaje de éxito
    printf("Fecha y hora insertadas en el archivo.\n\n");
}

void vaciarArchivo(FILE *archivoParam){

    // Vaciar el archivo (reducir a 0 bytes)
    ftruncate(fileno(archivoParam), 0);
    // Mostrar un mensaje de éxito
    printf("Archivo vaciado con éxito.\n\n");

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

void comprobarIPs(FILE *archivoParam) {
    
    // Declaración de variables
    char ip[16];
    FILE *archivoIPs;
    char comando[100];
    char lectura[100];
    int totalIPs = 0;
    int numIPs = 0;
    char busqueda [200];
    char ruta[100];
    // Declaramos array para almacenar las IPs que responden al ping
    char ipPositivas[10][16];

    // Pedimos al usuario la ruta del archivo que contiene las IPs
    printf("Introduce la ruta del archivo que contiene las IPs a testear: ");
    scanf("%s", ruta);

    // Abrir el archivo en modo lectura
    archivoIPs = fopen(ruta, "r");

    // Si no se ha podido abrir el archivo
    if (archivoIPs == 0) {
        // Mostrar un mensaje de error 
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", ruta);
        // Salir de la función
        return;
    }
    // Si se ha podido abrir el archivo
    else {
        // Si se pudo abrir el archivo, mostrar un mensaje de éxito
        printf("Archivo abierto con éxito.\n\n");
        // Mostrar un mensaje con las IPs que se van a testear
        printf("--- Testearemos las siguientes IPs ---\n");
        // Mientras haya IPs en el archivo, incrementar el contador de IPs
        while (fscanf(archivoIPs, "%s", ip) != EOF) {
            // Mostrar la IP que vamos a comprobar
            printf("%s\n", ip);
            // Incrementar el contador de IPs
            totalIPs++;
        }
        
        // Volver al inicio del archivo
        rewind(archivoIPs);

        // Leer las IPs del archivo y comprobar si responden al ping
        while (fscanf(archivoIPs, "%s", ip) != EOF) {
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
        fclose(archivoIPs);
    }

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
    // Vaciar el archivo (reducir a 0 bytes)
    if(ftruncate(fileno(archivoParam), 0) != 0){
        // Informar del error
        printf("Error al vaciar el archivo.\n");
        // Liberar la memoria reservada para el contenido
        free(contenido);
        // Salir de la función
        return;
    }
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
    if(fwrite(contenido, 1, (size_t)tam_archivo, archivoParam) != (size_t)tam_archivo){
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

// Diseñar una función cuya funcionalidad que muestre los adaptadores de red de la máquina local. 
// Se mostrará y almacenará en un archivo adaptador.txt la información para un adaptador de red; 
// él cual, se preguntará previamente al usuario (elegido por el usuario) su IP, máscara, y puerta 
// de enlace. El resto de configuraciones de red no se han de mostrar esta información. Para 
// realizar lo anterior la función lanzará un comando de dos que le proporcionará la información 
// que necesita junto con otra que no se considera relevante, y será capaz de extraer y mostrar 
// la que se ha detallado. (menú punto3)*/

void adaptadorRed(FILE *archivoParam) {
    
    // Declaración de variables
    char comando[200];
    char lectura[1024];
    char adaptador [100];
    FILE *consola = NULL;

    // Definimos el comando a ejecutar para obtener los adaptadores de red
    sprintf(comando,"ipconfig | findstr /C:\"Adaptador\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i");
    // Ejecutar el comando definido
    consola = _popen(comando, "r");
    // Mostrar los datos de ipconfig
    while (fgets(lectura, sizeof(lectura), consola) != NULL) {
        printf("%s", lectura);
    }
    // Cerrar la conexión con el comando ipconfig
    _pclose(consola);

    // Pedir el nombre del adaptador de red
    printf("Introduce el nombre del adaptador de red del que quieres guardar su información: ");
    scanf("%s", adaptador);

    // Crear el comando para obtener la información del adaptador de red que coincida con la IP, la máscara y la puerta de enlace introducidas
    sprintf(comando, "ipconfig | findstr /C:\"Adaptador de ethernet %s\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i",adaptador);
    // Ejecutar el comando definido
    consola = _popen(comando, "r");
    // Si no se ha podido ejecutar el comando ipconfig
    if (consola == NULL) {
        // Mostrar un mensaje de error
        printf("Error al ejecutar el comando ipconfig.\n");
    }
    // Si se ha podido ejecutar el comando ipconfig
    else {
        // Mostrar un mensaje de éxito
        printf("Comando ipconfig ejecutado con éxito.\n\n");
        
        printf("--- Datos del adaptador guardado ---\n");

        // Mientras haya respuesta del comando ipconfig
        while (fgets(lectura, sizeof(lectura), consola) != NULL) {

            if (strstr(lectura, "Adaptador") != NULL) {
                // Mostrar la línea
                printf("%s", lectura);
                // Escribir la información en el archivo adaptador.txt
                fprintf(archivoParam, "%s", lectura);
            }

            // Si la línea contiene la palabra "Dirección IP" y coincide con la IP introducida
            if (strstr(lectura, "IPv4") != NULL) {
                // Mostrar la línea
                printf("%s", lectura);
                // Escribir la información en el archivo adaptador.txt
                fprintf(archivoParam, "%s", lectura);
            }
            // Si la línea contiene la palabra "Máscara de subred" y coincide con la máscara de subred introducida
            if (strstr(lectura, "subred") != NULL) {
                // Mostrar la línea
                printf("%s", lectura);
                // Escribir la información en el archivo adaptador.txt
                fprintf(archivoParam, "%s", lectura);
            }
            // Si la línea contiene la palabra "Puerta de enlace predeterminada" y coincide con la puerta de enlace introducida
            if (strstr(lectura, "enlace") != NULL) {
                // Mostrar la línea
                printf("%s", lectura);
                // Escribir la información en el archivo adaptador.txt
                fprintf(archivoParam, "%s", lectura);
                break;
            }
        }
        // Cerrar el archivo
        fclose(archivoParam);
        // Cerrar la conexión con el comando ipconfig
        _pclose(consola);
        // Mostrar un mensaje de éxito
        printf("Información del adaptador de red guardada en adaptador.txt.\n\n");
    }
}

// Función para abrir un archivo de la ruta especificada en el modo especificado y almacenar el puntero al archivo
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam) {

    // Abrir el archivo en modo lectura
    *archivoParam = fopen(rutaParam, modoParam);
    // Si no se ha podido abrir el archivo
    if (archivoParam == 0) {
        // Mostrar un mensaje de error
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", rutaParam);
        // Devolver falso
        return false;
    }
    // Si se ha podido abrir el archivo
    else {
        // Mostrar un mensaje de éxito
        printf("Archivo abierto con éxito.\n");
        // Devolver verdadero
        return true;
    }
}

// Programa principal
int main() {

    // Declaración de variables
    int opcion;
    char ruta[100];
    FILE *archivo = NULL;
    char modo[4];
    // Bucle mientras no se seleccione la opción de salir
    do {
        // Menú principal
        printf("Bienvenid@ al programa para la obtención de DNS con menor ping.\n");
        printf("--- Menú Principal ---\n");
        printf("1. Insertar fecha y hora.\n");
        printf("2. Comprobar IPs.\n");
        printf("3. Mostrar adaptadores de red y registrar seleccionado.\n");
        printf("4. Vaciar archivo.\n");
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
                // Establecemos la ruta y el modo de apertura del archivo
                strcpy(ruta,"C:/temp/producto2.txt");
                strcpy(modo,"rt+");
                // Abrimos el archivo en modo escritura y lectura al inicio del archivo
                if (abrirArchivo(ruta, modo, &archivo)){
                    // Llamamos a la función para insertar la fecha y hora en el archivo
                    insertarFechaHora(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                };               
                break;
            case 2:
                // Establecemos la ruta y el modo de apertura del archivo
                strcpy(ruta,"C:/temp/producto2.txt");
                strcpy(modo,"at+");
                // Abrimos el archivo en modo escritura y lectura al inicio del archivo
                if (abrirArchivo(ruta, modo, &archivo)){
                    // Llamamos a la función para comprobar las IPs
                    comprobarIPs(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                }
                break;
            case 3:
                // Establecemos la ruta y el modo de apertura del archivo
                strcpy(ruta,"C:/temp/adaptador.txt");
                strcpy(modo,"wt");
                // Abrimos el archivo
                if (abrirArchivo(ruta, modo, &archivo)){
                    // Llamamos a la función para mostrar los adaptadores de red, pasando el archivo como parámetro
                    adaptadorRed(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                }
                break;
            case 4:
                // Solicitamos la ruta del archivo a vaciar
                printf("Introduce la ruta del archivo que deseas vaciar: ");
                scanf("%s", ruta);
                strcpy(modo,"rt+");
                // Abrimos el archivo en modo escritura y lectura al inicio del archivo
                if (abrirArchivo(ruta, modo, &archivo)){
                    // Llamamos a la función para eliminar la fecha y hora si ya existen
                    vaciarArchivo(archivo);
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



