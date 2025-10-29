/*
 ============================================================================
 Nombre del archivo: hilos_mutex.c
 Autor: Diego Alejandro Melgarejo Bejarano
 Profesor: John Corredor
 Universidad: Pontificia Universidad Javeriana
 Asignatura: Sistemas Operativos
 Fecha: 27 de octubre de 2025
 Descripción:
    Este programa implementa un sistema multihilo utilizando la librería pthread,
    donde múltiples hilos ejecutan tareas concurrentes y acceden a una variable
    global compartida de forma segura mediante un mecanismo de exclusión mutua
    (mutex).

    Cada hilo imprime su número de creación y su identificador (ID) asignado
    por el sistema. Luego, accede a una sección crítica en la que incrementa
    un contador global protegido por un mutex para evitar condiciones de carrera.

    El ejercicio permite comprender los conceptos fundamentales de:
        - Creación de hilos con pthread_create.
        - Sincronización mediante pthread_mutex_lock / pthread_mutex_unlock.
        - Acceso concurrente a memoria compartida y exclusión mutua.

    Sintaxis de ejecución:
        ./hilos_mutex
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ==========================================================
// CONSTANTES Y VARIABLES GLOBALES
// ==========================================================

/*
 * Número total de hilos que se crearán en el programa.
 * En este caso, se crearán 10 hilos que se ejecutarán de manera concurrente.
 */
#define NTHREADS 10

/*
 * Declaración e inicialización del mutex global.
 * Este mutex protegerá la variable 'counter' para evitar conflictos
 * cuando varios hilos intenten modificarla al mismo tiempo.
 */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

/*
 * Variable global compartida entre todos los hilos.
 * Cada hilo incrementará su valor dentro de una sección crítica protegida.
 */
int counter = 0;

// ==========================================================
// FUNCIÓN DEL HILO
// ==========================================================
/*
 * Función: thread_function
 * ------------------------
 * Descripción:
 *    Función que ejecutará cada hilo creado.
 *    Recibe un puntero genérico (void*) que se interpreta como puntero a entero,
 *    el cual indica el número del hilo.
 *
 *    Cada hilo imprime su número e ID, y luego incrementa la variable global
 *    'counter' dentro de una sección crítica protegida por el mutex.
 *
 * Parámetros:
 *    arg → puntero a entero que contiene el número del hilo.
 *
 * Retorna:
 *    void* (NULL, por convención en funciones de hilos).
 */
void *thread_function(void *arg) {
    int i = *(int *)(arg); // Obtiene el número del hilo desde el argumento

    // Muestra el número del hilo y su ID asignado por pthread
    printf("Thread number: %d | Thread ID: %ld\n", i, pthread_self());

    // ----------------------------------------------------------
    // SECCIÓN CRÍTICA: INCREMENTO DEL CONTADOR GLOBAL
    // ----------------------------------------------------------
    pthread_mutex_lock(&mutex1);   // Bloquea el acceso al recurso compartido
    counter++;                     // Incrementa el contador global
    pthread_mutex_unlock(&mutex1); // Libera el acceso al recurso

    return NULL;
}

// ==========================================================
// FUNCIÓN PRINCIPAL
// ==========================================================
/*
 * Función: main
 * -------------
 * Descripción:
 *    Punto de entrada del programa.
 *    Crea múltiples hilos (NTHREADS), cada uno ejecutando 'thread_function'.
 *    Al finalizar, imprime el valor acumulado del contador global.
 *
 * Nota importante:
 *    En este código, se pasa la dirección de la variable 'i' (&i) a todos los hilos.
 *    Esto puede causar comportamientos inesperados, ya que todos los hilos
 *    comparten la misma dirección de memoria. Sin embargo, se conserva así
 *    según las especificaciones originales del reto.
 *
 * Retorna:
 *    0 si el programa finaliza correctamente.
 */
int main() {
    pthread_t thread_id[NTHREADS]; // Arreglo con los identificadores de los hilos
    int i;

    // ----------------------------------------------------------
    // CREACIÓN DE LOS HILOS
    // ----------------------------------------------------------
    for (i = 0; i < NTHREADS; i++) {
        pthread_create(&thread_id[i], NULL, thread_function, &i);
    }

    // ----------------------------------------------------------
    // ESPERA A QUE LOS HILOS FINALICEN
    // ----------------------------------------------------------
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // ----------------------------------------------------------
    // RESULTADOS FINALES
    // ----------------------------------------------------------
    printf("Final counter value: %d\n", counter);

    return 0;
}
