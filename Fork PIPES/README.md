# Laboratorios Fork PIPES --- Sistemas Operativos

Pequeño conjunto de 3 laboratorios en C para practicar conceptos de
**fork**, **procesos** y **pipes**.\
Compilas con `make`, ejecutas con `./labN`, y limpias con `make clean`.

------------------------------------------------------------------------

  \#   Fuente         Ejecutable
  ---- -------------- ------------
  1    `labo-1.c`     `lab1`
  2    `labo-2-2.c`   `lab2`
  3    `lab-pipe.c`   `labpipe`

------------------------------------------------------------------------

Requisitos

-   `gcc`
-   `make`
-   Linux o macOS

------------------------------------------------------------------------

Compilación

``` bash
# Compilar todo
make

# Compilar solo un laboratorio
make lab1
```

------------------------------------------------------------------------

Ejecución

``` bash
./lab1
./lab2
./labpipe
```

------------------------------------------------------------------------

Limpieza

``` bash
make clean
```

------------------------------------------------------------------------

Estructura del laboratorio

    .
    ├─ labo-1.c
    ├─ labo-2-2.c
    ├─ lab-pipe.c
    ├─ Makefile
    └─ README.md

------------------------------------------------------------------------

Objetivos de `make`

``` makefile
# make        -> compila todos los laboratorios
# make labN   -> compila solo el laboratorio N
# make clean  -> elimina los binarios generados
```
