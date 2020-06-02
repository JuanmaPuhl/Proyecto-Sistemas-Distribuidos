#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define PORT 14550 //Puerto
#define MAXDATASIZE 2048 //Maximos datos del paquete


/**En este caso la comunicacion es orientada a sockets con conexion*/
int main(int argc, char **argv)
{
	int sockfd,numbytes;
	struct hostent *he;
	struct sockaddr_in their_addr;
	char* funcion;
	//Reviso los parametros
	/*La llamada debera ser:
	 * ./cliente host-ip a/b
	 * siendo a y b las opciones para las funciones.
	 * a es la funcion que pide 4 bytes
	 * b es la funcion que pide 2048 bytes*/
	if(argc != 3){
		printf("Error con los argumentos.\n");
		exit(-1);
	}
	//Reviso el host
	if((he=gethostbyname(argv[1]))==NULL){
		printf("Error: Hostname invalido");
		exit(-1);
	}
	funcion = argv[2];
	//Ahora abro el socket
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("Error: Ocurrio un problema al abrir el socket");
		exit(-1);
	}
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);
	bzero(&(their_addr.sin_zero),8);
	
	//Voy a conectarme con el server
	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr))==-1){
	}
	
	/*Ahora debo revisar la funcion que ingreso el usuario*/
	if(strcmp(funcion,"a")==0){
		char recibido[4];
		//Envio la funcion solicitada al sv.
		if(send(sockfd,"a",1,0) == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		//Recibo los datos
		if((numbytes=recv(sockfd,recibido,4,0))==-1){
			//Si no recibo datos me quedo bloqueado hasta que llegan...
		}
		recibido[numbytes]='\0';
		printf("Recibido %d bytes\n",numbytes);
	}
		if(strcmp(funcion,"b")==0){
		char recibido[2048];
		//Envio la funcion solicitada al sv.
		if(send(sockfd,"b",1,0) == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		//Recibo los datos
		if((numbytes=recv(sockfd,recibido,2048,0))==-1){
			//Si no recibo datos me quedo bloqueado hasta que llegan...
		}
		recibido[numbytes]='\0';
		printf("Recibido %d bytes\n",numbytes);
	}
	close(sockfd);
	return 0;
}

