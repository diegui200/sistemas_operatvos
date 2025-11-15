/************************************************************************************************************/
/*  PROGRAMA: Multiplicación de Matrices con Paralelismo mediante Procesos Fork                             */
/*  AUTOR:    Carolina Ujueta Ricardo                                                                       */
/*  FECHA:    Noviembre 2025                                                                                */
/*                                                                                                          */
/*  ENCABEZADO: funciones.h                                                                                 */
/*  Contiene las declaraciones de las funciones utilizadas en la multiplicación de matrices                 */
/*  usando paralelismo con procesos Fork.                                                                   */
/*                                                                                                          */
/************************************************************************************************************/

#ifndef MMCLASICAFORK_H
#define MMCLASICAFORK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

/* Funciones de medición de tiempo */
void InicioMuestra();
void FinMuestra();

/* Funciones para manejo de matrices */
void iniMatrix(double *mA, double *mB, int D);
void impMatrix(double *matrix, int D);
void multiMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF);

#endif
