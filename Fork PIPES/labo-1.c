/*************************************************************************************************************
*            Pontificia Universidad Javeriana                                                                *
*                                                                                                            *
* Autor:     Diego Melgarejo                                                                                 *
* Fecha:     7 de Octubre, 2025                                                                              *
* Materia:   Sistemas Operativos                                                                             *
* Profesor:  John Jairo Corredor                                                                             *
* Tema:      Función fork(), creación y duplicidad de procesos                                               *
* Objetivo:  Comprender el funcionamiento de la llamada al sistema fork() y cómo el sistema operativo crea   *
*            y ejecuta procesos independientes (padre e hijo) de forma concurrente.                          *
*                                                                                                            *
**************************************************************************************************************
*                                                                                                            *
* HOW TO USE                                                                                                 *
*                                                                                                            *
* HOW TO COMPILE TO PRODUCE EXECUTABLE PROGRAM:                                                              *
*   Linux/macOS:          gcc labo-1.c -o labo                                                                 *
*   Windows (MinGW):      gcc labo-1.c -o labo.exe                                                             *
*                                                                                                            *
* HOW TO RUN THE PROGRAM:                                                                                    *
*   Linux/macOS:          ./labo-1                                                                             *
*   Windows (CMD):        labo-1.exe                                                                           *
*   Windows (PowerShell): .\labo-1.exe                                                                         *
*************************************************************************************************************/

/************************************************* Headers **************************************************/

#include <unistd.h>   // Proporciona funciones del sistema UNIX como fork() y getpid()
#include <stdio.h>    // Proporciona la función printf() para salida estándar
#include <stdlib.h>   // Contiene utilidades generales del sistema, como exit()

/************************************************************************************************************
*                                                                                                           *
*  int main();                                                                                              *
*                                                                                                           *
*  Propósito: Demostrar el funcionamiento básico de la llamada al sistema fork() para crear un proceso hijo. *
*                                                                                                           *
*  Descripción:                                                                                             *
*     - fork() crea un proceso hijo duplicando el proceso actual.                                           *
*     - El proceso padre recibe un valor positivo (PID del hijo).                                           *
*     - El proceso hijo recibe un valor igual a cero.                                                       *
*     - Si ocurre un error, fork() devuelve un valor negativo.                                              *
*     - Ambos procesos continúan ejecutando el mismo código desde la línea siguiente a la llamada fork().   *
*                                                                                                           *
*  Retorno:  0 si el programa finaliza correctamente.                                                       *
*                                                                                                           *
************************************************************************************************************/
int main (){
        /* Se crea la llamada al sistema fork(), la cual genera un nuevo proceso */
        int procesoID = fork();

        /* ---- Verificación del tipo de proceso ---- */
        if (procesoID > 0){
                /* Este bloque se ejecuta únicamente en el proceso padre */
                printf("\t ##==> Proceso Padre con ID %d \n", getpid());

        } else {
                /* Este bloque se ejecuta únicamente en el proceso hijo */
                printf("\t ##==> Proceso Hijo con ID %d \n", getpid());
        }

        /* Ambos procesos ejecutan el siguiente código, pues se copian tras el fork() */
        printf(" A partir de aqui es el proceso main o proceso principal\n");

        return 0;
}
