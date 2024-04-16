#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

// Declaración de funciones
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam);
void insertarFechaHora(FILE *archivo);
void comprobarIPs(FILE *archivoParam, char *rutaParam);
void copiarAdaptadorRed( FILE *archivoParam);
void addAdaptadorRed(FILE *archivoParam);
char * entradaMasVaciar (char * destino, int longitud, FILE * fuente);
void vaciarEntrada();
bool validarIP(const char *ip);
bool encontrarAdaptador(char *nAdaptador);
bool mostrarAdaptadores(void);


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

void comprobarIPs(FILE *archivoParam, char *rutaParam) {
    
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

    // Mientras no se haya seleccionado una ruta válida
    while (1){
        // Pedimos al usuario la ruta del archivo que contiene las IPs
        printf("Introduce la ruta del archivo que contiene las IPs a testear: ");
        entradaMasVaciar(ruta, sizeof(ruta), stdin);
        ruta[strlen(ruta) - 1] = '\0';
        // Comprobamos si el fichero indicado existe
        if (fopen(ruta, "r") == NULL){
            // Si no existe, mostramos un mensaje de error
            printf("El archivo no existe. ¿Quieres crearlo e introducir las IP? (s = sí, otro = no) ");
            // Si el usuario no quiere crear el archivo, salir del bucle
            if (getchar() != 's') {
                break;
            }
            // Si el usuario quiere crear el archivo
            else {
                // Lo abrimos en modo escritura para crearlo
                archivoIPs = fopen(ruta, "wt+");
                // Si no se ha podido abrir el archivo
                if (archivoIPs == 0) {
                    // Mostrar un mensaje de error
                    printf("Error al abrir el archivo ubicado en la ruta : %s\n", ruta);
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
                        entradaMasVaciar(ip, sizeof(ip),stdin);
                        // Si el usuario introduce 'fin', salir del bucle
                        if (strcmp(ip, "fin") == 0) {
                            // Salir del bucle
                            break;
                        }
                        // Escribir la IP en el archivo
                        fprintf(archivoIPs, "%s\n", ip);
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
        // Añadir un separador por pantalla 
        printf("-----------------------------------\n\n");

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

            Sleep(1000);
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
    // Vaciar el archivo abriéndolo en modo escritura
    freopen(rutaParam, "w", archivoParam);
    if (archivoParam == 0) {
        // Informar del error
        printf("Error al vaciar el archivo ubicado en la ruta : %s\n", rutaParam);
        // Liberar la memoria reservada para el contenido
        free(contenido);
        // Salir de la función
        return;
    }
    else {
        // Mostrar un mensaje de éxito
        printf("Archivo vaciado con éxito.\n\n");
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
}

// Diseñar una función cuya funcionalidad que muestre los adaptadores de red de la máquina local. 
// Se mostrará y almacenará en un archivo adaptador.txt la información para un adaptador de red; 
// él cual, se preguntará previamente al usuario (elegido por el usuario) su IP, máscara, y puerta 
// de enlace. El resto de configuraciones de red no se han de mostrar esta información. Para 
// realizar lo anterior la función lanzará un comando de dos que le proporcionará la información 
// que necesita junto con otra que no se considera relevante, y será capaz de extraer y mostrar 
// la que se ha detallado. (menú punto3)*/

void copiarAdaptadorRed(FILE *archivoParam) {
    
    // Declaración de variables
    char comando[1024];
    char lectura[1024];
    char adaptador [100];
    bool dentroBloque = false;
    FILE *consola = NULL;
    int i = 0;

    // Si no se han podido mostrar los adaptadores
    if (!mostrarAdaptadores()) {
        return;
    }
    // Si se han podido mostrar
    else {
        // Pedir el nombre del adaptador de red
        printf("Introduce el nombre del adaptador de red del que quieres guardar su información: ");
        // Leer el nombre del adaptador de red
        entradaMasVaciar(adaptador, sizeof(adaptador), stdin);
        // Eliminar el salto de línea
        adaptador[strlen(adaptador) - 1] = '\0';
        // Mostrar un mensaje de éxito
        printf("Adaptador de red introducido: %s\n", adaptador);
        // Si no se ha encontrado el adaptador
        if (!encontrarAdaptador(adaptador)) {
            return;
        }
        // Si se ha podido ejecutar el comando ipconfig
        else {
            // Crear el comando para obtener la información del adaptador de red 
            sprintf(comando, "ipconfig | findstr /C:\"%s\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i", adaptador);
            // Ejecutar el comando definido
            consola = _popen(comando, "r");
            // Inicializar contador
            i = 0;
            // Mientras haya respuesta del comando ipconfig
            while (fgets(lectura, sizeof(lectura), consola) != NULL) {
                // Detectar el inicio de un bloque de adaptador
                if (strstr(lectura, "Adaptador de Ethernet") != NULL || strstr(lectura, "Adaptador de LAN inal") != NULL){
                    // Verificar si es el bloque del adaptador solicitado
                    if (strstr(lectura, adaptador) != NULL) {
                        // Mostrar encabezado
                        printf("--- Datos del adaptador guardado ---\n");
                        // Establecer que estamos dentro del bloque del adaptador
                        dentroBloque = true;
                        // Añadir un separador en el archivo
                        fprintf(archivoParam, "-----------------------------------\n");
                        // Añadir un título en el archivo
                        fprintf(archivoParam, "--- Datos del adaptador guardado ---\n");
                        // Escribir la información en el archivo adaptador.txt
                        fprintf(archivoParam, "%s", lectura); 
                        // Mostrar la línea por pantalla
                        printf("%s", lectura); 
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
                    if (strstr(lectura, "IPv4") != NULL || strstr(lectura, "subred") != NULL || strstr(lectura, "enlace") != NULL) {
                        // Escribir la información en el archivo adaptador.txt
                        fprintf(archivoParam, "%s", lectura);
                        // Mostrar la línea
                        printf("%s", lectura);
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
    char comando[1024];
    char lectura[1024];
    char respuesta [100];
    bool bAdaptador = false;
    FILE *consola = NULL;

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
            // Vaciar el buffer de entrada
            vaciarEntrada();
            // Leer el nombre del adaptador de red
            fgets(respuesta, 100, stdin);
            // Eliminar el salto de línea
            respuesta[strlen(respuesta) - 1] = '\0';
            // Mostrar un mensaje de éxito
            printf("Adaptador de red introducido: %s\n", respuesta);
            // Si se encuentra el adaptador
            if(encontrarAdaptador(respuesta)){
                // Mostrar un mensaje de error
                printf("El adaptador ya existe. Introduce otro nombre.\n\n");
                // Registramos que el adaptador ya existe
                bAdaptador = false;
            }
            else {
                // Registramos que no existe el adaptador
                bAdaptador = true;
            }   
        }
    }
    // Si no existe el adaptador
    if (bAdaptador){
        // Añadir separador en el archivo
        fprintf(archivoParm, "-----------------------------------\n\n");
        // Registrar el nombre del adaptador en el archivo adaptador.txt
        fprintf(archivoParm, "Adaptador de Red %s\n", respuesta);
        // Añadir un separador en el archivo
        fprintf(archivoParm, "-----------------------------------\n\n");
        // Mostrar un mensaje de éxito
        printf("Nombre del adaptador registrado con éxito.\n\n");

        // Mientras los carácteres introducidos no cumplan con el formato de una IP
        while (1){
            // Solicitar la IP, la máscara y la puerta de enlace
            printf("Introduce la IP: ");
            // Vaciar el buffer de entrada
            vaciarEntrada();
            fgets(respuesta, 100, stdin);
            // Comprobar si la IP introducida es válida
            respuesta[strlen(respuesta) - 1] = '\0';
            if (validarIP(respuesta)){
                // Mostrar un mensaje de error
                printf("La IP introducida no es válida. Introduce una IP válida.\n");
            }
            // Si la IP es válida, salir del bucle
            else {
                // Registrar la IP en el archivo adaptador.txt
                fprintf(archivoParm, "Dirección IPv4: %s\n", respuesta);
                break;
            }
        }
        while (1){
            // Solicitar la máscara
            printf("Introduce la máscara de subred: ");
            // Vaciar el buffer de entrada
            vaciarEntrada();
            fgets(respuesta, 100, stdin);
            // Comprobar si la máscara introducida es válida
            respuesta[strlen(respuesta) - 1] = '\0';
            if (validarIP(respuesta)){
                // Mostrar un mensaje de error
                printf("La máscara introducida no es válida. Introduce una máscara válida.\n");
            }
            // Si la máscara es válida, salir del bucle
            else {
                // Registrar la máscara en el archivo adaptador.txt
                fprintf(archivoParm, "Máscara: %s\n", respuesta);
                break;
            }
        }
        while (1){
            // Solicitar la puerta de enlace
            printf("Introduce la puerta de enlace: ");
            // Vaciar el buffer de entrada
            vaciarEntrada();
            fgets(respuesta, 100, stdin);
            // Comprobar si la puerta de enlace introducida es válida
            respuesta[strlen(respuesta) - 1] = '\0';
            if (validarIP(respuesta)){
                // Mostrar un mensaje de error
                printf("La puerta de enlace introducida no es válida. Introduce una puerta de enlace válida.\n");
            }
            // Si la puerta de enlace es válida, salir del bucle
            else {
                // Registrar la puerta de enlace en el archivo adaptador.txt
                fprintf(archivoParm, "Puerta de enlace: %s\n", respuesta);
                break;
            }
        }
        // Añadir un separador en el archivo
        fprintf(archivoParm, "-----------------------------------\n\n");
        // Volver al inicio del archivo
        rewind(archivoParm);
        // Mostar el contenido del archivo adaptador.txt
        while (fgets(lectura, sizeof(lectura), archivoParm) != NULL) {
            // Mostrar el contenido del archivo
            printf("%s", lectura);
        }
        // Mostrar un mensaje de éxito
        printf("Se han registrado con éxito los datos del adaptador introducidos.\n\n");
        // Cerrar el archivo
        fclose(archivoParm);
    }
}

bool mostrarAdaptadores(void){
    // Declaración de variables
    char comando[1024];
    char lectura[1024];
    FILE *consola = NULL;

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
        while (fgets(lectura, sizeof(lectura), consola) != NULL) {
            printf("%s", lectura);
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
    // Declaración de variables
    char comando[1024];
    char lectura[1024];
    FILE *consola = NULL;

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
        while (fgets(lectura, sizeof(lectura), consola) != NULL) {
            // Detectar el inicio de un bloque de adaptador
            if (strstr(lectura, "Adaptador de Ethernet") != NULL || strstr(lectura, "Adaptador de LAN inal") != NULL){
                // Verificar si encontramos el adaptador solicitado
                if (strstr(lectura, nAdaptador) != NULL) {
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



// Función para abrir un archivo de la ruta especificada en el modo especificado y almacenar el puntero al archivo
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam) {

    // Abrir el archivo en modo lectura
    *archivoParam = fopen(rutaParam, modoParam);
    // Si no se ha podido abrir el archivo
    if (*archivoParam == 0) {
        // Mostrar un mensaje de error
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", rutaParam);
        // Devolver falso
        return false;
    }
    // Si se ha podido abrir el archivo
    else {
        // Mostrar un mensaje de éxito
        printf("Archivo abierto con éxito.\n\n");
        // Devolver verdadero
        return true;
    }
}

// Función para vaciar el buffer de entrada y leerlo
char * entradaMasVaciar (char * destino, int longitud, FILE * fuente) {
    // Vaciar el buffer de entrada
    vaciarEntrada();
    // Leer la entrada
    return fgets(destino, longitud, fuente);
}

// Función para vaciar el buffer de entrada
void vaciarEntrada () {
    int c;
    do{
        c = getchar();
    } 
    while (c != EOF && c != '\n');
}

// Función para validar una dirección IP
bool validarIP(const char *ip) {
    // Declaración de variables
    int num, puntos = 0;
    // Declarar puntero a la IP
    const char *ip_puntero;
    // Inicializar el puntero al final de la IP
    ip_puntero += strlen(ip) - 1;

    // Comprobar si la IP es nula
    if (ip == NULL) {
        // Si es nula, la IP no es válida
        return false;
    }

    // Mientras no se haya llegado al inicio de la IP
    while (ip_puntero >= ip) {
        // Comprobar si el carácter actual es un punto
        if (*ip_puntero == '.') {
            // Si el primer caracter es un punto, hay más de 3 puntos o la IP es un punto
            if (ip_puntero == ip || puntos == 3 || *(ip_puntero + 1) == '.'){
                // Si es igual, la IP no es válida
                return false;  
            }
            // Incrementar el número de puntos contados
            puntos++;
        } 
        // Si el carácter actual no es la primera de las 3 cifras antes del punto y no es un número entre 0 y 9
        else if (ip_puntero != ip && (*ip_puntero < '0' || *ip_puntero > '9')) { 
            // La IP no es válida
            return false;
        }
        // Incrementar el puntero a la IP
        ip_puntero++;        
    }
    // Si no hay 3 puntos
    if (puntos != 3) {  
        // La IP no es válida
        return false;
    }

    // Si el último carácter es un punto
    if (*(ip_puntero - 1) == '.') {
        // La IP no es válida
        return false;  
    }

    // Para cada uno de las 4 cifras de la IP    
    for (int i = 0; i < 4; i++) {
        // Convertir la cifra en carácteres a un número
        num = atoi(ip);
        // Comprobar si el número está entre 0 y 255
        if (num < 0 || num > 255) {
            // Si no está en el rango, la IP no es válida
            return false;  
        }
        // Mover la IP a la siguiente cifra
        ip = strchr(ip, '.');
        // Si la ip no es nula
        if (ip != NULL) {
            // Mover la IP al siguiente octeto
            ip++;  
        }
    }
    // Si todos los octetos son válidos, la IP es válida
    return true;
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
        printf("Bienvenid@ al programa.\n");
        printf("--- Menú Principal ---\n");
        printf("1. Insertar fecha y hora.\n");
        printf("2. Comprobar IPs.\n");
        printf("3. Mostrar adaptadores de red y registrar seleccionado.\n");
        printf("4. Mostrar adaptadores de red y añadir uno.\n");
        printf("5. Vaciar archivo.\n");
        printf("0. Salir del programa.\n");
        printf("Selecciona una opción: ");
        // Mientras la opción no sea un número, seguir pidiendo la opción
        while (scanf("%d", &opcion) != 1) {
            // Registrar opción inválida
            opcion = -1;
            // Limpiar el buffer de entrada 
            vaciarEntrada();
            break;
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
                    comprobarIPs(archivo, ruta);
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
                    copiarAdaptadorRed(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                }
                break;
            case 4:
                // Establecemos la ruta y el modo de apertura del archivo
                strcpy(ruta,"C:/temp/adaptador.txt");
                strcpy(modo,"at+");
                // Abrimos el archivo
                if (abrirArchivo(ruta, modo, &archivo)){
                    // Llamamos a la función para mostrar los adaptadores de red, pasando el archivo como parámetro
                    addAdaptadorRed(archivo);
                    // Cerramos el archivo
                    fclose(archivo);
                }
                break;
            case 5:
                // Solicitamos la ruta del archivo a vaciar
                printf("Introduce la ruta del archivo que deseas vaciar: ");
                entradaMasVaciar(ruta,sizeof(ruta),stdin);
                strcpy(modo,"wt");
                // Comprobamos si el fichero indicado existe 
                if (fopen(ruta, "r") == NULL){
                    // Si no existe, mostramos un mensaje de error
                    printf("El archivo no existe.\n\n");
                    // Salimos del bucle
                    break;
                }
                // Si el archivo existe
                else {
                    // Si existe, mostramos un mensaje de éxito
                    printf("Archivo encontrado.\n\n");
                    // Abrimos el archivo en modo escritura
                    if (abrirArchivo(ruta, modo, &archivo)){
                    // Confirmamos que se ha vaciado el archivo
                    printf("Archivo vaciado con éxito.\n\n");
                    // Cerramos el archivo
                    fclose(archivo);
                    }
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



