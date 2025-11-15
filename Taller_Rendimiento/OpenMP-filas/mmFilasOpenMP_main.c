/************************************************************************************************************/
/*  PROGRAMA: Multiplicación de Matrices con Transposición (Filas x Filas)                                  */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo, estudiante de Ingeniería de Sistemas)*/
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Este programa realiza la multiplicación de dos matrices cuadradas utilizando el método de               */
/*  transposición para optimizar la localidad espacial en memoria.                                          */
/*  Se implementa paralelismo con OpenMP para mejorar el rendimiento mediante múltiples hilos.              */
/*                                                                                                          */
/*  CARACTERÍSTICAS PRINCIPALES:                                                                            */
/*  - Inicializa matrices A y B con valores aleatorios.                                                     */
/*  - Transpone la segunda matriz para mejorar la eficiencia de cache.                                      */
/*  - Ejecuta la multiplicación usando OpenMP con hilos configurables.                                      */
/*  - Mide el tiempo total de ejecución en microsegundos.                                                   */
/*  - Imprime matrices pequeñas para verificación visual.                                                   */
/************************************************************************************************************/

#include "mmFilasOpenMP.h"

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN PRINCIPAL
 *-----------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("\nUso: $ ./mmTransOpenMP <Tamaño_Matriz> <Número_Hilos>\n\n");
        exit(0);
    }

    int N  = atoi(argv[1]);
    int TH = atoi(argv[2]);

    double *matrixA = (double *)calloc(N * N, sizeof(double));
    double *matrixB = (double *)calloc(N * N, sizeof(double));
    double *matrixC = (double *)calloc(N * N, sizeof(double));

    srand(time(NULL));

    omp_set_num_threads(TH);

    iniMatrix(matrixA, matrixB, N);

    impMatrix(matrixA, N, 0);
    impMatrix(matrixB, N, 1);

    InicioMuestra();
    multiMatrixTrans(matrixA, matrixB, matrixC, N);
    FinMuestra();

    impMatrix(matrixC, N, 0);

    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}

/************************************************************************************************************/
/*  USO:                                                                                                    */
/*      $ ./mmTransOpenMP <Tamaño_Matriz> <Número_Hilos>                                                    */
/*                                                                                                          */
/*  EJEMPLO:                                                                                                */
/*      $ ./mmTransOpenMP 4 2                                                                               */
/*                                                                                                          */
/*  SALIDA (para matrices pequeñas):                                                                        */
/*      Muestra matrices A, B (transpuesta) y C, además del tiempo total de ejecución.                      */
/************************************************************************************************************/
