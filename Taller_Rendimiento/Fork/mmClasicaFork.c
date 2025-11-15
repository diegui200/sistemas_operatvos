/************************************************************************************************************/
/*  PROGRAMA: Multiplicación de Matrices con Paralelismo mediante Procesos Fork                             */
/*  AUTOR:    J. Corredor, PhD (documentado por estudiante de Ingeniería de Sistemas – PUJ)                 */
/*  FECHA:    Noviembre 2025                                                                                */
/*                                                                                                          */  
/*  IMPLEMENTACIÓN: funciones.c                                                                             */
/*  Contiene la implementación de las funciones utilizadas en el programa principal.                        */
/************************************************************************************************************/

#include "mmClasicaFork.h"

/* Variables globales para medición de tiempo */
struct timeval inicio, fin;

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: InicioMuestra
 *  Descripción:
 *      Registra el tiempo de inicio de la ejecución para medir el tiempo total del proceso.
 *-----------------------------------------------------------------------------------------*/
void InicioMuestra(){
    gettimeofday(&inicio, (void *)0);
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: FinMuestra
 *  Descripción:
 *      Calcula y muestra el tiempo total transcurrido desde el inicio hasta el final.
 *-----------------------------------------------------------------------------------------*/
void FinMuestra(){
    gettimeofday(&fin, (void *)0);
    fin.tv_usec -= inicio.tv_usec;
    fin.tv_sec  -= inicio.tv_sec;
    double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
    printf("\nTiempo total de ejecución: %9.0f microsegundos\n", tiempo);
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: multiMatrix
 *  Parámetros:
 *      - mA, mB, mC: punteros a las matrices A, B y C.
 *      - D: dimensión de la matriz.
 *      - filaI, filaF: rango de filas que calcula el proceso.
 *  Descripción:
 *      Calcula la multiplicación clásica de matrices en el rango asignado a cada proceso.
 *-----------------------------------------------------------------------------------------*/
void multiMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF) {
    double Suma, *pA, *pB;
    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {
            Suma = 0.0;
            pA = mA + i*D;
            pB = mB + j;
            for (int k = 0; k < D; k++, pA++, pB += D) {
                Suma += *pA * *pB;
            }
            mC[i*D + j] = Suma;
        }
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: impMatrix
 *  Descripción:
 *      Imprime en consola una matriz cuadrada (solo si D < 9 para evitar saturación).
 *-----------------------------------------------------------------------------------------*/
void impMatrix(double *matrix, int D) {
    if (D < 9) {
        printf("\nImpresión de matriz:\n");
        for (int i = 0; i < D*D; i++, matrix++) {
            if (i % D == 0) printf("\n");
            printf(" %.2f ", *matrix);
        }
        printf("\n");
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: iniMatrix
 *  Descripción:
 *      Inicializa las matrices A y B con valores aleatorios en rangos diferentes.
 *-----------------------------------------------------------------------------------------*/
void iniMatrix(double *mA, double *mB, int D){
    for (int i = 0; i < D*D; i++, mA++, mB++){
        *mA = (double)rand() / RAND_MAX * (5.0 - 1.0);
        *mB = (double)rand() / RAND_MAX * (9.0 - 5.0);
    }
}
