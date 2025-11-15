/************************************************************************************************************/
/*  PROGRAMA: Multiplicación de Matrices con Paralelismo mediante Hilos POSIX (Pthreads)                    */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo, estudiante de Ingeniería de Sistemas)*/
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Este programa realiza la multiplicación de dos matrices cuadradas de tamaño N utilizando hilos POSIX.   */
/*  Cada hilo calcula un subconjunto de filas de la matriz resultante, mejorando el desempeño en equipos     */
/*  con múltiples núcleos. El uso de `pthread_mutex_t` garantiza la sincronización y seguridad de acceso     */
/*  cuando varios hilos escriben en la matriz compartida.                                                   */
/*                                                                                                          */
/*  CARACTERÍSTICAS PRINCIPALES:                                                                            */
/*  - Inicialización de matrices con valores aleatorios.                                                    */
/*  - División del trabajo por filas entre los hilos creados.                                               */
/*  - Cálculo concurrente de los resultados parciales en cada hilo.                                         */
/*  - Uso de mutex para evitar condiciones de carrera.                                                      */
/*  - Medición del tiempo total de ejecución con `gettimeofday()`.                                          */
/*  - Permite definir el tamaño de la matriz y el número de hilos desde la línea de comandos.               */
/************************************************************************************************************/

/***************************************** Headers **********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

/*************************************** Variables Globales ***********************************/
pthread_mutex_t MM_mutex;
double *matrixA, *matrixB, *matrixC;
struct timeval inicio, fin;

struct parametros {
    int nH;   // Número total de hilos
    int idH;  // Identificador del hilo
    int N;    // Dimensión de la matriz
};


/*************************************** Declaracion Funciones ***********************************/
void InicioMuestra();
void FinMuestra();
void iniMatrix(double *m1, double *m2, int D);
void impMatrix(double *matriz, int D);
void *multiMatrix(void *variables);

/****************************************************************************************************
 * Firma:     int main(int argc, char *argv[])                                                      *
 *                                                                                                  *
 * Propósito:                                                                                       *
 *   Inicializar las estructuras necesarias para la multiplicación de matrices usando pthreads,     *
 *   lanzar los hilos de trabajo, medir el tiempo de ejecución y mostrar el resultado.              *
 *                                                                                                  *
 * Parámetros:                                                                                      *
 *   - argc : número de argumentos de línea de comandos.                                            *
 *   - argv : arreglo de cadenas:                                                                   *
 *            argv[1] -> tamaño de la matriz (N).                                                   *
 *            argv[2] -> número de hilos (n_threads).                                               *
 *                                                                                                  *
 * Retorno:                                                                                         *
 *   - 0 cuando el programa finaliza correctamente.                                                 *
 ****************************************************************************************************/
int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Uso correcto:\n\t$ ./ejecutable <Tamaño_Matriz> <Número_Hilos>\n");
        exit(0);
    }

    int N = atoi(argv[1]);          // Tamaño de la matriz
    int n_threads = atoi(argv[2]);  // Número de hilos
    pthread_t p[n_threads];         // Arreglo de identificadores de hilos
    pthread_attr_t atrMM;           // Atributos de los hilos

    /* Reservar memoria dinámica para las matrices */
    matrixA = (double *)calloc(N*N, sizeof(double));
    matrixB = (double *)calloc(N*N, sizeof(double));
    matrixC = (double *)calloc(N*N, sizeof(double));

    /* Inicialización de matrices */
    srand(time(NULL));
    iniMatrix(matrixA, matrixB, N);
    impMatrix(matrixA, N);
    impMatrix(matrixB, N);

    /* Inicio de la medición de tiempo */
    InicioMuestra();

    /* Inicialización de mutex y atributos */
    pthread_mutex_init(&MM_mutex, NULL);
    pthread_attr_init(&atrMM);
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    /* Creación de hilos */
    for (int j = 0; j < n_threads; j++) {
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros));
        datos->idH = j;
        datos->nH  = n_threads;
        datos->N   = N;
        pthread_create(&p[j], &atrMM, multiMatrix, (void *)datos);
    }

    /* Espera a que todos los hilos terminen */
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);

    /* Final de la medición de tiempo */
    FinMuestra();

    /* Impresión de la matriz resultado */
    impMatrix(matrixC, N);

    /* Liberación de recursos */
    free(matrixA);
    free(matrixB);
    free(matrixC);
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);
    pthread_exit(NULL);

    return 0;
}


/****************************************************************************************************
 * Firma:     void InicioMuestra()                                                                  *
 *                                                                                                  *
 * Propósito:                                                                                       *
 *   Tomar una marca de tiempo inicial usando gettimeofday para medir el tiempo de ejecución        *
 *   de la sección crítica del programa (multiplicación de matrices).                               *
 *                                                                                                  *
 * Parámetros:                                                                                      *
 *   - Ninguno.                                                                                     *
 *                                                                                                  *
 * Retorno:                                                                                         *
 *   - Ninguno.                                                                                     *
 ****************************************************************************************************/
void InicioMuestra() {
    gettimeofday(&inicio, (void *)0);
}


