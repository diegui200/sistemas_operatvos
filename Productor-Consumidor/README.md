# Comunicación entre Procesos mediante FIFO (Named Pipes)
### Autor: Diego Alejandro Melgarejo Bejarano  
### Materia: Sistemas Operativos  
### Fecha: 14 de octubre de 2025  
### Tema: Comunicación entre procesos – FIFO (Named Pipes)  

---

**Objetivo General**
Implementar un modelo de comunicación entre dos procesos (Cliente y Servidor) utilizando una **tubería con nombre (FIFO)**, donde el cliente envía mensajes al servidor y este responde invirtiendo la cadena recibida.

---

**Descripción General**
El ejercicio consiste en dos programas en C que emplean mecanismos de **comunicación entre procesos (IPC)** mediante el uso de **FIFOs o Named Pipes**.  
El servidor crea la tubería y espera mensajes del cliente.  
El cliente envía cadenas de texto, el servidor las invierte y las retorna.

---

**Código del Cliente (`FIFO_CLIENT.c`)**

```c
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
   int fd;
   int end_process;
   int stringlen;
   int read_bytes;
   char readbuf[80];
   char end_str[5];

   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");

   fd = open(FIFO_FILE, O_CREAT | O_RDWR);
   strcpy(end_str, "end");

   while (1) {
      printf("Enter string: ");
      fgets(readbuf, sizeof(readbuf), stdin);
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      end_process = strcmp(readbuf, end_str);

      if (end_process != 0) {
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      } else {
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }
   }
   return 0;
}
```

---

**Código del Servidor (`FIFO_SERVER.c`)**

```c
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
   int fd;
   char readbuf[80];
   char end[10];
   int to_end;
   int read_bytes;

   mkfifo(FIFO_FILE, S_IFIFO | 0640);
   strcpy(end, "end");
   fd = open(FIFO_FILE, O_RDWR);

   while (1) {
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';
      printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

      to_end = strcmp(readbuf, end);
      if (to_end == 0) {
         close(fd);
         break;
      }

      reverse_string(readbuf);
      printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      write(fd, readbuf, strlen(readbuf));
      sleep(2);
   }

   return 0;
}

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
```

---

**Ejecución del Programa**
1. **Compilar ambos archivos:**
   ```bash
   gcc FIFO_SERVER.c -o servidor
   gcc FIFO_CLIENT.c -o cliente
   ```

2. **Ejecutar primero el servidor:**
   ```bash
   ./servidor
   ```

3. **Luego ejecutar el cliente en otra terminal:**
   ```bash
   ./cliente
   ```

4. **Intercambiar mensajes entre procesos.**
   - Escribir cadenas y ver cómo el servidor devuelve la cadena invertida.
   - Para finalizar, escribir `end` en el cliente.

---

**Conclusiones**
- El uso de **FIFOs (Named Pipes)** permite la comunicación **bidireccional entre procesos independientes**.
- Estos mecanismos son útiles para sistemas cliente-servidor simples en Linux.
- Es importante controlar el orden de apertura para evitar bloqueos en `open()`.
- Se demuestra cómo un proceso puede leer, procesar y responder información enviada por otro.
