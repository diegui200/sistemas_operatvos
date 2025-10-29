/*********************************************************************************************
 * Pontificia Universidad Javeriana
 *
 * Autor:       Diego Alejandro Melgarejo Bejarano
 * Materia:     Sistemas Operativos
 * Docente:     J. Corredor, PhD
 * Fecha:       27/10/2025
 * Tema:        POSIX - Creación y sincronización de hilos (pthread)
 *
 * Descripción general:
 *     Este programa en C utiliza la biblioteca POSIX Threads (pthread) para dividir
 *     el trabajo de búsqueda del valor máximo en un vector entre varios hilos.
 *
 *     Cada hilo analiza una porción del vector, calcula el máximo parcial y,
 *     posteriormente, el programa principal combina dichos resultados para obtener
 *     el máximo global.
 *********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* ============================================================================
 * ESTRUCTURA: argHilos
 * ----------------------------------------------------------------------------
 * Descripción:
 *    Contiene los parámetros que recibe cada hilo para procesar su segmento del vector.
 *
 * Campos:
 *    - inicio: índice de inicio (inclusive) del rango asignado al hilo.
 *    - fin: índice de fin (exclusivo) del rango asignado al hilo.
 *    - vector: puntero al arreglo completo de enteros sobre el que se realiza la búsqueda.
 *    - maxparcial: variable donde el hilo almacena el máximo encontrado en su segmento.
 *
 * Uso:
 *    Esta estructura se pasa como argumento a la función `buscarMax`, que es la rutina
 *    ejecutada por cada hilo creado con `pthread_create()`.
 * ============================================================================ */
typedef struct argHilos {
    int inicio;
    int fin;
    int *vector;
    int maxparcial;
} param_H;

/* ============================================================================
 * FUNCIÓN: buscarMax
 * ----------------------------------------------------------------------------
 * Descripción:
 *    Función ejecutada por cada hilo. Se encarga de buscar el valor máximo
 *    dentro del rango [inicio, fin) del vector asignado.
 *
 * Pasos:
 *    1. Inicializa `maxparcial` con el primer elemento del segmento.
 *    2. Recorre el subarreglo y actualiza el máximo encontrado.
 *    3. Al terminar, el resultado queda almacenado en la estructura recibida.
 *
 * Parámetros:
 *    - parametro: puntero genérico (void*) a una estructura `param_H` con los datos del hilo.
 *
 * Retorna:
 *    - NULL (la salida del hilo no se utiliza).
 *
 * Notas:
 *    - Se asume que `inicio` < `fin` y que los índices están dentro de los límites del vector.
 * ============================================================================ */
void *buscarMax(void *parametro) {
    param_H *argumentos = (param_H *)parametro;

    // Inicializar el máximo parcial con el primer elemento del segmento asignado
    argumentos->maxparcial = argumentos->vector[argumentos->inicio];

    // Recorrer el rango [inicio, fin) para determinar el máximo
    for (int i = argumentos->inicio; i < argumentos->fin; i++) {
        if (argumentos->vector[i] > argumentos->maxparcial)
            argumentos->maxparcial = argumentos->vector[i];
    }

    // Finaliza el hilo (también podría usar return NULL)
    pthread_exit(0);
    return NULL;
}

/* ============================================================================
 * FUNCIÓN: maximoValor
 * ----------------------------------------------------------------------------
 * Descripción:
 *    Divide el vector en segmentos y lanza `nhilos` hilos para encontrar el máximo
 *    de cada parte. Luego combina los resultados parciales para obtener el máximo global.
 *
 * Parámetros:
 *    - vector: puntero al arreglo de enteros.
 *    - n: cantidad total de elementos en el vector.
 *    - nhilos: número de hilos a utilizar.
 *
 * Retorna:
 *    - El valor máximo encontrado en todo el vector.
 *
 * Notas:
 *    - Cada hilo recibe su propio bloque mediante una estructura `param_H`.
 *    - Se espera que `nhilos <= n` y que `n > 0`.
 * ============================================================================ */
int maximoValor(int *vector, int n, int nhilos) {
    pthread_t hilos[nhilos];
    param_H args[nhilos];

    int tamSegmento = n / nhilos;
    int maxGlobal = vector[0];

    // Crear los hilos y asignar sus rangos
    for (int i = 0; i < nhilos; i++) {
        args[i].inicio = i * tamSegmento;
        args[i].fin = (i == nhilos - 1) ? n : (i + 1) * tamSegmento;
        args[i].vector = vector;
        pthread_create(&hilos[i], NULL, buscarMax, (void *)&args[i]);
    }

    // Esperar la finalización de los hilos y combinar los resultados parciales
    for (int i = 0; i < nhilos; i++) {
        pthread_join(hilos[i], NULL);
        if (args[i].maxparcial > maxGlobal)
            maxGlobal = args[i].maxparcial;
    }

    return maxGlobal;
}

/* ============================================================================
 * FUNCIÓN PRINCIPAL: main
 * ----------------------------------------------------------------------------
 * Descripción:
 *    - Valida los argumentos de entrada.
 *    - Abre un archivo de texto con los datos del vector.
 *    - Lee el tamaño del vector y sus elementos.
 *    - Llama a `maximoValor()` para calcular el máximo usando varios hilos.
 *    - Muestra el resultado en pantalla.
 *
 * Uso:
 *    ./programa archivo.txt nhilos
 *
 * Formato esperado del archivo:
 *    6
 *    3 12 7 9 21 4
 *
 * Donde:
 *    - La primera línea indica el tamaño del vector (n).
 *    - Las siguientes contienen los n valores a procesar.
 * ============================================================================ */
int main(int argc, char *argv[]) {
    FILE *fichero;
    int n, nhilos, i, ret, maximo;
    int *vec;

    // Validar número de argumentos
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo.txt> <nhilos>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Abrir archivo
    fichero = fopen(argv[1], "r");
    if (fichero == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Leer tamaño del vector
    if (fscanf(fichero, "%d", &n) != 1) {
        fprintf(stderr, "Error al leer el tamaño del vector.\n");
        fclose(fichero);
        exit(EXIT_FAILURE);
    }

    // Leer número de hilos
    nhilos = atoi(argv[2]);
    if (nhilos <= 0 || nhilos > n) {
        fprintf(stderr, "Número de hilos inválido.\n");
        fclose(fichero);
        exit(EXIT_FAILURE);
    }

    // Reservar memoria dinámica para el vector
    vec = malloc(sizeof(int) * n);
    if (vec == NULL) {
        fprintf(stderr, "Error reservando memoria.\n");
        fclose(fichero);
        exit(EXIT_FAILURE);
    }

    // Leer los valores del archivo
    for (i = 0; i < n; i++) {
        if (fscanf(fichero, "%d", &vec[i]) != 1) {
            fprintf(stderr, "Error al leer el elemento número %d.\n", i);
            free(vec);
            fclose(fichero);
            exit(EXIT_FAILURE);
        }
    }

    // Calcular el máximo usando hilos
    maximo = maximoValor(vec, n, nhilos);

    // Mostrar resultado
    printf("Máximo: %d\n", maximo);

    // Liberar recursos
    fclose(fichero);
    free(vec);

    return 0;
}
