/*
 ============================================================================
 Nombre del archivo : productor_spooler.c
 Autor              : Diego Melgarejo, Juan Motta
 Materia            : Sistemas Operativos
 Profesor           : John Corredor
 Fecha              : 12 / 11 / 2025
 ============================================================================
 Objetivo del programa:
    Implementar un sistema de múltiples hilos productores y un hilo spooler
    que funcionan sobre un buffer circular compartido. Los productores generan
    líneas de texto que depositan en los buffers, mientras que el hilo spooler
    consume e imprime dichas líneas en orden FIFO.

 Funcionalidades:
     10 hilos productores → producen 10 líneas cada uno.
     1 hilo spooler       → consume continuamente lo que haya en el buffer.
     Exclusión mutua mediante mutex.
     Sincronización usando variables de condición.
     Implementación de buffer circular para lectura y escritura.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAX_BUFFERS 100    // Tamaño total del buffer circular

// Buffer circular para almacenar mensajes
char buffers[MAX_BUFFERS][100];

// Punteros del buffer circular
int write_pos = 0;         // Posición donde el productor escribe
int read_pos = 0;          // Posición donde el spooler lee

// Contadores de estado del buffer
int free_slots = MAX_BUFFERS;   // Espacios disponibles
int pending_lines = 0;          // Líneas listas para imprimir

// Mutex para proteger las variables compartidas
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// Variables de condición
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER; // Para productores
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER; // Para spooler

/*
 ============================================================================
 Función: producer
 Parámetro: ID del hilo productor
 Descripción:
    Cada hilo productor genera 10 líneas, espera si el buffer está lleno,
    escribe en el buffer circular, y luego despierta al spooler.
 ============================================================================
*/
void *producer(void *arg){
    int id = *((int *)arg);
    int count = 0, i, r;

    for(i = 0; i < 10; i++){ // Cada hilo produce 10 líneas

        // Bloquear mutex para tocar el buffer
        if((r = pthread_mutex_lock(&mtx)) != 0){
            fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        // Si no hay espacio libre → esperar
        while(free_slots == 0)
            pthread_cond_wait(&cond_prod, &mtx);

        // Escribir en la posición actual del buffer circular
        int pos = write_pos;
        write_pos = (write_pos + 1) % MAX_BUFFERS;
        free_slots--;

        sprintf(buffers[pos], "Thread %d: %d\n", id, ++count);
        pending_lines++;

        // Despertar al hilo spooler
        pthread_cond_signal(&cond_cons);

        // Liberar mutex
        if((r = pthread_mutex_unlock(&mtx)) != 0){
            fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        sleep(1); // Retardo para simular producción lenta
    }
    return NULL;
}

/*
 ============================================================================
 Función: spooler
 Descripción:
    Hilo consumidor que imprime continuamente las líneas disponibles.
    Si no hay líneas pendientes, espera a que algún productor escriba.
 ============================================================================
*/
void *spooler(void *arg){
    int r;

    while(1){

        // Bloqueo de mutex para acceder al buffer
        if((r = pthread_mutex_lock(&mtx)) != 0){
            fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        // No hay líneas → esperar
        while(pending_lines == 0)
            pthread_cond_wait(&cond_cons, &mtx);

        // Imprimir la línea disponible
        printf("%s", buffers[read_pos]);

        pending_lines--;
        read_pos = (read_pos + 1) % MAX_BUFFERS;
        free_slots++;

        // Despertar a productores
        pthread_cond_signal(&cond_prod);

        // Liberar mutex
        if((r = pthread_mutex_unlock(&mtx)) != 0){
            fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }
    }
    return NULL;
}

/*
 ============================================================================
 Función principal
 ============================================================================
*/
int main(int argc, char **argv){
    pthread_t prod[10], spool;
    int ids[10], i, r;

    write_pos = read_pos = 0;

    // Crear hilo spooler
    if((r = pthread_create(&spool, NULL, spooler, NULL)) != 0){
        fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
        exit(1);
    }

    // Crear 10 productores
    for(i = 0; i < 10; i++){
        ids[i] = i;
        if((r = pthread_create(&prod[i], NULL, producer, &ids[i])) != 0){
            fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }
    }

    // Esperar a que todos los productores terminen
    for(i = 0; i < 10; i++){
        if((r = pthread_join(prod[i], NULL)) != 0){
            fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }
    }

    // Esperar a que spooler consuma completamente lo producido
    while(pending_lines > 0)
        sleep(1);

    // Cancelar al hilo spooler (ya no hay nada más que imprimir)
    if((r = pthread_cancel(spool)) != 0){
        fprintf(stderr,"Error = %d (%s)\n", r, strerror(r));
        exit(1);
    }

    return 0;
}
