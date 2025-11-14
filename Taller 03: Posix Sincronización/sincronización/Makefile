# Productor – Spooler con Buffer Circular
**Autor:** Diego Melgarejo, Juan Motta
**Materia:** Sistemas Operativos
**Profesor:** John Corredor
**Fecha:** 12 / 11 / 2025

---

##  Descripción General

Este programa implementa un sistema de **múltiples hilos productores** y un **hilo spooler**, los cuales comparten un **buffer circular** protegido mediante sincronización con *mutex* y *variables de condición*.

Cada productor genera líneas de texto e intenta escribirlas en el buffer.
El hilo spooler consume las líneas en orden **FIFO**, simulando un sistema de impresión.

---

##  Funcionalidades Principales

- **10 hilos productores**, cada uno produce **10 líneas**.
- **1 hilo spooler**, que imprime continuamente lo que haya en el buffer.
- **Buffer circular:** 100 posiciones disponibles.
- **Sincronización mediante:**
  - `pthread_mutex_t` para exclusión mutua.
  - `pthread_cond_t` para controlar productores y consumidor.
- Manejo seguro del acceso al buffer:
  - Variables compartidas protegidas.
  - Esperas condicionales cuando el buffer está lleno o vacío.

---

##  Conceptos Implementados

- **Productor–Consumidor clásico** usando hilos (`pthread`).
- **Buffer circular** para lectura/escritura eficiente.
- **Variables de condición**:
  - `cond_prod` → despierta productores cuando hay espacio.
  - `cond_cons` → despierta spooler cuando hay líneas disponibles.
- **Uso de `pthread_cancel`** para finalizar el hilo spooler cuando no hay más trabajo.

---

##  Archivos incluidos

| Archivo | Descripción |
|--------|-------------|
| `productor_spooler.c` | Código fuente del sistema productor-spooler |
| `Makefile` | Compilación automática usando pthread |

---

##  Ejecución

1. Compilar:
   ```bash
   make
