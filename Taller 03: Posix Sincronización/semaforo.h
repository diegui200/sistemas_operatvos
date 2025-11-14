/*
 ============================================================================
 Nombre del archivo : semaforo.h
 Autor              : Diego Melgarejo, Juan Motta
 Materia            : Sistemas Operativos
 Profesor           : John Corredor
 Fecha              : 13 / 11 / 2025
 ============================================================================
 Objetivo del archivo:
    Declarar la estructura de datos compartida y los elementos fundamentales
    requeridos para la implementación del problema Productor–Consumidor
    utilizando:
         Memoria compartida POSIX (shm_open, mmap)
         Semáforos POSIX con nombre
         Un buffer circular de tamaño fijo

 Descripción general:
    Este archivo de encabezado define:
        - Constante BUFFER: capacidad del buffer circular.
        - Estructura compartir_datos: contiene el arreglo compartido de enteros
          y los índices de lectura/escritura utilizados por productor y consumidor.
        - Inclusiones estándar necesarias para manejar:
              • memoria compartida
              • semáforos
              • operaciones de archivo y sistema

    Tanto el productor como el consumidor deben incluir este header para
    acceder correctamente a la misma estructura y constantes compartidas.
 ============================================================================
*/

#ifndef __SEMAFORO_H__
#define __SEMAFORO_H__

/* =========================================================================
 * Inclusión de librerías necesarias para:
 *  - Entrada/salida estándar
 *  - Gestión de memoria dinámica
 *  - Funciones de sistema (open, close, ftruncate…)
 *  - Manejo de memoria compartida POSIX (shm_open, mmap)
 *  - Semáforos POSIX con nombre (sem_open, sem_wait, sem_post…)
 * =========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

/*
 ============================================================================
 Constante: BUFFER
 Descripción:
    Tamaño del buffer circular compartido entre el proceso productor y el
    proceso consumidor. Este valor determina:
        - El número máximo de elementos que puede contener el buffer.
        - La inicialización del semáforo "vacio" (BUFFER espacios libres).
 ============================================================================
*/
#define BUFFER 5

/*
 ============================================================================
 Estructura: compartir_datos
 Descripción:
    Modelo de la región de memoria compartida. Contiene:
        - bus[]   → Arreglo de tamaño BUFFER que actúa como buffer circular.
        - entrada → Índice donde el productor almacena el siguiente elemento.
        - salida  → Índice donde el consumidor retira el siguiente elemento.
 ============================================================================
*/
typedef struct {
    int bus[BUFFER];   // Buffer circular
    int entrada;       // Índice de escritura (Productor)
    int salida;        // Índice de lectura (Consumidor)
} compartir_datos;

#endif  // __SEMAFORO_H__
