/*
===============================================================================
Nombre:        Diego Alejandro Melgarejo Bejarano
Profesor:      John Corredor
Universidad:   Pontificia Universidad Javeriana
Asignatura:    Sistemas Operativos
===============================================================================
Objetivo del challenge:
    Implementar un programa en C que sincroniza la ejecución de dos hilos
    utilizando variables de condición (`pthread_cond_t`) y exclusión mutua 
    (`pthread_mutex_t`).

Descripción general:
    - Un hilo ("productor") actualiza una variable compartida (`valor`).
    - Otro hilo ("consumidor") espera la notificación de que el valor cambió 
      antes de imprimirlo.

    Este ejemplo demuestra la coordinación entre hilos y cómo usar correctamente
    las primitivas de sincronización de POSIX (pthread).
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------------
 * VARIABLES GLOBALES Y DE SINCRONIZACIÓN
 * ---------------------------------------------------------------------------
 * varCondition : Variable de condición usada para notificar eventos entre hilos.
 * mutexVar     : Mutex que protege el acceso a las variables compartidas.
 * valor        : Dato compartido que será modificado por un hilo y leído por otro.
 * notificar    : Bandera lógica que indica si el valor ha sido actualizado.
 * ------------------------------------------------------------------------- */
pthread_cond_t varCondition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexVar = PTHREAD_MUTEX_INITIALIZER;
int valor = 100;
bool notificar = false;

/* ---------------------------------------------------------------------------
 * Función: reportar
 * ---------------------------------------------------------------------------
 * Descripción:
 *    Hilo consumidor. Espera a que otro hilo (productor) modifique la variable
 *    compartida `valor`. Utiliza una variable de condición para suspenderse
 *    hasta recibir la señal de actualización.
 *
 * Detalle técnico:
 *    - `pthread_cond_wait()` libera el mutex y suspende el hilo hasta que se
 *      recibe una señal.
 *    - Cuando se despierta, el mutex se bloquea automáticamente de nuevo.
 *
 * Parámetros:
 *    - nousado: puntero genérico no utilizado (compatibilidad con pthreads).
 *
 * Retorna:
 *    - NULL, ya que el hilo no devuelve datos al principal.
 * ------------------------------------------------------------------------- */
void *reportar(void *nousado) {
    pthread_mutex_lock(&mutexVar);

    // Esperar mientras no se haya modificado la variable
    while (!notificar)
        pthread_cond_wait(&varCondition, &mutexVar);

    pthread_mutex_unlock(&mutexVar);

    // Al recibir la señal, imprime el nuevo valor
    printf("El valor es: %d\n", valor);
    return NULL;
}

/* ---------------------------------------------------------------------------
 * Función: asignar
 * ---------------------------------------------------------------------------
 * Descripción:
 *    Hilo productor. Modifica el valor compartido `valor` y envía una señal
 *    para notificar al hilo consumidor que la actualización está disponible.
 *
 * Detalle técnico:
 *    - `pthread_mutex_lock()` garantiza exclusión mutua al modificar variables.
 *    - `pthread_cond_signal()` despierta un hilo que esté esperando la condición.
 *
 * Parámetros:
 *    - nousado: puntero genérico no utilizado.
 *
 * Retorna:
 *    - NULL, ya que no se requiere devolver un valor.
 * ------------------------------------------------------------------------- */
void *asignar(void *nousado) {
    valor = 20;

    pthread_mutex_lock(&mutexVar);
    notificar = true;

    // Notificar al hilo que espera la condición
    pthread_cond_signal(&varCondition);

    pthread_mutex_unlock(&mutexVar);
    return NULL;
}

/* ---------------------------------------------------------------------------
 * Función: main
 * ---------------------------------------------------------------------------
 * Descripción:
 *    Función principal del programa. Crea y lanza dos hilos:
 *      1. `reporte`: hilo consumidor que espera la notificación.
 *      2. `asigne`:  hilo productor que modifica el valor.
 *
 *    Finalmente, el programa espera a que ambos hilos finalicen antes de salir.
 *
 * Flujo general:
 *    main() → crea hilos → productor modifica valor → señal → consumidor imprime.
 * ------------------------------------------------------------------------- */
int main() {
    pthread_t reporte, asigne;

    // Crear los hilos
    pthread_create(&reporte, NULL, reportar, NULL);
    pthread_create(&asigne, NULL, asignar, NULL);

    // Esperar la finalización de ambos hilos
    pthread_join(reporte, NULL);
    pthread_join(asigne, NULL);

    return 0;
}
