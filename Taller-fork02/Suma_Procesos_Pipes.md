# Comunicación entre Procesos con Pipes y Forks
### Autor: Diego Alejandro Melgarejo Bejarano
### Materia: Sistemas Operativos
### Fecha: 14 de octubre de 2025

---

Objetivo General
Implementar la comunicación entre múltiples procesos utilizando **pipes** y la jerarquía de procesos **padre-hijo-nieto** mediante la llamada al sistema `fork()`.  
El programa permite leer datos desde dos archivos, calcular sus sumas parciales en procesos distintos y combinar los resultados en un proceso padre.

---

Descripción General del Programa
El programa recibe los siguientes argumentos en la línea de comandos:

```
./suma_procesos N1 archivo00 N2 archivo01
```

Donde:
- `N1`: cantidad de números a leer del primer archivo.
- `archivo00`: nombre del primer archivo con números enteros.
- `N2`: cantidad de números a leer del segundo archivo.
- `archivo01`: nombre del segundo archivo con números enteros.

El objetivo es crear tres procesos que trabajen de forma coordinada:
1. **Nieto:** calcula la suma del primer archivo.
2. **Segundo hijo:** calcula la suma del segundo archivo.
3. **Primer hijo:** calcula la suma total combinando los resultados.
4. **Padre:** recibe los valores desde los pipes y los imprime.

---

Funcionamiento del Código

Lectura y validación de archivos
El programa abre los dos archivos recibidos por argumento y cuenta cuántos datos contienen.  
Si el número solicitado (`N1` o `N2`) es mayor que los datos disponibles, se ajusta automáticamente para evitar errores.

```c
FILE *f1 = fopen(argv[2], "r");
FILE *f2 = fopen(argv[4], "r");
```

Luego, los datos se almacenan en **arreglos dinámicos** `arr1` y `arr2` usando `malloc()`.

---

Creación de los Pipes
Se crean tres pipes:
- `pipeA`: comunicación entre el nieto y el padre.
- `pipeB`: comunicación entre el segundo hijo y el padre.
- `pipeTotal`: comunicación entre el primer hijo y el padre.

```c
int pipeA[2], pipeB[2], pipeTotal[2];
pipe(pipeA);
pipe(pipeB);
pipe(pipeTotal);
```

---

Estructura de Procesos
La jerarquía generada por las llamadas `fork()` es la siguiente:

```
Padre
 ├── Hijo 1
 │    └── Nieto
 └── Hijo 2
```

Cada proceso realiza una tarea diferente relacionada con la suma de los archivos.

---

Nieto
Calcula la suma de los valores del primer archivo (`arr1`) y la envía por el **pipeA**.

```c
int sumaA = 0;
for (int i = 0; i < N1; i++)
    sumaA += arr1[i];
write(pipeA[1], &sumaA, sizeof(int));
```

---

Segundo hijo
Calcula la suma de los valores del segundo archivo (`arr2`) y la envía por el **pipeB**.

```c
int sumaB = 0;
for (int i = 0; i < N2; i++)
    sumaB += arr2[i];
write(pipeB[1], &sumaB, sizeof(int));
```

---

Primer hijo
Espera que su proceso hijo (el nieto) termine, calcula la **suma total** (A + B) y la envía por **pipeTotal**.

```c
int sumaTotal = sumaA + sumaB;
write(pipeTotal[1], &sumaTotal, sizeof(int));
```

---

Proceso Padre
El proceso padre espera a que terminen sus hijos, **lee los tres resultados** desde los pipes y los muestra por pantalla:

```c
read(pipeA[0], &sumaA, sizeof(int));
read(pipeB[0], &sumaB, sizeof(int));
read(pipeTotal[0], &sumaTotal, sizeof(int));
```

Finalmente imprime:

```
========== RESULTADOS ==========
Suma del archivo 1 (nieto): X
Suma del archivo 2 (segundo hijo): Y
Suma total (primer hijo): Z
================================
```

---

Conclusión
Este ejercicio demuestra la forma en que los procesos pueden **trabajar en paralelo** y comunicarse usando **pipes anónimos**.  
También refuerza los conceptos de:
- Creación de procesos (`fork()`)
- Sincronización (`wait()`)
- Comunicación entre procesos (IPC)
- Memoria dinámica (`malloc()`)

El diseño jerárquico permite distribuir tareas y combinar resultados eficientemente, simulando una estructura básica de procesamiento concurrente.
