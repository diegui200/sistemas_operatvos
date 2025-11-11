/*
 ============================================================================
 Nombre del archivo : listar_archivos_directorio.c
 Autor              : Diego Alejandro Melgarejo Bejarano
 Profesor           : John Corredor
 Universidad        : Pontificia Universidad Javeriana
 Asignatura         : Sistemas Operativos
 Fecha              : 11 / 11 / 2025
 ============================================================================
 Objetivo del programa:
    Implementar un programa en C que permita leer el contenido de un
    directorio ingresado por el usuario y mostrar únicamente los archivos
    regulares presentes en él, junto con su tamaño en bytes.

    Para lograrlo, el programa hace uso de llamadas al sistema relacionadas
    con manejo de directorios (opendir, readdir, closedir) y obtención de
    información de archivos mediante la estructura stat.

    El propósito es afianzar los conceptos de:
      - Manejo de directorios en sistemas basados en UNIX.
      - Construcción de rutas absolutas/relativas.
      - Uso de la función stat() para recuperar información del sistema.
      - Identificación de tipos de archivos (regulares, directorios, etc.).
 ============================================================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main() {

    DIR *d;
    char nomdir[90];
    char nomfich[90];
    struct stat datos;
    struct dirent *direc;

    /*
     ============================================================================
     Entrada de datos:
        Se solicita al usuario escribir el nombre o ruta del directorio que desea
        analizar. Se utiliza fgets para evitar desbordamientos y se limpia el salto 
        de línea final.
     ============================================================================
    */
    printf("Introduzca el Nombre de un Directorio: ");
    fgets(nomdir, sizeof(nomdir), stdin);
    nomdir[strlen(nomdir) - 1] = '\0';  // Eliminación del '\n' generado por fgets

    /*
     ============================================================================
     Apertura del directorio:
        Se intenta abrir el directorio ingresado. Si opendir() retorna NULL,
        significa que el directorio no existe o no se tiene permiso para leerlo.
     ============================================================================
    */
    if ((d = opendir(nomdir)) == NULL) {
        printf("El directorio no existe\n");
        return -1;
    }

    /*
     ============================================================================
     Lectura del contenido del directorio:
        Se recorre cada entrada devuelta por readdir().
        Para cada una, se construye la ruta completa concatenando:
            - el nombre del directorio
            - una barra '/'
            - el nombre del archivo (direc->d_name)

        Luego se llama stat() para obtener información del archivo.
        Se imprime únicamente si es un archivo regular (S_ISREG).
     ============================================================================
    */
    while ((direc = readdir(d)) != NULL) {

        // Construcción de la ruta absoluta del archivo
        strcpy(nomfich, nomdir);
        strcat(nomfich, "/");
        strcat(nomfich, direc->d_name);

        stat(nomfich, &datos); // Información del archivo

        // Verificación: si es archivo regular, se imprime su nombre y tamaño
        if (S_ISREG(datos.st_mode))
            printf("Nombre: %s\t| Tamaño: %ld bytes\n", direc->d_name, datos.st_size);
    }

    /*
     ============================================================================
     Cierre del directorio:
        Se libera el recurso una vez concluida la lectura.
     ============================================================================
    */
    closedir(d);

    return 0;
}
