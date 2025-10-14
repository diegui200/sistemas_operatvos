/**************************************************************************************
 * Fecha: 14 de octubre de 2025
 * Autor: Diego Alejandro Melgarejo Bejarano
 * Materia: Sistemas Operativos
 * Tema: Comunicación entre procesos mediante FIFO (Named Pipes)
 * Objetivo: Implementar un proceso CLIENTE que se comunique con otro proceso SERVIDOR 
 *            a través de una tubería con nombre (FIFO). 
 * Descripción general:
 *  - El cliente envía cadenas de texto al servidor de manera continua hasta que se escriba "end".
 *  - Utiliza funciones del sistema para lectura y escritura a través del FIFO compartido.
 *  - La comunicación es bidireccional: el cliente envía una cadena y luego espera la respuesta.
 *  - Se emplea la función `open()` para abrir la tubería, `write()` para enviar datos 
 *    y `read()` para recibir la respuesta del servidor.
 *
 * Estructura del programa:
 *  - Definición de constantes y variables:
 *      - Se define el nombre del archivo FIFO en "/tmp/fifo_twoway".
 *      - Se crean variables para manejar la lectura y escritura.
 *  - Apertura del FIFO:
 *      - Se abre o crea la tubería en modo lectura/escritura (O_CREAT | O_RDWR).
 *  - Comunicación Cliente → Servidor:
 *      - Se solicita al usuario ingresar una cadena.
 *      - Se envía la cadena al servidor mediante `write()`.
 *      - Se recibe la respuesta mediante `read()` y se muestra por consola.
 *  - Condición de finalización:
 *      - Si el usuario ingresa la palabra “end”, el cliente envía este mensaje al servidor
 *        y finaliza el proceso cerrando el descriptor del FIFO.
 *
 * Consideraciones:
 *  - El cliente debe ejecutarse después de que el servidor haya creado el FIFO.
 *  - Si el servidor no está activo, la llamada a `open()` puede quedar bloqueada hasta que
 *    el FIFO sea abierto por el otro extremo.
 **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
   int fd;                  // Descriptor de archivo para el FIFO
   int end_process;         // Variable de control para terminar el ciclo
   int stringlen;           // Longitud de la cadena ingresada
   int read_bytes;          // Bytes leídos desde el FIFO
   char readbuf[80];        // Buffer para almacenar cadenas
   char end_str[5];         // Palabra clave para finalizar (“end”)

   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");

   // Apertura o creación del FIFO para lectura y escritura
   fd = open(FIFO_FILE, O_CREAT | O_RDWR);

   // Copia la palabra “end” a la variable de comparación
   strcpy(end_str, "end");

   // Ciclo principal de comunicación
   while (1) {
      printf("Enter string: ");
      fgets(readbuf, sizeof(readbuf), stdin);     // Lee la cadena ingresada por el usuario
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';              // Elimina el salto de línea
      end_process = strcmp(readbuf, end_str);     // Compara si la entrada es “end”

      // Si la cadena no es "end", se envía al servidor
      if (end_process != 0) {
         write(fd, readbuf, strlen(readbuf));     // Envía la cadena por el FIFO
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));

         // Espera y recibe la respuesta del servidor
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      } else {
         // Si se ingresa “end”, se envía al servidor y se termina
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         close(fd); // Cierra el descriptor del FIFO
         break;
      }
   }

   return 0;
}

