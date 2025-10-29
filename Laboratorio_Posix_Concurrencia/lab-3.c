/*
 ============================================================================
 Nombre del archivo: hilos_condicion.c
 Autor: Diego Alejandro Melgarejo Bejarano
 Profesor: John Corredor
 Universidad: Pontificia Universidad Javeriana
 Asignatura: Sistemas Operativos
 Fecha: 27 de octubre de 2025
 Descripción:
    Este programa implementa un sistema de sincronización entre hilos en C
    utilizando la librería pthread, combinando el uso de mutex y variables
    de condición (pthread_cond_t).

    El objetivo es coordinar la ejecución de dos funciones concurrentes:
        - count01()
        - count02()
    Ambas comparten un contador global y lo incrementan de forma controlada.
    El hilo count01 se bloquea cuando el contador entra en un rango definido,
    y se reactiva únicamente cuando count02 envía una señal.

    El ejercicio permite comprender:
        - El uso combinado de mutex y variables de condición.
        - La coordinación y señalización entre hilos.
        - El control de secciones críticas y condiciones de espera.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ==========================================================
// VARIABLES GLOBALES Y SINCRONIZACIÓN
// ==========================================================

/*
 * Mutex y variable de condición para la sincronización de hilos.
 * - count_mutex: protege el acceso a la variable global 'count'.
 * - condition_mutex: controla el bloqueo y desbloqueo de hilos mediante señales.
 * - condition_cond: variable de condición que permite a un hilo esperar
 *   hasta recibir una señal de otro hilo.
 */
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

/*
 * Variable global compartida entre los hilos.
 * Su valor se incrementa por ambos hilos de forma sincronizada.
 */
int count = 0;

/*
 * Constantes de control del comportamiento del contador.
 * COUNT_DONE  → valor máximo del contador antes de finalizar.
 * COUNT_HALT1 → umbral inferior del rango de pausa.
 * COUNT_HALT2 → umbral superior del rango de pausa.
 */
#define COUNT_DONE 15
#define COUNT_HALT1 4
#define COUNT_HALT2 11

// ==========================================================
// FUNCIÓN count01
// ==========================================================
/*
 * Función: count01
 * ----------------
 * Descripción:
 *    Este hilo incrementa el contador global 'count' mientras su valor
 *    NO esté dentro del rango COUNT_HALT1 - COUNT_HALT2.
 *
 *    Si el contador entra en ese rango, el hilo se bloquea y espera una señal
 *    (pthread_cond_wait) para continuar. Dicha señal es enviada por count02().
 *
 *    La función finaliza cuando el contador alcanza COUNT_DONE.
 *
 * Retorna:
 *    NULL (por convención en hilos pthread).
 */
void *count01() {
    for (;;) {
        pthread_mutex_lock(&condition_mutex);  // Bloquea el mutex de condición

        // El hilo se bloquea si el contador está dentro del rango de pausa
        while (count >= COUNT_HALT1 && count <= COUNT_HALT2) {
            pthread_cond_wait(&condition_cond, &condition_mutex);
        }

        pthread_mutex_unlock(&condition_mutex);  // Libera el mutex de condición

        // Sección crítica: incremento seguro del contador
        pthread_mutex_lock(&count_mutex);
        count++;
        printf("Counter value functionCount > 01 <==: %d\n", count);
        pthread_mutex_unlock(&count_mutex);

        // Si el contador alcanza el valor máximo, el hilo termina
        if (count > COUNT_DONE) return NULL;
    }
}

// ==========================================================
// FUNCIÓN count02
// ==========================================================
/*
 * Función: count02
 * ----------------
 * Descripción:
 *    Este hilo también incrementa la variable global 'count'.
 *    Además, se encarga de enviar señales (pthread_cond_signal)
 *    para despertar al hilo count01 cuando el contador esté
 *    fuera del rango COUNT_HALT1 - COUNT_HALT2.
 *
 *    Representa la lógica de coordinación entre hilos mediante
 *    variables de condición.
 *
 * Retorna:
 *    NULL (por convención en hilos pthread).
 */
void *count02() {
    for (;;) {
        pthread_mutex_lock(&condition_mutex);

        // Envía una señal si el contador está fuera del rango de pausa
        if (count < COUNT_HALT1 || count > COUNT_HALT2) {
            pthread_cond_signal(&condition_cond);
        }

        pthread_mutex_unlock(&condition_mutex);

        // Sección crítica: incremento protegido
        pthread_mutex_lock(&count_mutex);
        count++;
        printf("Counter value functionCount ==> 02 <==: %d\n", count);
        pthread_mutex_unlock(&count_mutex);

        // Termina si se alcanza el valor máximo
        if (count >= COUNT_DONE) return NULL;
    }
}

// ==========================================================
// FUNCIÓN PRINCIPAL
// ==========================================================
/*
 * Función: main
 * -------------
 * Descripción:
 *    Crea dos hilos que ejecutan las funciones count01() y count02().
 *    Ambos cooperan y sincronizan su ejecución mediante mutex y
 *    variables de condición. El programa principal espera la
 *    finalización de ambos hilos antes de terminar.
 *
 * Retorna:
 *    0 si el programa finaliza correctamente.
 */
int main() {
    pthread_t thread1, thread2; // Identificadores de los hilos

    // Creación de los hilos
    pthread_create(&thread1, NULL, count01, NULL);
    pthread_create(&thread2, NULL, count02, NULL);

    // Espera a que ambos hilos finalicen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Finalización exitosa
    printf("\nPrograma finalizado correctamente.\n");
    return 0;
}
