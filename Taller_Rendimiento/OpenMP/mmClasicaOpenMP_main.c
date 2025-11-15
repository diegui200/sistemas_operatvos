/************************************************************************************************************/
/*  PROGRAMA: Multiplicación de Matrices con Paralelismo mediante OpenMP                                    */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo estudiante de Ingeniería de Sistemas) */
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Este programa realiza la multiplicación de dos matrices cuadradas de tamaño N utilizando                */
/*  paralelismo basado en OpenMP. Cada hilo se encarga de calcular un subconjunto de las                    */
/*  operaciones necesarias, lo cual reduce el tiempo de ejecución en comparación con la versión             */
/*  secuencial.                                                                                             */
/*                                                                                                          */
/*  CARACTERÍSTICAS PRINCIPALES:                                                                            */
/*  - Genera matrices con valores aleatorios.                                                               */
/*  - Paraleliza los bucles de multiplicación usando directivas OpenMP.                                     */
/*  - Permite definir el tamaño de las matrices y el número de hilos desde la línea de comandos.            */
/*  - Incluye funciones para medir el tiempo de ejecución total.                                            */
/*  - Imprime las matrices en pantalla si su tamaño es menor a 9x9.                                         */
/*                                                                                                          */
/************************************************************************************************************/

#include "mmClasicaOpenMP.h"

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN PRINCIPAL
 *-----------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    if(argc < 3){
        printf("\nUso correcto: ./clasicaOpenMP <Tamaño_Matriz> <Número_Hilos>\n\n");
        exit(0);
    }

    int N  = atoi(argv[1]);      // Tamaño de la matriz
    int TH = atoi(argv[2]);      // Número de hilos

    double *matrixA = (double *)calloc(N*N, sizeof(double));
    double *matrixB = (double *)calloc(N*N, sizeof(double));
    double *matrixC = (double *)calloc(N*N, sizeof(double));

    srand(time(NULL));           // Inicialización de valores aleatorios
    omp_set_num_threads(TH);     // Configuración del número de hilos para OpenMP

    iniMatrix(matrixA, matrixB, N);
    impMatrix(matrixA, N);
    impMatrix(matrixB, N);

    /* Inicia medición de tiempo */
    InicioMuestra();

    /* Multiplicación de matrices paralelizada con OpenMP */
    multiMatrix(matrixA, matrixB, matrixC, N);

    /* Finaliza medición de tiempo */
    FinMuestra();

    impMatrix(matrixC, N);

    /* Liberación de memoria */
    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}

/************************************************************************************************************/
/*  USO:                                                                                                    */
/*      $ ./clasicaOpenMP <Tamaño_Matriz> <Número_Hilos>                                                    */
/*                                                                                                          */
/*  EJEMPLO:                                                                                                */
/*      $ ./clasicaOpenMP 4 2                                                                               */
/*                                                                                                          */
/*  SALIDA (para matrices pequeñas):                                                                        */
/*      - Impresión de las matrices A y B generadas aleatoriamente.                                         */
/*      - Matriz C resultante tras la multiplicación.                                                       */
/*      - Tiempo total de ejecución en microsegundos.                                                       */
/*                                                                                                          */
/************************************************************************************************************/
