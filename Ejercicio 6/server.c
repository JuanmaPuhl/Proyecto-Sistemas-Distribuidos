#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include "header.h"

#define MYPORT 14550
#define BACKLOG 10
#define MAXDATASIZE 128 //Maximos datos del paquete

/*
 * Este server hace de intermediario entre las dos agencias. 
 * Cuando llega un mensaje de pedido entonces lo redirijo a la otra agencia.
 * Los pedidos a funciones que esten en la misma agencia no seran recibidos, se
 * procesan en el cliente.
 * */
socklen_t sin_size;
void procesar(int socketEmisor, int socketReceptor)
{
	while(1){
		Cabecera* cabecera = malloc(sizeof(Cabecera)); //Obtengo el mensaje
		if(recv(socketEmisor,cabecera,sizeof(Cabecera),MSG_PEEK)==-1)
		{
		}
		int tipo = cabecera->identificador;
		if(tipo == IdpedirFuncion) //Si el cliente pide una funcion
		{
			recv(socketEmisor,cabecera,sizeof(Cabecera),0);
			Funcion* funcion = malloc(sizeof(Funcion));
			//Debo obtener que funcion va a ejecutar
			if(recv(socketEmisor,funcion,sizeof(Funcion),0)==-1)
			{
			}
			//Debo enviarle el pedido al otro cliente.
			cabecera->identificador = Idpedirdato;
			//Envio la cabecera
			if(send(socketReceptor,cabecera,sizeof(Cabecera),0) == -1){
				perror("sendto");
				exit(EXIT_FAILURE);
			}
			//Ahora debo enviar la funcion que necesito
			if(send(socketReceptor,funcion,sizeof(Funcion),0) == -1){
				perror("sendto");
				exit(EXIT_FAILURE);
			}
			//Tengo que esperar la respuesta del cliente
			if(recv(socketReceptor,cabecera,sizeof(Cabecera),0)==-1)
			{
				perror("recv");
			}
			tipo = cabecera->identificador;
			if(tipo == Iddardatos) //Recibi los datos que necesitaba
			{
				Datos* datos = malloc(sizeof(Datos));
				if(recv(socketReceptor,datos,sizeof(Datos),0)==-1)
				{
				}
				cabecera->identificador = Iddardatos; //Ahora le envio los datos solicitados al emisor
				//Envio la cabecera
				if(send(socketEmisor,cabecera,sizeof(Cabecera),0) == -1){
					perror("sendto");
					exit(EXIT_FAILURE);
				}
				//Ahora debo enviar los datos solicitados
				if(send(socketEmisor,datos,sizeof(Datos),0) == -1){
					perror("sendto");
					exit(EXIT_FAILURE);
				}
			}
		}
		else
		{
			if(tipo == IdCerrar) //El mensaje que llego indica que el cliente emisor quiere cerrar la comunicacion
			{
				//Debo enviar al receptor que voy a cerrar el canal.
				cabecera->identificador = IdCerrar;
				//Envio la cabecera, no es necesario enviar mas nada
				if(send(socketReceptor,cabecera,sizeof(Cabecera),0) == -1){
					perror("sendto");
					exit(EXIT_FAILURE);
				}
				//Cierro los dos fd que habia abierto
				close(socketEmisor);
				close(socketReceptor);
			}
		}
	}
	
}


int main(int argc, char **argv)
{
	int sockfd;
	//int newfd;
	
	struct sockaddr_in
	{
		unsigned short sin_family; //Familia de la direccion
		unsigned short int sin_port; //Puerto
		struct in_addr sin_addr; //Direccion de internet
		unsigned char sin_zero[8]; //Del mismo tiempo que struct sockaddr
	};
	
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	
	
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Ocurrio un error al iniciar el socket");
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1)
	{
		
	}
	//Escucho conexiones entrantes
	if(listen(sockfd,BACKLOG) == -1)
	{
	}
	printf("Entre.\n");
	/*Acepto las dos conexiones de las agencias, no importa el orden*/
	int fd1 = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size);
	printf("server:conexion desde:%s\n",inet_ntoa(their_addr.sin_addr));
	printf("Desde puerto:%d\n",ntohs(their_addr.sin_port));
	int fd2 = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size);
	printf("server:conexion desde:%s\n",inet_ntoa(their_addr.sin_addr));
	printf("Desde puerto:%d\n",ntohs(their_addr.sin_port));
	//Creo un nuevo proceso. Cada proceso manejara una agencia.
	if(!fork())
	{
		procesar(fd1,fd2);
	}
	else
	{
		procesar(fd2,fd1);
	}
	/**Cuando entro aca tengo que cerrar las conexiones con las dos agencias*/
	
	return 0;
}

