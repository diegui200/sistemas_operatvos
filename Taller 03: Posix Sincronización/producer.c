/*
 ============================================================================
 Nombre del archivo : productor.c
 Autor              : Diego Melgarejo, Juan Motta
 Materia            : Sistemas Operativos
 Profesor           : John Corredor
 Fecha              : 23 / 12 / 2025
 ============================================================================
 Objetivo del programa:
    Implementar el rol del productor dentro del clásico problema
    Productor–Consumidor utilizando:
         Memoria compartida (shm_open, ftruncate, mmap)
         Semáforos POSIX (sem_open, sem_wait, sem_post)
         Un buffer circular de enteros compartido entre procesos

 Funcionalidades:
     Crea los semáforos "vacio" y "lleno".
     Crea o abre un segmento de memoria compartida.
     Inicializa la posición de escritura (entrada) del buffer circular.
     Produce 10 valores enteros, uno por segundo.
     Coordina la escritura en el buffer mediante semáforos.
     Libera todos los recursos del sistema al terminar.
 ============================================================================
*/

#include "semaforo.h"

/*
 ============================================================================
 Función principal
 Descripción:
    Este proceso actúa como PRODUCTOR. Coloca 10 elementos enteros en un
    buffer circular compartido con un proceso consumidor.
 ============================================================================
*/
int main() {

    /*
     * ----------------------------------------------------------------------
     * 1. Crear o abrir semáforos POSIX
     * ----------------------------------------------------------------------
     *
     * /vacio → Contador inicial del número de espacios disponibles (BUFFER).
     * /lleno → Contador inicial de elementos disponibles (0).
     */
    sem_t *vacio = sem_open("/vacio", O_CREAT, 0644, BUFFER);
    sem_t *lleno = sem_open("/lleno", O_CREAT, 0644, 0);

    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    /*
     * ----------------------------------------------------------------------
     * 2. Crear o abrir la memoria compartida
     * ----------------------------------------------------------------------
     *
     * shm_open crea un archivo especial en memoria que permite
     * comunicarse entre procesos.
     */
    int shm_fd = shm_open("/memoria_compartida", O_CREAT | O_RDWR, 0644);
    if (shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /*
     * ----------------------------------------------------------------------
     * 3. Ajustar el tamaño del segmento con ftruncate
     * ----------------------------------------------------------------------
     */
    if (ftruncate(shm_fd, sizeof(compartir_datos)) == -1) {
        perror("ftruncate");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    /*
     * ----------------------------------------------------------------------
     * 4. Mapear el segmento de memoria al espacio del proceso
     * ----------------------------------------------------------------------
     *
     * mmap devuelve un puntero que representa la memoria compartida.
     */
    compartir_datos *compartir = mmap(
        NULL,
        sizeof(compartir_datos),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        shm_fd,
        0
    );

    if (compartir == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    /*
     * Inicializar índice de entrada.
     *
     * El productor controla la variable 'entrada', el consumidor controla
     * la variable 'salida'.
     */
    compartir->entrada = 0;

    /*
     * ----------------------------------------------------------------------
     * 5. Producción de elementos (10 enteros)
     * ----------------------------------------------------------------------
     *
     * Por cada elemento:
     *   - sem_wait(vacio) → espera si no hay espacios libres.
     *   - escribe en el buffer en la posición 'entrada'.
     *   - avanza la posición circular.
     *   - sem_post(lleno) → notifica al consumidor que hay un ítem disponible.
     */
    for (int i = 1; i <= 10; i++) {

        /* Esperar espacio libre en el buffer */
        sem_wait(vacio);

        /* Insertar elemento en el buffer */
        compartir->bus[compartir->entrada] = i;
        printf("Productor: Produce %d\n", i);

        /* Avanzar índice circular */
        compartir->entrada = (compartir->entrada + 1) % BUFFER;

        /* Notificar que hay un nuevo elemento disponible */
        sem_post(lleno);

        /* Simulación de tiempo de producción */
        sleep(1);
    }

    /*
     * ----------------------------------------------------------------------
     * 6. Liberar recursos del programa
     * ----------------------------------------------------------------------
     */
    munmap(compartir, sizeof(compartir_datos));
    close(shm_fd);

    sem_close(vacio);
    sem_close(lleno);

    /*
     * ----------------------------------------------------------------------
     * 7. Eliminar semáforos y memoria compartida del sistema
     * ----------------------------------------------------------------------
     *
     * IMPORTANTE: Solo debe hacerse cuando ya no habrá más procesos 
     * que los utilicen.
     */
    sem_unlink("/vacio");
    sem_unlink("/lleno");
    shm_unlink("/memoria_compartida");

    return 0;
}

