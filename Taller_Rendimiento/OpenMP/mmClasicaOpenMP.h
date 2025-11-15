/************************************************************************************************************/
/*  ARCHIVO: mmClasicaOpenMP.h                                                                              */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo, estudiante de Ingeniería de Sistemas)*/
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Archivo de encabezado que declara las funciones y estructuras necesarias para la                        */
/*  implementación del programa de multiplicación de matrices usando OpenMP.                                */
/*                                                                                                          */
/*  Incluye:                                                                                                */
/*  - Declaración de funciones de inicialización, impresión y multiplicación de matrices.                   */
/*  - Declaración de funciones para medición del tiempo de ejecución.                                       */
/*  - Inclusión de librerías estándar y OpenMP.                                                             */
/*                                                                                                          */
/************************************************************************************************************/

#ifndef MMCLASICAOPENMP_H
#define MMCLASICAOPENMP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

/*------------------------------------------------------------------------------------------
 *  PROTOTIPOS DE FUNCIONES
 *-----------------------------------------------------------------------------------------*/
void InicioMuestra();
void FinMuestra();
void impMatrix(double *matrix, int D);
void iniMatrix(double *m1, double *m2, int D);
void multiMatrix(double *mA, double *mB, double *mC, int D);

#endif
