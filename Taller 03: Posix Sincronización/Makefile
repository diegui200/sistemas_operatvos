# ============================================================================
# Makefile - Sistema Productor / Consumidor con Semáforos POSIX y Memoria Compartida
# Autor: Diego Melgarejo, Juan Motta
# Universidad Javeriana
# Materia: Sistemas Operativos
# Profesor: John Jairo Corredor
# ============================================================================
# Descripción:
#   Este Makefile compila automáticamente los programas del sistema
#   Productor–Consumidor que utilizan:
#       - Semáforos POSIX con nombre (sem_open)
#       - Memoria compartida POSIX (shm_open + mmap)
#       - Buffer circular definido en 'semaforo.h'
#   Programas incluidos:
#        productor.c   → Crea semáforos, memoria compartida y produce valores.
#        consumidor.c  → Abre semáforos/memoria existente y consume valores.
#   El objetivo es facilitar la compilación y limpieza de los ejecutables.
# ============================================================================

CC = gcc
CFLAGS = -Wall -pthread

# Ejecutables
PRODUCTOR = productor
CONSUMIDOR = consumidor

# Fuentes
PROD_SRC = productor.c
CONS_SRC = consumidor.c

# Header
HEADER = semaforo.h

# Regla principal
all: $(PRODUCTOR) $(CONSUMIDOR)

$(PRODUCTOR): $(PROD_SRC) $(HEADER)
	$(CC) $(CFLAGS) $(PROD_SRC) -o $(PRODUCTOR)

$(CONSUMIDOR): $(CONS_SRC) $(HEADER)
	$(CC) $(CFLAGS) $(CONS_SRC) -o $(CONSUMIDOR)

# Limpiar ejecutables
clean:
	rm -f $(PRODUCTOR) $(CONSUMIDOR)
