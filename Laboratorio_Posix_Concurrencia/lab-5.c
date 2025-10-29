/*
 ============================================================================
 Nombre:       Diego Alejandro Melgarejo Bejarano
 Profesor:     John Corredor
 Universidad:  Pontificia Universidad Javeriana
 Asignatura:   Sistemas Operativos
 ============================================================================
 Objetivo del challenge:
    Implementar un programa en C que utiliza múltiples hilos (threads)
    para calcular la suma de los cuadrados de los números del 1 al 20.

    Cada hilo calcula el cuadrado de un número específico y lo acumula
    en una variable global compartida. Este ejercicio busca evidenciar
    el uso de la biblioteca pthread para la creación y sincronización
    de hilos, así como los riesgos de acceder a variables compartidas
    sin protección de exclusión mutua.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

/*
 * Variable global compartida:
 *  - acumulado: almacena la suma de los cuadrados de los números.
 *
 * Todos los hilos modifican esta variable simultáneamente, lo que puede
 * provocar una condición de carrera si no se utiliza un mecanismo de
 * sincronización (por ejemplo, un mutex).
 */
int acumulado = 0;


void *cuadrados(void *x) {
    // Conversión del argumento genérico a tipo entero
    int xi = (intptr_t)x;

    // Suma el cuadrado del número al acumulador global (sin mutex)
    acumulado += xi * xi;

    // No retorna ningún valor
    return NULL;
}

/*
 ============================================================================
 Función principal: main
 Descripción:
    - Crea 20 hilos, cada uno calcula el cuadrado de un número del 1 al 20.
    - Espera a que todos los hilos finalicen con pthread_join().
    - Muestra el resultado total acumulado.

    Este programa muestra cómo la ejecución concurrente sin sincronización
    puede alterar los resultados esperados.
 ============================================================================
*/
int main(void) {

    pthread_t hilos[20]; // Arreglo de identificadores para los 20 hilos

    // Creación de los 20 hilos, pasando el número correspondiente (1–20)
    for(int i = 0; i < 20; i++) {
        pthread_create(&hilos[i], NULL, cuadrados, (void *)(intptr_t)(i + 1));
    }

    // Espera (join) hasta que todos los hilos terminen
    for(int i = 0; i < 20; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Imprime la suma final de los cuadrados
    printf("\n=========================================\n");
    printf(" Suma de cuadrados = %d\n", acumulado);
    printf("=========================================\n");

    return 0;
}
