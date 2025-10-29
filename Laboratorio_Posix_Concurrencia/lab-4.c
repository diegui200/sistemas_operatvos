/*
 ============================================================================
 Nombre:       Diego Alejandro Melgarejo Bejarano
 Profesor:     John Corredor
 Universidad:  Pontificia Universidad Javeriana
 Asignatura:   Sistemas Operativos
 ============================================================================
 Objetivo del challenge:
    Implementar un programa en C que cree varios hilos (threads)
    utilizando la biblioteca pthread. Cada hilo ejecuta una tarea
    simulada mediante un bucle largo, representando un trabajo pesado
    (job).

    El programa busca ilustrar la creación, ejecución y sincronización
    de múltiples hilos en paralelo, así como la utilización de
    pthread_join() para esperar la finalización de todos los hilos
    antes de que termine el programa principal.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> // Para la función strerror()

/*
 * Declaración global de recursos compartidos y sincronización:
 * - tid[]: arreglo que almacena los identificadores de los hilos.
 * - counter: variable global usada para contar los trabajos iniciados.
 * - lock: mutex que protege el acceso a la variable 'counter' y evita.
 *   condiciones de carrera cuando varios hilos acceden simultáneamente.
 */
pthread_t tid[3];
int counter;
pthread_mutex_t lock;

/*
 ============================================================================
 Función: compute
 Descripción:
    Esta función representa la tarea (job) que ejecutará cada hilo.
    Cada hilo:
    1. Bloquea el mutex y aumenta el contador global de manera segura.
    2. Imprime un mensaje indicando que el trabajo ha comenzado.
    3. Ejecuta un bucle largo que simula una tarea pesada.
    4. Imprime un mensaje al finalizar su ejecución.

 Parámetros:
    - arg: puntero genérico (no utilizado en este caso).

 Retorno:
    - NULL (no retorna ningún valor útil al hilo principal).
 ============================================================================
*/
void *compute(void *arg)
{
    unsigned long i = 0;

    // Sección crítica: incremento seguro del contador
    pthread_mutex_lock(&lock);
    counter += 1;
    printf("\nJob %d has started\n", counter);
    pthread_mutex_unlock(&lock);

    // Simula trabajo intensivo (consumo de CPU)
    for(i = 0; i < 0xFFFFFFFF; i++);

    // Indica finalización del trabajo
    printf("\nJob %d has finished\n", counter);

    return NULL;
}

/*
 ============================================================================
 Función principal: main
 Descripción:
    - Inicializa el mutex de sincronización.
    - Crea tres hilos, cada uno ejecutando la función compute().
    - Maneja posibles errores en la creación de los hilos.
    - Utiliza pthread_join() para esperar que todos los hilos finalicen.
    - Destruye el mutex antes de finalizar el programa.
 ============================================================================
*/
int main(void)
{
    int i = 0;
    int error;

    // Inicialización del mutex antes de usarlo
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init failed\n");
        return 1;
    }

    // Creación de tres hilos
    while(i < 3)
    {
        error = pthread_create(&(tid[i]), NULL, &compute, NULL);

        // Verificación de errores en la creación del hilo
        if (error != 0)
            printf("\nThread can't be created: [%s]", strerror(error));
        else
            printf("\nThread %d created successfully\n", i + 1);

        i++;
    }

    // Espera la finalización de todos los hilos
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    // Destruye el mutex antes de salir
    pthread_mutex_destroy(&lock);

    printf("\nAll threads have finished execution.\n");

    return 0;
}
