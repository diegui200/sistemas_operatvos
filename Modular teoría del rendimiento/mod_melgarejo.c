/**********************************************************************************
*       -----Pontificia Universidad Javeriana-----                                *
*                                                                                 *
*       Estudiante: Diego Melgarejo                                               *
*       Fecha: 23 septiembre                                                      *
*       Materia: Sistemas Operativos                                              *
*       Docente: John Corredor                                                    *
*       Objetivo: teoria de rendimiento                                           *
*                                                                                 *
**********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "modulo.h"

/* Se declaran las variables tipo puntero globales para el programa*/

double *mA, *mB, *mC;

struct timeval ini, fin;

/*funcion para */

void InicioMuestra(){
	gettimeofday(&ini, (void *)0);
}

/**/

void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= ini.tv_usec;
	fin.tv_sec -= ini.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

/*funcion para la impresion de matrices, mientras que la dim sea menor que 8*/

void iniMatriz(int n, double *m1, double *m2){
   	for(int i=0; i<n*n; i++){
	        m1[i] = i*2.3 + 1.3;
	        m2[i] = i*2.3;
	}
};

/*Funcion que imprime las matices*/

void imprMatrices(int n, double *matriz){
    if(n<9){
        printf("\n#######################################################\n");
   	for(int i=0; i<n*n; i++){
                if(i%n==0) printf("\n");
	        printf(" %f ", matriz[i]);
   }
    } else {
        printf("\n#######################################################\n");
    }
};

int main(int argc, char *argv[]) {
    if(argc<=2){
        printf("Numero argumentos incorrectos\n");
        printf("\n\t $ejecutable.exe DIM \n");
        return -1;
    }

    int N = (int) atof(argv[1]);
    if(N<=0){
        printf("\n Valores deben ser mayor que cero\n");
        return -1;
    };

/*se recerva de forma dinamica la memoria para cada matriz*/
    mA = (double *) malloc(N*N*sizeof(double));
    mB = (double *) malloc(N*N*sizeof(double));
    mC = (double *) calloc(N*N,sizeof(double));

	/** Se inicializan las 2 matrices **/
	iniMatriz(N, mA, mB);
	imprMatrices(N, mA);
	imprMatrices(N, mB);

     for(int i=0; i<0; i++){
        for(int j=0; j<N; j++){
            double sumaTemp, *pA, *pB;
            sumaTemp = 0.0;
            pA = mA + i*N;
            pB = mB + j;
            for(int k=0; k<N; k++, pA++, pB+=N){
                sumaTemp += *pA * *pB;
            }
            mC[j+i*N] = sumaTemp;
        }
    }

	imprMatrices(N, mB);

	/*se libera memoria entregando recursos al sistema*/
        printf("\n\tFin del programa.............!\n");
        free(mA);
        free(mB);
        free(mC);
        return 0;

}
