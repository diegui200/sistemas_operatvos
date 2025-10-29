/*
 ============================================================================
 Nombre del archivo: hilos_basicos.c
 Autor: Diego Alejandro Melgarejo Bejarano
 Materia: Sistemas Operativos
 Docente: J. Corredor, PhD
 Fecha: 27 de octubre de 2025
 Descripción:
    Este programa introduce el uso de hilos POSIX (pthread) en C.
    Se crean dos hilos independientes que ejecutan la misma función con mensajes distintos,
    mostrando la ejecución concurrente de tareas dentro de un mismo proceso.

    El programa utiliza las funciones principales de la biblioteca pthread:
        - pthread_create: para crear hilos.
        - pthread_join: para sincronizar la finalización de los hilos.

    Su propósito es ilustrar el comportamiento concurrente y la estructura básica
    de un programa multihilo.

    Sintaxis de ejecución:
        ./hilos_basicos
 ============================================================================
*/

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

// ==========================================================
// FUNCIÓN DEL HILO
// ==========================================================
/*
 * Función: print_message_function
 * --------------------------------
 * Descripción:
 *    Función que ejecutará cada hilo creado. Recibe un puntero genérico (void*)
 *    que se interpreta como puntero a cadena de caracteres, y lo imprime.
 *    Permite simular tareas independientes que se ejecutan en paralelo.
 *
 * Parámetros:
 *    ptr → puntero genérico que contiene el mensaje a imprimir.
 *
 * Retorna:
 *    NULL (por convención de funciones tipo void* en pthreads).
 */
void *print_message_function(void *ptr) {
    char *message = (char *)ptr;   // Conversión del puntero genérico a cadena
    printf("%s\n", message);       // Imprime el mensaje correspondiente al hilo
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
 *    Crea dos hilos que ejecutan la misma función con mensajes distintos.
 *    Se valida el resultado de creación de cada hilo y se sincroniza su ejecución
 *    con pthread_join, asegurando que ambos terminen antes de finalizar el programa.
 *
 * Retorna:
 *    0 si el programa finaliza correctamente.
 */
int main(void) {
    pthread_t thread1, thread2;     // Identificadores de los hilos
    char *message1 = "Thread 1";    // Mensaje del primer hilo
    char *message2 = "Thread 2";    // Mensaje del segundo hilo
    int iret1, iret2;               // Códigos de retorno de creación

    // ----------------------------------------------------------
    // CREACIÓN DE LOS HILOS
    // ----------------------------------------------------------
    iret1 = pthread_create(&thread1, NULL, print_message_function, (void*) message1);
    iret2 = pthread_create(&thread2, NULL, print_message_function, (void*) message2);

    // Verificación de creación correcta
    printf("Thread 1 returns: %d\n", iret1);
    printf("Thread 2 returns: %d\n", iret2);

    // ----------------------------------------------------------
    // SINCRONIZACIÓN DE HILOS
    // ----------------------------------------------------------
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // ----------------------------------------------------------
    // FINALIZACIÓN
    // ----------------------------------------------------------
    printf("Finalización correcta del programa.\n");

    return 0;
}
