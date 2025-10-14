/**************************************************************************************
 * Fecha: 14 de octubre de 2025
 * Autor: Diego Alejandro Melgarejo Bejarano
 * Materia: Sistemas Operativos
 * Tema: Comunicación entre procesos mediante FIFO (Named Pipes)
 * Objetivo: Implementar un proceso SERVIDOR que se comunique con un CLIENTE a través 
 *            de una tubería con nombre (FIFO) en el sistema operativo Linux.
 *
 * Descripción general:
 *  - El servidor crea la tubería con nombre en "/tmp/fifo_twoway" utilizando `mkfifo()`.
 *  - Espera recibir cadenas enviadas por el cliente mediante `read()`.
 *  - Al recibir una cadena, verifica si es la palabra “end” para finalizar.
 *  - Si no lo es, invierte el contenido de la cadena y la envía de regreso al cliente 
 *    mediante `write()`.
 *  - Se utiliza la función auxiliar `reverse_string()` para invertir la cadena recibida.
 *
 * Estructura del programa:
 *  - Creación del FIFO:
 *      - Uso de `mkfifo()` para crear la tubería si no existe.
 *  - Apertura del FIFO:
 *      - Se abre en modo lectura/escritura (O_RDWR).
 *  - Ciclo de comunicación Servidor ↔ Cliente:
 *      - El servidor lee la cadena enviada por el cliente.
 *      - Si la cadena es distinta de “end”, la invierte y la reenvía.
 *      - Si la cadena es “end”, el servidor cierra el FIFO y termina la ejecución.
 *  - Función auxiliar `reverse_string()`:
 *      - Invierte los caracteres de una cadena de texto.
 *
 * Consideraciones:
 *  - El servidor debe ejecutarse antes que el cliente para que la tubería exista.
 *  - `sleep(2)` se usa para evitar que el servidor lea sus propios mensajes antes 
 *    de que el cliente los reciba.
 **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_FILE "/tmp/fifo_twoway"

void reverse_string(char *);

int main() {
   int fd;                 // Descriptor de archivo para el FIFO
   char readbuf[80];       // Buffer de lectura/escritura
   char end[10];           // Palabra clave para finalizar (“end”)
   int to_end;             // Bandera para detectar el fin
   int read_bytes;         // Cantidad de bytes leídos

   // Crea el FIFO si no existe (modo lectura/escritura para propietario y grupo)
   mkfifo(FIFO_FILE, S_IFIFO | 0640);

   // Copia la palabra “end” en la variable de control
   strcpy(end, "end");

   // Abre el FIFO en modo lectura y escritura
   fd = open(FIFO_FILE, O_RDWR);

   // Ciclo principal de comunicación
   while (1) {
      // Lee el mensaje enviado por el cliente
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';
      printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

      // Verifica si el mensaje recibido es “end”
      to_end = strcmp(readbuf, end);
      if (to_end == 0) {
         close(fd);   // Cierra el FIFO
         break;       // Sale del bucle
      }

      // Invierte la cadena recibida
      reverse_string(readbuf);

      // Muestra y envía la cadena invertida de regreso al cliente
      printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      write(fd, readbuf, strlen(readbuf));

      // Pausa breve para sincronizar la lectura entre procesos
      sleep(2);
   }

   return 0;
}

// Función para invertir una cadena de texto
void reverse_string(char *str) {
   int last, limit, first;
   char temp;

   last = strlen(str) - 1;
   limit = last / 2;
   first = 0;

   while (first < last) {
      temp = str[first];
      str[first] = str[last];
      str[last] = temp;
      first++;
      last--;
   }
   return;
}
