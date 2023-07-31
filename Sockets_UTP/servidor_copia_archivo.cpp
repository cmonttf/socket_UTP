#include <stdio.h> /* Éste es el código del servidor */
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>


#define SERVER_PORT 12345 /* arbitrario, pero el cliente y el servidor deben coincidir */
#define BUF_SIZE 4096 /* tamaño de bloque para la transferencia */
#define QUEUE_SIZE 10


int main(int argc, char *argv[])
{

socklen_t destino_tam;
int s, b, l, fd, sa, bytes, on = 1;
char buf[BUF_SIZE]; /* búfer para el archivo saliente */
struct sockaddr_in channel, destino_dir; /* contiene la dirección IP */

/* Construye la estructura de la dirección para enlazar el socket. */
memset(&channel, 0, sizeof(channel)); /* llena con cero la estructura */
channel.sin_family = AF_INET;
channel.sin_addr.s_addr = htonl(INADDR_ANY);
channel.sin_port = htons(SERVER_PORT);

/* Apertura pasiva. Espera una conexión. */
s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* crea el socket */
if (s < 0) printf("falla en socket \n");
//setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
if (b < 0) printf("falla en bind \n"); 

l = listen(s, QUEUE_SIZE); /* especifica el tamaño de la cola */
if (l < 0) printf("falla en listen \n"); 

/* El socket ahora está configurado y enlazado. Espera una conexión y la procesa. */
while (1) {
destino_tam=sizeof(destino_dir);
sa = accept(s, (struct sockaddr*)&destino_dir, &destino_tam); /* se bloquea para la solicitud de conexión */
if (sa < 0) printf("falla en accept \n"); 
printf("***Servidor se conecto con el cliente: %d.\n",destino_dir.sin_addr.s_addr);

read(sa, buf, BUF_SIZE); /* lee el nombre del archivo desde el socket */
/* Obtiene y regresa el archivo.*/

fd = open(buf, O_RDONLY); /* abre el archivo para regresarlo */
if (fd < 0) printf("falla en open \n");

while (1) {
bytes = read(fd, buf, BUF_SIZE); /* lee del archivo */
if (bytes <= 0) break; /* verifica el final del archivo */
write(sa, buf, bytes); /* escribe bytes en el socket */
}
close(fd); /* cierra el archivo */
close(sa); /* cierra la conexión */
}

}

