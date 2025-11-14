/*
 ============================================================================
 Nombre del archivo : consumidor.c
 Autor              : Diego Melgarejo, Juan Motta
 Materia            : Sistemas Operativos
 Profesor           : John Corredor
 Fecha              : 13 / 11 / 2025
 ============================================================================
 Objetivo del programa:
    Implementar el rol de un consumidor dentro del problema productor–
    consumidor utilizando:
         Semáforos POSIX (sem_open, sem_wait, sem_post)
         Memoria compartida con shm_open y mmap
         Un buffer circular compartido con otro proceso productor.

 Funcionalidades:
     Abre los semáforos 'vacio' y 'lleno' creados por el productor.
     Abre la memoria compartida donde se encuentra el buffer circular.
     Consume 10 elementos del buffer, respetando sincronización.
     Utiliza sem_wait y sem_post para controlar acceso al buffer.
     Avanza un índice de lectura tipo buffer circular.
     Limpia memoria, cierra semáforos y elimina recursos al finalizar.
 ============================================================================
*/

#include "semaforo.h"

/*
 ============================================================================
 Función principal
 Descripción:
    Este proceso actúa como CONSUMIDOR. Accede a la memoria compartida
    creada por el productor y consume 10 elementos del buffer en orden FIFO.
 ============================================================================
*/
int main() {

    /*
     * ----------------------------------------------------------------------
     * 1. Abrir semáforos existentes
     * ----------------------------------------------------------------------
     *
     * Los semáforos fueron creados previamente por el proceso productor.
     *
     * - /vacio → lleva la cuenta de espacios libres.
     * - /lleno → lleva la cuenta de ítems disponibles para consumir.
     */
    sem_t *vacio = sem_open("/vacio", 0);
    sem_t *lleno = sem_open("/lleno", 0);

    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    /*
     * ----------------------------------------------------------------------
     * 2. Abrir la memoria compartida
     * ----------------------------------------------------------------------
     *
     * shm_open abre el segmento creado por el productor.
     */
    int fd_compartido = shm_open("/memoria_compartida", O_RDWR, 0644);
    if (fd_compartido < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /*
     * ----------------------------------------------------------------------
     * 3. Mapear el segmento de memoria a nuestro espacio virtual
     * ----------------------------------------------------------------------
     *
     * mmap nos permite acceder a la estructura compartir_datos como si fuera
     * una variable local.
     */
    compartir_datos *compartir = mmap(
        NULL,
        sizeof(compartir_datos),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd_compartido,
        0
    );

    if (compartir == MAP_FAILED) {
        perror("mmap");
        close(fd_compartido);
        exit(EXIT_FAILURE);
    }

    /*
     * Inicializar el índice de salida en 0.
     * El productor mantiene 'entrada', el consumidor mantiene 'salida'.
     */
    compartir->salida = 0;

    /*
     * ----------------------------------------------------------------------
     * 4. Consumir 10 elementos del buffer circular
     * ----------------------------------------------------------------------
     *
     * El consumidor:
     *   - Espera que haya ítems disponibles (sem_wait(lleno))
     *   - Lee el elemento de la posición actual
     *   - Avanza el índice circular
     *   - Libera un espacio vacío (sem_post(vacio))
     */
    for (int i = 1; i <= 10; i++) {

        /* Esperar a que haya un ítem disponible */
        sem_wait(lleno);

        /* Consumir el ítem en la posición 'salida' */
        int item = compartir->bus[compartir->salida];
        printf("Consumidor: Consume %d\n", item);

        /* Avanzar el índice circular */
        compartir->salida = (compartir->salida + 1) % BUFFER;

        /* Notificar que ahora hay un espacio vacío */
        sem_post(vacio);

        /* Retardo para simular consumo lento */
        sleep(2);
    }

    /*
     * ----------------------------------------------------------------------
     * 5. Liberar memoria y cerrar semáforos
     * ----------------------------------------------------------------------
     */
    munmap(compartir, sizeof(compartir_datos));
    close(fd_compartido);
    sem_close(vacio);
    sem_close(lleno);

    /*
     * ----------------------------------------------------------------------
     * 6. Eliminar recursos del sistema
     * ----------------------------------------------------------------------
     *
     * El consumidor elimina /lleno y el segmento de memoria. Es importante
     * que solo uno de los procesos lo elimine para evitar errores.
     */
    sem_unlink("/lleno");
    shm_unlink("/memoria_compartida");

    return 0;
}

