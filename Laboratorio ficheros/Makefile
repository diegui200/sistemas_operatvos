# ============================================================================
# Makefile para compilar los programas SistemaFichero.c y lab2.c
# Autor: Diego Alejandro Melgarejo Bejarano
# Materia: Sistemas Operativos
# Fecha: 11 / 11 / 2025
# ============================================================================

CC = gcc

CFLAGS = -lpthread

EXEC1 = SistemaFichero
EXEC2 = lab2

SRC1 = SistemaFichero.c
SRC2 = lab2.c

all: $(EXEC1) $(EXEC2)

$(EXEC1): $(SRC1)
	$(CC) $(CFLAGS) -o $(EXEC1) $(SRC1)

$(EXEC2): $(SRC2)
	$(CC) $(CFLAGS) -o $(EXEC2) $(SRC2)

clean:
	rm -f $(EXEC1) $(EXEC2)

mrproper: clean
	rm -f *~
