/*
 ============================================================================
 Nombre del archivo : permisos_y_fechas_directorio.c
 Autor              : Diego Alejando Melgarejo Bejarano
 Profesor           : John Corredor
 Universidad        : Pontificia Universidad Javeriana
 Asignatura         : Sistemas Operativos
 Fecha              : 11 / 11 / 2025
 ============================================================================
 Objetivo del programa:
    Implementar un programa en C que permita analizar los archivos contenidos
    en un directorio ingresado por el usuario. El programa deberá:

       Mostrar la ruta completa de cada archivo.
       Mostrar el modo (permisos) del archivo.
       Verificar si el propietario tiene permiso de lectura.
       Identificar si el elemento es archivo regular o directorio.
       Para archivos regulares: mostrar información si su última
        modificación ocurrió en los últimos 10 días.

    El propósito del ejercicio es reforzar el manejo de:
      - Lectura de directorios mediante opendir(), readdir().
      - Uso de la estructura stat para consultar atributos del archivo.
      - Comprobación de permisos mediante máscaras de bits.
      - Validación de tiempos con time(), ctime(), st_mtime.
 ============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {

    int er;                     // Variable para almacenar el retorno de stat()
    char nomdir[100];           // Nombre del directorio ingresado por el usuario
    char nomfich[100];          // Ruta completa del archivo encontrado
    struct stat atr;            // Estructura con información del archivo
    DIR *d;                     // Puntero para abrir el directorio
    struct dirent *rd1;         // Estructura de cada entrada del directorio
    time_t fecha;               // Tiempo actual

    /*
     ============================================================================
     Entrada del usuario: se solicita el nombre del directorio
     Se usa fgets para evitar desbordamientos.
     ============================================================================
    */
    printf("Nombre directorio:\n");
    fgets(nomdir, sizeof(nomdir), stdin);

    // Eliminar el salto de línea final agregado por fgets
    nomdir[strlen(nomdir) - 1] = '\0';

    // Obtener el tiempo actual
    fecha = time(&fecha);

    /*
     ============================================================================
     Intento de apertura del directorio.
     Si opendir() devuelve NULL, el directorio no existe o no hay permisos.
     ============================================================================
    */
    if ((d = opendir(nomdir)) == NULL) {
        printf("No existe ese directorio\n");
        return -1;
    } else {

        /*
         ============================================================================
         Lectura del directorio:
         Se recorren todas las entradas con readdir().
         Se descartan "." y ".." ya que no representan archivos reales.
         ============================================================================
        */
        while ((rd1 = readdir(d)) != NULL) {

            if ((strcmp(rd1->d_name, ".") != 0) && (strcmp(rd1->d_name, "..") != 0)) {

                // Construcción de la ruta completa del archivo
                strcpy(nomfich, nomdir);
                strcat(nomfich, "/");
                strcat(nomfich, rd1->d_name);

                printf("Fichero: %s\n", nomfich);

                // Obtener información del archivo mediante stat()
                er = stat(nomfich, &atr);

                printf("Modo: %#o\n", atr.st_mode);

                /*
                 ============================================================================
                 Verificación del permiso R (lectura) para el propietario.
                 Se evalúa la máscara 0400 que corresponde al bit de lectura del usuario.
                 ============================================================================
                */
                if ((atr.st_mode & 0400) != 0)
                    printf("Permiso R para propietario\n");
                else
                    printf("No permiso R para propietario\n");

                /*
                 ============================================================================
                 Identificar tipo del archivo usando macros S_ISDIR y S_ISREG
                 ============================================================================
                */
                if (S_ISDIR(atr.st_mode))
                    printf("Es un directorio\n");

                if (S_ISREG(atr.st_mode)) {

                    /*
                     ============================================================================
                     Comprobar si el archivo fue modificado en los últimos 10 días.
                     10 días = 10 * 24 horas * 60 minutos * 60 segundos
                     ============================================================================
                    */
                    if ((fecha - 10 * 24 * 60 * 60) < atr.st_mtime) {

                        printf("FICHERO: %s\n", rd1->d_name);
                        printf("Fecha de modificación: %s", ctime(&atr.st_mtime));
                        printf("Tiempo en segundos: %ld\n", atr.st_mtime);
                    }
                }

                printf("\n");
            }
        }

        /*
         ============================================================================
         Cierre del directorio luego de finalizar la lectura.
         ============================================================================
        */
        closedir(d);
    }

    return 0;
}
