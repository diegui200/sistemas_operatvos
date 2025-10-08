/*************************************************************************************************************
*            Pontificia Universidad Javeriana                                                                *
*                                                                                                            *
* Autor:     Diego Melgarejo                                                                                 *
* Fecha:     7 de Octubre, 2025                                                                              *
* Materia:   Sistemas Operativos                                                                             *
* Profesor:  John Jairo Corredor                                                                             *
* Tema:      Función fork(), creación y duplicidad de procesos                                               *
* Objetivo:  Comprender la duplicación de procesos mediante la llamada al sistema fork() y observar cómo     *
*            ambos procesos (padre e hijo) ejecutan el mismo código en paralelo generando resultados         *
*            concurrentes o "caóticos" en su salida.                                                         *
*                                                                                                            *
**************************************************************************************************************
*                                                                                                            *
* HOW TO USE                                                                                                 *
*                                                                                                            *
* HOW TO COMPILE TO PRODUCE EXECUTABLE PROGRAM:                                                              *
*   Linux/macOS:          gcc labo-2.c -o labo2                                                              *
*   Windows (MinGW):      gcc labo-2.c -o labo2.exe                                                          *
*                                                                                                            *
* HOW TO RUN THE PROGRAM:                                                                                    *
*   Linux/macOS:          ./labo2-2                                                                          *
*   Windows (CMD):        labo-2-2.exe                                                                          *
*   Windows (PowerShell): .\labo2.exe                                                                        *
*************************************************************************************************************/

/************************************************* Headers **************************************************/

#include <unistd.h>   // Contiene la declaración de fork() y getpid()
#include <stdio.h>    // Permite el uso de printf() para salida estándar
#include <stdlib.h>   // Contiene funciones generales como exit() y manejo de errores

/************************************************************************************************************
*                                                                                                           *
*  int main();                                                                                              *
*                                                                                                           *
*  Propósito: Demostrar el funcionamiento de la llamada al sistema fork() y cómo ambos procesos (padre e    *
*             hijo) ejecutan el mismo código, generando resultados intercalados según la planificación del  *
*             sistema operativo.                                                                            *
*                                                                                                           *
*  Descripción:                                                                                             *
*     - fork() crea un proceso hijo duplicando el proceso actual.                                           *
*     - Ambos procesos continúan la ejecución desde el mismo punto en el código.                            *
*     - El proceso hijo se identifica con fork() == 0.                                                      *
*     - El proceso padre se identifica con un valor > 0.                                                    *
*     - Ambos procesos ejecutan un bucle que imprime una secuencia de valores, demostrando ejecución         *
*       concurrente.                                                                                        *
*                                                                                                           *
*  Retorno:  0 si el programa finaliza correctamente.                                                       *
*                                                                                                           *
************************************************************************************************************/
int main (){
        /* ---- Mensaje inicial indicando el comienzo del proceso principal ---- */
        printf("#=> Inicio del proceso main o proceso principal <=# \n\n\n");

        /* ---- Se crea el proceso hijo mediante la llamada al sistema fork() ---- */
        int procesoID = fork();

        /* ---- Verificación del tipo de proceso ---- */
        if (procesoID == 0){
                /* Este bloque se ejecuta únicamente en el proceso hijo */
                printf("\t ##==> Proceso hijo recién creado \n");

        } else {
                /* Este bloque se ejecuta únicamente en el proceso padre */
                printf("\t ##==> Proceso padre \n");
        }

        /* ---- Ambos procesos ejecutan este bloque en paralelo ---- */
        printf("Imprimiendo ...... \n");

        /* Bucle que imprime cinco valores para mostrar la ejecución concurrente */
        for(int i = 0; i < 5; i++){
                printf("\t\t == %d == \n", i);
        }

        printf(".... \n");
        return 0;
}
