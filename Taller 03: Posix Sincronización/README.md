# Sistema Productor – Consumidor con Semáforos POSIX y Memoria Compartida  
**Autor:** Diego  Melgarejo, Juan Motta
**Materia:** Sistemas Operativos
**Fecha:** 12 / 11 / 2025

---

##  Descripción general

Este proyecto implementa el clásico problema **Productor–Consumidor**, utilizando:

-  Semáforos POSIX con nombre (`sem_open`)
-  Memoria compartida POSIX (`shm_open` + `mmap`)
-  Buffer circular
-  Dos procesos independientes: Productor y Consumidor

El productor escribe números consecutivos en un buffer compartido y el consumidor los lee en orden FIFO.

---

##  Archivos del proyecto

| Archivo | Descripción |
|--------|-------------|
| `semaforo.h` | Encabezado con la estructura compartida y la constante `BUFFER` |
| `productor.c` | Crea semáforos, memoria compartida y escribe 10 elementos |
| `consumidor.c` | Abre semáforos y memoria existente, consume 10 elementos |
| `Makefile` | Permite compilar automáticamente los programas |

---

##  Estructura compartida

El archivo `semaforo.h` define:

```c
#define BUFFER 5

typedef struct {
    int bus[BUFFER];
    int entrada;
    int salida;
} compartir_datos;