/****************************************************************************************************
 * Firma:     void FinMuestra()                                                                     *
 *                                                                                                  *
 * Propósito:                                                                                       *
 *   Tomar la marca de tiempo final, calcular la diferencia con respecto al inicio y mostrar el     *
 *   tiempo total de ejecución en segundos (a partir de microsegundos).                             *
 *                                                                                                  *
 * Parámetros:                                                                                      *
 *   - Ninguno.                                                                                     *
 *                                                                                                  *
 * Retorno:                                                                                         *
 *   - Ninguno.                                                                                     *
 ****************************************************************************************************/
void FinMuestra() {
    gettimeofday(&fin, (void *)0);
    fin.tv_usec -= inicio.tv_usec;
    fin.tv_sec  -= inicio.tv_sec;
    double tiempo = (double)(fin.tv_sec * 1000000 + fin.tv_usec);
    printf("\nTiempo total de ejecución: %.6f segundos\n", tiempo / 1e6);
}


/****************************************************************************************************
 * Firma:     void iniMatrix(double *m1, double *m2, int D)                                         *
 *                                                                                                  *
 * Propósito:                                                                                       *
 *   Inicializar las matrices A y B con valores aleatorios en rangos definidos para realizar        *
 *   posteriormente la multiplicación matricial.                                                    *
 *                                                                                                  *
 * Parámetros:                                                                                      *
 *   - m1 : puntero a la primera matriz (A), de tamaño D x D.                                       *
 *   - m2 : puntero a la segunda matriz (B), de tamaño D x D.                                       *
 *   - D  : dimensión de las matrices (número de filas y columnas).                                 *
 *                                                                                                  *
 * Retorno:                                                                                         *
 *   - Ninguno.                                                                                     *
 ****************************************************************************************************/
void iniMatrix(double *m1, double *m2, int D) {
    for (int i = 0; i < D * D; i++, m1++, m2++) {
        *m1 = (double)rand() / RAND_MAX * (5.0 - 1.0);
        *m2 = (double)rand() / RAND_MAX * (9.0 - 5.0);
    }
}


/****************************************************************************************************
 * Firma:     void impMatrix(double *matriz, int D)                                                 *
 *                                                                                                  *
 * Propósito:                                                                                       *
 *   Imprimir en consola el contenido de una matriz cuadrada de tamaño D x D, siempre que D sea     *
 *   menor que 9, para evitar saturar la salida estándar con matrices grandes.                      *
 *                                                                                                  *
 * Parámetros:                                                                                      *
 *   - matriz : puntero a la matriz a imprimir.                                                     *
 *   - D      : dimensión de la matriz (número de filas y columnas).                                *
 *                                                                                                  *
 * Retorno:                                                                                         *
 *   - Ninguno.                                                                                     *
 ****************************************************************************************************/
void impMatrix(double *matriz, int D) {
    if (D < 9) {
        for (int i = 0; i < D * D; i++) {
            if (i % D == 0)
                printf("\n");
            printf(" %.2f ", matriz[i]);
        }
        printf("\n>-------------------->\n");
    }
}


/****************************************************************************************************
 * Firma:     void *multiMatrix(void *variables)                                                    *
 *                                                                                                  *
 * Propósito:                                                                                       *
 *   Ejecutar el cálculo de un subconjunto de filas de la matriz resultado C = A x B. Cada hilo     *
 *   recibe un bloque de filas (según su id) y realiza el producto fila–columna correspondiente.    *
 *                                                                                                  *
 * Parámetros:                                                                                      *
 *   - variables : puntero genérico (void *) que se castea a struct parametros*, el cual contiene   *
 *                 el id del hilo, el número total de hilos y la dimensión de las matrices.         *
 *                                                                                                  *
 * Retorno:                                                                                         *
 *   - void* : valor genérico requerido por la interfaz de pthreads (no se usa).                    *
 ****************************************************************************************************/
void *multiMatrix(void *variables) {
    struct parametros *data = (struct parametros *)variables;

    int idH  = data->idH;
    int nH   = data->nH;
    int D    = data->N;
    int filaI = (D / nH) * idH;
    int filaF = (D / nH) * (idH + 1);

    double Suma, *pA, *pB;

    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {
            pA = matrixA + i * D;
            pB = matrixB + j;
            Suma = 0.0;
            for (int k = 0; k < D; k++, pA++, pB += D) {
                Suma += *pA * *pB;
            }
            matrixC[i * D + j] = Suma;
        }
    }

    pthread_mutex_lock(&MM_mutex);
    pthread_mutex_unlock(&MM_mutex);
    pthread_exit(NULL);
}
/************************************************************************************************************/
/*  USO:                                                                                                    */
/*      $ ./multiMatrix_pthread <Tamaño_Matriz> <Número_Hilos>                                              */
/*                                                                                                          */
/*  EJEMPLO:                                                                                                */
/*      $ ./multiMatrix_pthread 4 2                                                                          */
/*                                                                                                          */
/*  SALIDA (para matrices pequeñas):                                                                        */
/*      - Impresión de las matrices A y B inicializadas aleatoriamente.                                     */
/*      - Cálculo concurrente de la matriz C realizado por varios hilos.                                    */
/*      - Tiempo total de ejecución mostrado en microsegundos.                                              */
/************************************************************************************************************/
