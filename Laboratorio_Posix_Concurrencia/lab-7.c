/*
===============================================================================
Nombre:        Diego Alejandro Melgarejo Alejandro
Profesor:      John Corredor
Universidad:   Pontificia Universidad Javeriana
Asignatura:    Sistemas Operativos
===============================================================================
Objetivo del challenge:
    Implementar un programa en C que demuestra el uso de variables de condición
    (`pthread_cond_t`) y de exclusión mutua (`pthread_mutex_t`) para coordinar
    la ejecución entre hilos.

Descripción general:
    - Un hilo modifica el valor de una variable compartida.
    - Otro hilo espera la señal de actualización para mostrar el nuevo valor.

    Se incluyen secciones comentadas donde se ejemplifica el uso real de las
    funciones `pthread_cond_wait` y `pthread_cond_signal`, las cuales permiten
    sincronizar los hilos de manera controlada y evitar condiciones de carrera.
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
 * - varCondition: variable de condición usada para notificar a los hilos.
 * - mutexVar:     mutex que protege el acceso a la variable compartida.
 * - valor:        recurso compartido entre los hilos.
 * - notificar:    bandera que indica si ya se modificó el valor.
 * ------------------------------------------------------------------------- */
pthread_cond_t varCondition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexVar = PTHREAD_MUTEX_INITIALIZER;
int valor = 100;
bool notificar = false;

/* ---------------------------------------------------------------------------
 * Función: reportar
 * ---------------------------------------------------------------------------
 * Descripción:
 *    Hilo encargado de imprimir el valor de la variable compartida `valor`.
 *    En una versión completa, este hilo esperaría una señal antes de proceder,
 *    asegurando que no imprima un valor antiguo.
 *
 * Parámetros:
 *    - nousado: argumento genérico (no utilizado en este caso).
 *
 * Retorna:
 *    - NULL, ya que no se devuelve ningún valor al hilo principal.
 * ------------------------------------------------------------------------- */
void *reportar(void *nousado) {
    /*
    pthread_mutex_lock(&mutexVar);
    while(!notificar)
        pthread_cond_wait(&varCondition, &mutexVar);
    pthread_mutex_unlock(&mutexVar);
    */

    printf("El valor es: %d\n", valor);
    return NULL;
}

/* ---------------------------------------------------------------------------
 * Función: asignar
 * ---------------------------------------------------------------------------
 * Descripción:
 *    Este hilo modifica el valor compartido `valor`. En una implementación
 *    sincronizada, notificaría al otro hilo usando una variable de condición.
 *
 * Parámetros:
 *    - nousado: argumento genérico (no utilizado en este caso).
 *
 * Retorna:
 *    - NULL, ya que el hilo no necesita devolver datos.
 * ------------------------------------------------------------------------- */
void *asignar(void *nousado) {
    valor = 20;

    /*
    pthread_mutex_lock(&mutexVar);
    notificar = true;
    pthread_cond_signal(&varCondition);
    pthread_mutex_unlock(&mutexVar);
    */

    return NULL;
}

/* ---------------------------------------------------------------------------
 * Función: main
 * ---------------------------------------------------------------------------
 * Descripción:
 *    Función principal del programa. Crea y ejecuta dos hilos:
 *      1. `reporte`: encargado de imprimir el valor.
 *      2. `asigne`: encargado de modificar el valor.
 *
 *    Luego espera (join) a que ambos finalicen antes de terminar el programa.
 * ------------------------------------------------------------------------- */
int main() {
    pthread_t reporte, asigne;

    // Creación de los dos hilos
    pthread_create(&reporte, NULL, reportar, NULL);
    pthread_create(&asigne, NULL, asignar, NULL);

    // Esperar a que ambos hilos finalicen
    pthread_join(reporte, NULL);
    pthread_join(asigne, NULL);

    return 0;
}
