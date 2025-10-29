/*
 ============================================================================
 Nombre:       Diego Alejandro Melagrejo Bejarano
 Profesor:     John Corredor
 Universidad:  Pontificia Universidad Javeriana
 Asignatura:   Sistemas Operativos
 ============================================================================
 Objetivo del challenge:
    Implementar un programa en C que utiliza múltiples hilos (threads)
    para calcular la suma de los cuadrados de los números del 1 al 20.

    Cada hilo calcula el cuadrado de un número específico y lo acumula
    en una variable global compartida. Este ejercicio busca demostrar
    el uso de la biblioteca pthread para la creación y sincronización
    de hilos, evidenciando cómo el uso de exclusión mutua (mutex)
    evita condiciones de carrera al acceder a recursos compartidos.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

/*
 * Mutex global:
 * Protege el acceso concurrente a la variable compartida 'acumulado'.
 * Inicializado estáticamente mediante la macro PTHREAD_MUTEX_INITIALIZER.
 */
pthread_mutex_t acumuladoMutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Variable global compartida:
 *  - 'acumulado' almacena la suma de los cuadrados calculados por los hilos.
 *  - El acceso a esta variable se encuentra protegido por el mutex.
 */
int acumulado = 0;

/*
 ============================================================================
 Función: cuadrados
 Descripción:
    Calcula el cuadrado del número recibido como argumento (xi)
    y lo suma al acumulador global, utilizando un mutex para
    evitar condiciones de carrera entre los hilos.

 Parámetros:
    - x: puntero genérico que contiene el número (1–20) que se procesará.

 Retorno:
    - NULL (no retorna valor).
 ============================================================================
*/
void *cuadrados(void *x) {
    // Conversión del argumento recibido a tipo entero
    int xi = (intptr_t)x;

    // Bloqueo del mutex: garantiza acceso exclusivo a la variable global
    pthread_mutex_lock(&acumuladoMutex);

    // Sección crítica: actualización segura del acumulador
    acumulado += xi * xi;

    // Desbloqueo del mutex: otros hilos pueden continuar
    pthread_mutex_unlock(&acumuladoMutex);

    // Fin del hilo
    return NULL;
}

/*
 ============================================================================
 Función principal: main
 Descripción:
    Crea 20 hilos, cada uno encargado de calcular el cuadrado de un número
    del 1 al 20. Posteriormente, el programa espera la finalización de todos
    los hilos (join) y muestra el resultado final.

    El uso del mutex asegura la consistencia del dato compartido 'acumulado'
    y evita condiciones de carrera en la sección crítica.
 ============================================================================
*/
int main(void) {

    pthread_t hilos[20]; // Arreglo para almacenar los identificadores de los 20 hilos

    // Creación de los 20 hilos
    for(int i = 0; i < 20; i++) {
        pthread_create(&hilos[i], NULL, cuadrados, (void *)(intptr_t)(i + 1));
    }

    // Espera (join) de todos los hilos antes de continuar
    for(int i = 0; i < 20; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Impresión del resultado final
    printf("\n=========================================\n");
    printf(" Suma de cuadrados = %d\n", acumulado);
    printf("=========================================\n");

    // Destrucción del mutex (buena práctica)
    pthread_mutex_destroy(&acumuladoMutex);

    return 0;
}
