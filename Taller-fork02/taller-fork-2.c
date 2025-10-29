/*
 ============================================================================
 Nombre del archivo: suma_procesos.c
 Autor: Diego Alejandro Melgarejo Bejarano
 Materia: Sistemas Operativos
 Fecha: 14 de octubre de 2025
 Descripción:
    Este programa implementa un sistema de comunicación entre procesos
    utilizando tuberías (pipes) y la creación jerárquica de procesos mediante fork().

    El programa recibe dos archivos con números enteros y dos valores (N1 y N2)
    que indican cuántos números deben leerse de cada archivo.
    Luego crea tres procesos:
        - Un nieto: calcula la suma del primer arreglo.
        - Un segundo hijo: calcula la suma del segundo arreglo.
        - El primer hijo: calcula la suma total (de ambos arreglos).
    Finalmente, el proceso padre muestra los resultados obtenidos.

    Sintaxis de ejecución:
        ./suma_procesos N1 archivo00 N2 archivo01
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    // Verificación de argumentos correctos
    if (argc != 5) {
        printf("Uso correcto: %s N1 archivo00 N2 archivo01\n", argv[0]);
        return 1;
    }

    int N1 = atoi(argv[1]);
    int N2 = atoi(argv[3]);

    // ==========================================================
    // LECTURA DE LOS ARCHIVOS Y CARGA DE DATOS EN ARREGLOS
    // ==========================================================

    FILE *f1 = fopen(argv[2], "r");
    FILE *f2 = fopen(argv[4], "r");

    if (!f1 || !f2) {
        printf("Error al abrir los archivos.\n");
        return 1;
    }

    int cont1 = 0, cont2 = 0, temp;

    // Contar cuántos números tiene cada archivo
    while (fscanf(f1, "%d", &temp) == 1)
        cont1++;
    while (fscanf(f2, "%d", &temp) == 1)
        cont2++;

    rewind(f1);
    rewind(f2);

    // Validar que los N pedidos no sean mayores al tamaño del archivo
    if (N1 > cont1) {
        printf("Advertencia: el archivo %s solo tiene %d datos. Se leerán esos.\n", argv[2], cont1);
        N1 = cont1;
    }
    if (N2 > cont2) {
        printf("Advertencia: el archivo %s solo tiene %d datos. Se leerán esos.\n", argv[4], cont2);
        N2 = cont2;
    }

    // Reserva dinámica de memoria para los arreglos
    int *arr1 = (int *) malloc(N1 * sizeof(int));
    int *arr2 = (int *) malloc(N2 * sizeof(int));

    // Lectura de los datos en los arreglos
    for (int i = 0; i < N1; i++)
        fscanf(f1, "%d", &arr1[i]);
    for (int i = 0; i < N2; i++)
        fscanf(f2, "%d", &arr2[i]);

    fclose(f1);
    fclose(f2);

    // ==========================================================
    // MOSTRAR LOS NÚMEROS DE CADA ARCHIVO
    // ==========================================================

    printf("\nDatos del archivo 1:\n");
    for (int i = 0; i < N1; i++)
        printf("%d ", arr1[i]);
    printf("\n");

    printf("Datos del archivo 2:\n");
    for (int i = 0; i < N2; i++)
        printf("%d ", arr2[i]);
    printf("\n");

    // ==========================================================
    // CREACIÓN DE PROCESOS Y PIPES PARA COMUNICACIÓN
    // ==========================================================

    int pipeA[2], pipeB[2], pipeTotal[2];
    pipe(pipeA);
    pipe(pipeB);
    pipe(pipeTotal);

    pid_t pid1, pid2, pid3;

    // ==========================================================
    // PRIMER HIJO
    // ==========================================================
    pid1 = fork();

    if (pid1 == 0) {
        // ----------------------------------------------
        // NIETO
        // ----------------------------------------------
        pid3 = fork();
        if (pid3 == 0) {
            // Nieto: calcula la suma del primer arreglo
            int sumaA = 0;
            for (int i = 0; i < N1; i++)
                sumaA += arr1[i];

            close(pipeA[0]);             // Cierra lectura
            write(pipeA[1], &sumaA, sizeof(int));  // Envía suma al pipeA
            close(pipeA[1]);
            exit(0);
        } else {
            // Primer hijo espera al nieto
            wait(NULL);

            // Primer hijo calcula suma total (A + B)
            int sumaA = 0, sumaB = 0;
            for (int i = 0; i < N1; i++)
                sumaA += arr1[i];
            for (int i = 0; i < N2; i++)
                sumaB += arr2[i];

            int sumaTotal = sumaA + sumaB;

            // Envía suma total al pipeTotal
            close(pipeTotal[0]);
            write(pipeTotal[1], &sumaTotal, sizeof(int));
            close(pipeTotal[1]);
            exit(0);
        }
    }

    // ==========================================================
    // SEGUNDO HIJO
    // ==========================================================
    pid2 = fork();
    if (pid2 == 0) {
        // Segundo hijo: calcula la suma del segundo arreglo
        int sumaB = 0;
        for (int i = 0; i < N2; i++)
            sumaB += arr2[i];

        close(pipeB[0]);
        write(pipeB[1], &sumaB, sizeof(int));
        close(pipeB[1]);
        exit(0);
    }

    // ==========================================================
    // PROCESO PADRE
    // ==========================================================

    int sumaA, sumaB, sumaTotal;

    // Cierra los extremos de escritura (solo leerá)
    close(pipeA[1]);
    close(pipeB[1]);
    close(pipeTotal[1]);

    // Espera a los hijos
    wait(NULL);
    wait(NULL);

    // Lee resultados desde los pipes
    read(pipeA[0], &sumaA, sizeof(int));
    read(pipeB[0], &sumaB, sizeof(int));
    read(pipeTotal[0], &sumaTotal, sizeof(int));

    // Muestra los resultados
    printf("\n========== RESULTADOS ==========\n");
    printf("Suma del archivo 1 (nieto): %d\n", sumaA);
    printf("Suma del archivo 2 (segundo hijo): %d\n", sumaB);
    printf("Suma total (primer hijo): %d\n", sumaTotal);
    printf("================================\n");

    // Liberar memoria
    free(arr1);
    free(arr2);

    return 0;
}
