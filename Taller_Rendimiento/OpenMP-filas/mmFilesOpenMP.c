/************************************************************************************************************/
/*  ARCHIVO: funcionesTrans.c                                                                               */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo, estudiante de Ingeniería de Sistemas)*/
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Implementación de funciones auxiliares para la multiplicación de matrices                               */
/*  mediante transposición (filas x filas) con OpenMP.                                                      */
/************************************************************************************************************/

#include "mmFilasOpenMP.h"

struct timeval inicio, fin;

/*------------------------------------------------------------------------------------------
 *  FUNCIONES DE TIEMPO
 *-----------------------------------------------------------------------------------------*/
void InicioMuestra() {
    gettimeofday(&inicio, (void *)0);
}

void FinMuestra() {
    gettimeofday(&fin, (void *)0);
    fin.tv_usec -= inicio.tv_usec;
    fin.tv_sec  -= inicio.tv_sec;
    double tiempo = (double)(fin.tv_sec * 1000000 + fin.tv_usec);
    printf("\nTiempo total de ejecución: %.6f segundos\n", tiempo / 1e6);
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: iniMatrix()
 *-----------------------------------------------------------------------------------------*/
void iniMatrix(double *m1, double *m2, int D) {
    for (int i = 0; i < D * D; i++, m1++, m2++) {
        *m1 = (double)rand() / RAND_MAX * (5.0 - 1.0);
        *m2 = (double)rand() / RAND_MAX * (9.0 - 5.0);
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: impMatrix()
 *-----------------------------------------------------------------------------------------*/
void impMatrix(double *matrix, int D, int t) {
    int aux = 0;
    if (D < 6) {
        switch (t) {
            case 0: // Matriz normal
                for (int i = 0; i < D * D; i++) {
                    if (i % D == 0) printf("\n");
                    printf("%.2f ", matrix[i]);
                }
                printf("\n  - \n");
                break;

            case 1: // Matriz transpuesta
                while (aux < D) {
                    for (int i = aux; i < D * D; i += D)
                        printf("%.2f ", matrix[i]);
                    aux++;
                    printf("\n");
                }
                printf("\n  - \n");
                break;

            default:
                printf("Sin tipo de impresión\n");
        }
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: multiMatrixTrans()
 *-----------------------------------------------------------------------------------------*/
void multiMatrixTrans(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB;

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                pA = mA + i * D;
                pB = mB + j * D;
                Suma = 0.0;

                for (int k = 0; k < D; k++, pA++, pB++) {
                    Suma += *pA * *pB;
                }

                mC[i * D + j] = Suma;
            }
        }
    }
}

/************************************************************************************************************/
/*  FIN DEL ARCHIVO: mmFilasOpenMP.c                                                                        */
/************************************************************************************************************/
