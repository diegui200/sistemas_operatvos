/************************************************************************************************************/
/*  PROGRAMA: Multiplicación de Matrices con Paralelismo mediante Procesos Fork                             */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo estudiante de Ingeniería de Sistemas) */
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Este programa implementa la multiplicación de dos matrices cuadradas de tamaño N utilizando             */
/*  paralelismo basado en procesos Fork. Cada proceso hijo calcula un subconjunto de filas de la            */
/*  matriz resultado, mejorando la eficiencia computacional en sistemas multiprocesador.                    */
/*                                                                                                          */
/*  CARACTERÍSTICAS PRINCIPALES:                                                                            */
/*  - Inicializa matrices con valores aleatorios.                                                           */
/*  - Divide las filas entre múltiples procesos hijos.                                                      */
/*  - Cada proceso calcula su bloque de la matriz resultado.                                                */
/*  - Utiliza temporización para medir el tiempo total de ejecución.                                        */
/*  - Permite definir el tamaño de la matriz y el número de procesos desde la línea de comandos.            */
/*                                                                                                          */
/************************************************************************************************************/

#include "mmClasicaFork.h"

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN PRINCIPAL
 *-----------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("\nUso correcto: ./nom_ejecutable <Tamaño_Matriz> <Número_Procesos>\n");
        exit(0);
    }

    int N     = atoi(argv[1]);   // Tamaño de la matriz
    int num_P = atoi(argv[2]);   // Número de procesos
    double *matA = (double *) calloc(N*N, sizeof(double));
    double *matB = (double *) calloc(N*N, sizeof(double));
    double *matC = (double *) calloc(N*N, sizeof(double));

    srand(time(0));

    iniMatrix(matA, matB, N);
    impMatrix(matA, N);
    impMatrix(matB, N);

    int rows_per_process = N / num_P;

    /* Inicia medición de tiempo */
    InicioMuestra();

    /* Creación de procesos hijos */
    for (int i = 0; i < num_P; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Proceso hijo
            int start_row = i * rows_per_process;
            int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;

            multiMatrix(matA, matB, matC, N, start_row, end_row);

            if (N < 9) {
                printf("\n[Proceso hijo PID %d] Calculó filas %d a %d:\n", getpid(), start_row, end_row - 1);
                for (int r = start_row; r < end_row; r++) {
                    for (int c = 0; c < N; c++) {
                        printf(" %.2f ", matC[N*r + c]);
                    }
                    printf("\n");
                }
            }
            exit(0); // Finaliza hijo
        } else if (pid < 0) {
            perror("Error al crear proceso con fork()");
            exit(1);
        }
    }

    /* Espera a que terminen todos los procesos hijos */
    for (int i = 0; i < num_P; i++) {
        wait(NULL);
    }

    /* Finaliza medición de tiempo */
    FinMuestra();

    free(matA);
    free(matB);
    free(matC);

    return 0;
}
/************************************************************************************************************/
/*  USO:                                                                                                    */
/*      $ ./multiMatrix <Tamaño_Matriz> <Número_Procesos>                                                   */
/*                                                                                                          */
/*  EJEMPLO:                                                                                                */
/*      $ ./multiMatrix 4 2                                                                                 */
/*                                                                                                          */
/*  SALIDA (para matrices pequeñas):                                                                        */
/*      Impresión de matrices A y B generadas, además de la matriz C calculada por cada proceso.            */
/*      Finalmente, muestra el tiempo total de ejecución en microsegundos.                                  */
/*                                                                                                          */
/************************************************************************************************************/
