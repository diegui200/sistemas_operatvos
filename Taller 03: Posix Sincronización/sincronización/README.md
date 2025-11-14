[README.md](https://github.com/user-attachments/files/23538996/README.md)
# =============================================================================
# Makefile - Productor y Spooler con Buffer Circular
# Autor: Diego Melgarejo, Juan Motta
# Universidad Javeriana
# Profesor: John Corredor
# Fecha: 12 / 11 / 2025
# =============================================================================
# Descripción:
#   Compila automáticamente el programa productor_spooler.c con soporte para
#   la biblioteca pthread. Incluye tareas para compilación, ejecución y limpieza.
# =============================================================================

CC = gcc
CFLAGS = -Wall -pthread
TARGET = productor_spooler
SRC = productor_spooler.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

