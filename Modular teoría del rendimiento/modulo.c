/* Se incluyen las interfaces necesarias*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* Se declaran las variables tipo puntero globales para el programa*/

struct timeval ini, fin;

void InicioMuestra(){
	gettimeofday(&ini, (void *)0);
}

void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= ini.tv_usec;
	fin.tv_sec -= ini.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

/* Función para inicializar los arreglos m1 y m2 con valores calculados */
void iniMatriz(int n, double *m1, double *m2){
   	for(int i=0; i<n*n; i++){
	        m1[i] = i*2.3 + 1.3;
	        m2[i] = i*2.3;
	}
};

/*funcion que imprime la matriz*/

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
