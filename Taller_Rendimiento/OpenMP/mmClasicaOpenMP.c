/************************************************************************************************************/
/*  ARCHIVO: mmClasicaOpenMP.c                                                                              */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo estudiante de Ingeniería de Sistemas) */
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Este archivo contiene las declaraciones e implementaciones de las funciones utilizadas en el            */
/*  programa de multiplicación de matrices con OpenMP.                                                      */
/*                                                                                                          */
/*  Incluye:                                                                                                */
/*  - Inicialización y visualización de matrices.                                                           */
/*  - Multiplicación de matrices usando directivas de paralelismo OpenMP.                                   */
/*  - Medición del tiempo de ejecución mediante gettimeofday().                                             */
/*                                                                                                          */
/************************************************************************************************************/

#include "mmClasicaOpenMP.h"

struct timeval inicio, fin;

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: InicioMuestra
 *  DESCRIPCIÓN:
 *      Registra el tiempo inicial antes de iniciar la medición del tiempo de ejecución.
 *-----------------------------------------------------------------------------------------*/
void InicioMuestra() {
    gettimeofday(&inicio, (void *)0);
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: FinMuestra
 *  DESCRIPCIÓN:
 *      Registra el tiempo final, calcula la diferencia con el tiempo inicial y muestra
 *      el tiempo total de ejecución en microsegundos.
 *-----------------------------------------------------------------------------------------*/
void FinMuestra() {
    gettimeofday(&fin, (void *)0);
    fin.tv_usec -= inicio.tv_usec;
    fin.tv_sec  -= inicio.tv_sec;
    double tiempo = (double)(fin.tv_sec * 1000000 + fin.tv_usec);
    printf("Tiempo total de ejecución: %.0f microsegundos\n", tiempo);
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: impMatrix
 *  PARÁMETROS:
 *      - matrix: Puntero a la matriz a imprimir.
 *      - D: Dimensión de la matriz (N x N).
 *  DESCRIPCIÓN:
 *      Imprime el contenido de una matriz si su tamaño es menor a 9x9 para evitar
 *      saturar la salida estándar.
 *-----------------------------------------------------------------------------------------*/
void impMatrix(double *matrix, int D) {
    if (D < 9) {
        printf("\n");
        for (int i = 0; i < D * D; i++) {
            if (i % D == 0) printf("\n");
            printf("%.2f ", matrix[i]);
        }
        printf("\n**-----------------------------**\n");
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: iniMatrix
 *  PARÁMETROS:
 *      - m1, m2: Punteros a las matrices que se inicializarán.
 *      - D: Dimensión de las matrices (N x N).
 *  DESCRIPCIÓN:
 *      Asigna valores aleatorios entre [1, 5) a la matriz A y entre [2, 9) a la matriz B.
 *-----------------------------------------------------------------------------------------*/
void iniMatrix(double *m1, double *m2, int D) {
    for (int i = 0; i < D * D; i++, m1++, m2++) {
        *m1 = (double)rand() / RAND_MAX * (5.0 - 1.0);
        *m2 = (double)rand() / RAND_MAX * (9.0 - 2.0);
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: multiMatrix
 *  PARÁMETROS:
 *      - mA, mB, mC: Punteros a las matrices de entrada (A, B) y resultado (C).
 *      - D: Dimensión de las matrices (N x N).
 *  DESCRIPCIÓN:
 *      Realiza la multiplicación de matrices usando OpenMP para paralelizar los bucles.
 *      Cada hilo calcula un subconjunto de las filas de la matriz resultante.
 *-----------------------------------------------------------------------------------------*/
void multiMatrix(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB;

    #pragma omp parallel private(Suma, pA, pB)
    {
        #pragma omp for
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                pA = mA + i * D;
                pB = mB + j;
                Suma = 0.0;
                for (int k = 0; k < D; k++, pA++, pB += D) {
                    Suma += *pA * *pB;
                }
                mC[i * D + j] = Suma;
            }
        }
    }
}
/************************************************************************************************************/
/*  FIN DEL ARCHIVO: mmClasicaOpenMP.h                                                                      */
/************************************************************************************************************/
