# Resumen de Módulos - Laboratorios de Sistemas Operativos
**Autora:** Diego Alejandro Melgarejo Bejarano
**Profesor:** John Corredor  
**Universidad:** Pontificia Universidad Javeriana  
**Tema General:** Programación concurrente con hilos POSIX (`pthread`)

---

## LAB-1: Creación básica de hilos POSIX
### Objetivo
Implementar un programa que crea múltiples hilos utilizando la biblioteca POSIX (`pthread`).  
El propósito es comprender el flujo concurrente y la finalización ordenada mediante `pthread_create()` y `pthread_join()`.

### Funciones principales
- `void *print_message_function(void *ptr)`
  - Imprime un mensaje recibido como parámetro.
- `int main()`
  - Crea y sincroniza la ejecución de los hilos.

### Mecanismos utilizados
- Creación y unión de hilos (`pthread_create`, `pthread_join`).

### Resultado esperado
Los mensajes generados por cada hilo se imprimen en pantalla de forma concurrente.

---

## LAB-2: Sincronización simple con mutex
### Objetivo
Mostrar cómo los **mutex (`pthread_mutex_t`)** permiten proteger secciones críticas al acceder a variables compartidas.

### Funciones principales
- `void *incrementar(void *param)`
  - Incrementa una variable global protegida por un mutex.
- `int main()`
  - Crea varios hilos que comparten la variable y evita condiciones de carrera.

### Mecanismos utilizados
- Mutex (`pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`).

### Resultado esperado
El valor final de la variable compartida es coherente, sin errores de concurrencia.

---

## LAB-3: Uso de variables de condición (`pthread_cond_t`)
### Objetivo
Introducir la coordinación entre hilos mediante **variables de condición** para que un hilo espere hasta que otro complete una tarea.

### Funciones principales
- `void *reportar(void *param)`
  - Espera una señal para imprimir el valor actualizado.
- `void *asignar(void *param)`
  - Modifica el valor y notifica al hilo que espera.
- `int main()`
  - Crea ambos hilos y espera su finalización.

### Mecanismos utilizados
- `pthread_cond_wait`, `pthread_cond_signal`.
- Mutex para proteger las variables compartidas.

### Resultado esperado
El hilo `reportar` imprime el valor solo después de recibir la notificación de `asignar`.

---

## LAB-4: Espera activa vs sincronización real
### Objetivo
Demostrar la diferencia entre un hilo que **espera activamente** (sincronización ineficiente) y otro que utiliza una **condición** correctamente.

### Funciones principales
- `void *reportar(void *nousado)`
  - Espera la actualización de `valor` antes de imprimir.
- `void *asignar(void *nousado)`
  - Cambia el valor y señala al otro hilo.

### Mecanismos utilizados
- Variables de condición (`pthread_cond_t`).
- Mutex para proteger `valor`.

### Resultado esperado
El programa imprime el valor actualizado una sola vez, sin uso de bucles activos.

---

## LAB-5: División de tareas con hilos
### Objetivo
Repartir una tarea computacional (como calcular el máximo en un vector) entre varios hilos para mejorar el rendimiento.

### Funciones principales
- `void *buscarMax(void *parametro)`
  - Calcula el máximo parcial en un subarreglo asignado al hilo.
- `int maximoValor(int *vec, int n, int nhilos)`
  - Divide el trabajo y combina resultados parciales.
- `int main()`
  - Lee datos desde archivo, crea los hilos y muestra el máximo global.

### Mecanismos utilizados
- Hilos POSIX (`pthread_create`, `pthread_join`).
- Comunicación por estructuras compartidas.

### Resultado esperado
Se obtiene correctamente el valor máximo del vector mediante trabajo concurrente.

---

## LAB-6: Productor–Consumidor
### Objetivo
Implementar el patrón clásico **Productor–Consumidor** usando mutex y variables de condición.

### Funciones principales
- `void *productor(void *param)`
  - Produce ítems y los coloca en un búfer.
- `void *consumidor(void *param)`
  - Extrae ítems del búfer y los procesa.
- `int main()`
  - Inicializa, crea y sincroniza los hilos.

### Mecanismos utilizados
- `pthread_cond_wait` y `pthread_cond_signal` para manejar búfer lleno/vacío.
- Mutex para exclusión mutua.

### Resultado esperado
Los hilos cooperan correctamente sin pérdida ni sobreproducción de datos.

---

## LAB-7: Implementación de una barrera de sincronización
### Objetivo
Crear un mecanismo de **barrera** que sincronice la ejecución de varios hilos antes de continuar.

### Funciones principales
- `void *trabajador(void *param)`
  - Realiza una tarea y espera a que todos los hilos lleguen a la barrera.
- `int main()`
  - Crea los hilos y verifica la sincronización.

### Mecanismos utilizados
- Variables de condición y contadores protegidos con mutex.

### Resultado esperado
Ningún hilo avanza más allá de la barrera hasta que todos alcanzan ese punto.

---

## LAB-8: Control de recursos compartidos
### Objetivo
Gestionar el acceso controlado a un recurso (por ejemplo, una impresora o archivo) para evitar conflictos entre hilos.

### Funciones principales
- `void *usuario(void *param)`
  - Solicita acceso, utiliza el recurso y libera el mutex.
- `int main()`
  - Crea múltiples hilos simulando usuarios concurrentes.

### Mecanismos utilizados
- Mutex para acceso exclusivo.
- Posible uso de semáforos si se amplía a múltiples recursos.

### Resultado esperado
El recurso compartido nunca es utilizado simultáneamente por más de un hilo.

---

## LAB-9: Integración y monitoreo de hilos
### Objetivo
Combinar la creación, sincronización y monitoreo de hilos para desarrollar una aplicación concurrente completa.

### Funciones principales
- `void *monitor(void *param)`
  - Supervisa las operaciones realizadas por otros hilos.
- `void *trabajo(void *param)`
  - Realiza operaciones simuladas con sincronización.
- `int main()`
  - Coordina todos los componentes concurrentes.

### Mecanismos utilizados
- Mutex, variables de condición, y posiblemente señales (`pthread_kill` o flags booleanos).

### Resultado esperado
El programa demuestra un flujo concurrente estable y sincronizado entre múltiples tareas coordinadas.

---

## Observaciones Finales
- Cada laboratorio incrementa el nivel de complejidad: desde la creación básica de hilos hasta la sincronización avanzada.  
- Los mecanismos POSIX (`pthread_*`) constituyen la base de la programación concurrente en C bajo sistemas Unix/Linux.  
- Se recomienda ejecutar los programas con `make` usando el archivo `Makefile` actualizado (nombres `lab-1`, `lab-2`, etc.).  

---

**Diego Alejandro Melgarejo Bejarano**  
Pontificia Universidad Javeriana — Sistemas Operativos
