#===============================================================================
#  Makefile para la compilación de programas de Multiplicación de Matrices
#  Autor: J. Corredor, PhD (adaptado por Carolina Ujueta Ricardo)
#  Fecha: 13 Noviembre 2025
#===============================================================================

CC = gcc
CFLAGS = -Wall -O2
FOPENMP = -fopenmp
POSIX = -lpthread

# Programas finales
PROGRAMAS = mmClasicaFork mmClasicaOpenMP mmClasicaPosix mmFilasOpenMP

# Regla principal
all: $(PROGRAMAS)

# ------------------------------------------------------------------------------
# Cada ejecutable compila su main + el archivo funciones.c común
# ------------------------------------------------------------------------------

mmClasicaFork:
	$(CC) $(CFLAGS) Fork/mmClasicaFork_main.c mmClasicaFork.c -o $@ $(POSIX)

mmClasicaOpenMP:
	$(CC) $(CFLAGS) $(FOPENMP) OpenMP/mmClasicaOpenMP_main.c mmClasicaOpenMP.c -o $@

mmClasicaPosix:
	$(CC) $(CFLAGS) $(POSIX) Posix/mmClasicaPosix_main.c mmClasicaPosix.c -o $@

mmFilasOpenMP:
	$(CC) $(CFLAGS) $(FOPENMP) OpenMP-filas/mmFilasOpenMP_main.c mmFilasOpenMP.c -o $@

# Limpieza
clean:
	rm -f $(PROGRAMAS)
