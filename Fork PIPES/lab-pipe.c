/*************************************************************************************************************
*            Pontificia Universidad Javeriana                                                                *
*                                                                                                            *
* Autor:     Diego Alejandro Melgarejo Bejarano                                                              *
* Fecha:     7 de Octubre, 2025                                                                              *
* Materia:   Sistemas Operativos                                                                             *
* Profesor:  John Jairo Corredor                                                                             *
* Tema:      Función fork(), creación de procesos y comunicación mediante tuberías (PIPE).                   *
* Objetivo:  Demostrar el uso de fork() para la duplicación de procesos y la comunicación entre ellos        *
*            utilizando un pipe unidireccional entre proceso padre e hijo.                                   *
*                                                                                                            *
**************************************************************************************************************
*                                                                                                            *
* HOW TO USE                                                                                                 *
*                                                                                                            *
* HOW TO COMPILE TO PRODUCE EXECUTABLE PROGRAM:                                                              *
*   Linux/macOS:          gcc lab-pipe.c -o lab-pipe                                                         *
*   Windows (MinGW):      gcc lab-pipe.c -o lab-pipe.exe                                                     *
*                                                                                                            *
* HOW TO RUN THE PROGRAM:                                                                                    *
*   Linux/macOS:          ./lab-pipe                                                                         *
*   Windows (CMD):        lab-pipe.exe                                                                       *
*   Windows (PowerShell): .\lab-pipe.exe                                                                     *
*************************************************************************************************************/

/************************************************* Headers **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/************************************************************************************************************
*                                                                                                           *
*  int main();                                                                                              *
*                                                                                                           *
*  Propósito: Mostrar cómo se puede establecer comunicación entre procesos padre e hijo mediante un pipe.   *
*                                                                                                           *
*  Descripción:                                                                                             *
*     - Se crea un pipe (tubería) para permitir la comunicación entre procesos.                             *
*     - El proceso padre escribe un mensaje dentro del pipe.                                                *
*     - El proceso hijo lee el mensaje desde el pipe y lo muestra por pantalla.                             *
*                                                                                                           *
*     pipefd[0] → extremo de lectura del pipe.                                                              *
*     pipefd[1] → extremo de escritura del pipe.                                                            *
*                                                                                                           *
*     La función fork() crea un proceso hijo que hereda los descriptores del pipe del proceso padre,        *
*     permitiendo así la comunicación entre ambos.                                                          *
*                                                                                                           *
*  Retorno:   0 si finaliza correctamente.                                                                  *
*                                                                                                           *
************************************************************************************************************/

int main() {

    /* ------------------------------------------------------------------------------------------------------
       CREACIÓN DEL PIPE
       ------------------------------------------------------------------------------------------------------ */
    int pipefd[2];          // pipefd[0] = lectura, pipefd[1] = escritura
    pid_t procesoHijoPID;   // Variable para almacenar el PID del proceso hijo

    if (pipe(pipefd) == -1) {                        // Crea la tubería
        perror("Error al crear el PIPE");
        exit(EXIT_FAILURE);
    }

    /* ------------------------------------------------------------------------------------------------------
       CREACIÓN DEL PROCESO HIJO MEDIANTE fork()
       ------------------------------------------------------------------------------------------------------ */
    procesoHijoPID = fork();

    if (procesoHijoPID == -1) {                      // Error en fork()
        perror("Error al crear el proceso (FORK)");
        exit(EXIT_FAILURE);
    }

    /* ------------------------------------------------------------------------------------------------------
       BLOQUE DEL PROCESO HIJO
       ------------------------------------------------------------------------------------------------------ */
    if (procesoHijoPID == 0) {
        /* ---- Cierra el extremo de escritura del pipe (solo va a leer) ---- */
        close(pipefd[1]);

        /* ---- Buffer para almacenar el mensaje recibido ---- */
        char mensaje[100];  

        /* ---- Lee desde el extremo de lectura ---- */
        int lecBytes = read(pipefd[0], mensaje, sizeof(mensaje));

        if (lecBytes == -1) {
            perror("Error en la lectura del PIPE");
            exit(EXIT_FAILURE);
        }

        /* ---- Muestra el mensaje recibido ---- */
        printf("\t##==> Proceso HIJO (PID: %d) recibe mensaje del PADRE: %.*s\n", getpid(), lecBytes, mensaje);

        /* ---- Cierra el extremo de lectura ---- */
        close(pipefd[0]);
    } 

    /* ------------------------------------------------------------------------------------------------------
       BLOQUE DEL PROCESO PADRE
       ------------------------------------------------------------------------------------------------------ */
    else {
        /* ---- Cierra el extremo de lectura (solo va a escribir) ---- */
        close(pipefd[0]);

        /* ---- Define el mensaje a enviar ---- */
        char mensaje[] = "HOLA SOY TU PADRE LUCK!!!";

        /* ---- Escribe en el pipe ---- */
        int escBytes = write(pipefd[1], mensaje, strlen(mensaje));

        if (escBytes == -1) {
            perror("Error al escribir en el PIPE");
            exit(EXIT_FAILURE);
        }

        printf("\t##==> Proceso PADRE (PID: %d) envió mensaje al hijo (PID: %d).\n", getpid(), procesoHijoPID);

        /* ---- Cierra el extremo de escritura ---- */
        close(pipefd[1]);
    }

    /* ------------------------------------------------------------------------------------------------------
       MENSAJE FINAL (IMPRESO POR AMBOS PROCESOS)
       ------------------------------------------------------------------------------------------------------ */
    printf("\tFIN del proceso con PID %d...\n", getpid());

    return 0;
}

