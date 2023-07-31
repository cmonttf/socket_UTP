/* Esta página contiene un programa cliente que puede solicitar un archivo desde el programa servidor de la siguiente
página. El servidor responde
* enviando el archivo completo.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>


#define SERVER_PORT 12345 /* arbitrario, pero el cliente y el servidor deben coincidir */
#define BUF_SIZE 4096 /* tamaño de bloque para transferencia */


int main(int argc, char **argv)
{

int c, s, bytes; // variables auxiliares para registrar errores
char buf[BUF_SIZE]; /* búfer para el archivo entrante */
struct sockaddr_in channel; /* contiene la dirección IP */

if (argc != 3) printf("Usar: cliente nombre-servidor nombre-archivo \n"); 

s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
if (s < 0) printf("falla en socket \n"); 

//Se prepara la dirección de la máquina servidora
channel.sin_family = AF_INET;
channel.sin_addr.s_addr = inet_addr(argv[1]);
channel.sin_port = htons(SERVER_PORT);


c = connect(s, (struct sockaddr *) &channel, sizeof(channel));
if (c < 0) printf("falla en conexion \n"); 

/* Se ha establecido la conexión. Se envía el nombre del archivo incluyendo el byte 0 al final. */
write(s, argv[2], strlen(argv[2])+1);

/* Obtiene el archivo y lo escribe en la salida estándar. */
while (1) {
bytes = read(s, buf, BUF_SIZE); /* lee del socket */
if (bytes <= 0) exit(0); /* verifica el final del archivo */
write(1, buf, bytes); /* escribe en la salida estándar */
}

}


