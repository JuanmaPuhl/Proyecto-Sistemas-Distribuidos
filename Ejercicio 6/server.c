#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include "header.h"
#include <signal.h>

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
int quienPide = -1;
int funcionAPedir = -1;

int comunicar(int emisor, int receptor, int fn)
{
	printf("Entre a comunicar.\n");
	quienPide = 0; //Atiendo los mensajes
	funcionAPedir = -1; //Atiendo los mensajes
	/*Preparo el paquete a enviar*/
	Cabecera* cabecera = malloc(sizeof(Cabecera));
	Funcion* funcion = malloc(sizeof(Funcion));
	cabecera->identificador = Idpedirdato;
	funcion->Idfuncion = fn;
	printf("La funcion pedida es: %d.\n",fn);
	//Envio la cabecera
	if(send(receptor,cabecera,sizeof(Cabecera),0) == -1){
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	printf("Ya envie la cabecera a fd2.\n");
	//Ahora debo enviar la funcion que necesito
	if(send(receptor,funcion,sizeof(Funcion),0) == -1){
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	printf("Ya envie la funcion a fd2.\n");
	//Tengo que esperar la respuesta del cliente
	if(recv(receptor,cabecera,sizeof(Cabecera),0)==-1)
	{
		perror("recv");
	}
	printf("Recibi la cabecera desde fd2.\n");
	if(cabecera->identificador == IdpedirFuncion) //Recibi un mensaje de nuevo pedido
	{
		printf("fd2 me quiere pedir datos.\n");
		//Lo guardo para procesar mas tarde
		quienPide = receptor;
		//Recibo tambien la funcion pedida
		if(recv(receptor,funcion,sizeof(Funcion),0)==-1)
		{
			perror("recv");
		}
		funcionAPedir = funcion->Idfuncion;
		//Recibo la siguiente cabecera
		if(recv(receptor,cabecera,sizeof(Cabecera),0)==-1)
		{
			perror("recv");
		}
	}
	if(cabecera->identificador == Iddardatos) //El receptor me manda los datos
	{
		printf("Fd2 me quiere dar datos.\n");
		Datos* datos = malloc(sizeof(Datos));
		//Recibo los datos
		if(recv(receptor,datos,sizeof(Datos),0)==-1)
		{
			perror("recv");
		}
		printf("Recibo los datos de fd2.\n");
		//Ahora envio todo al emisor
		cabecera->identificador = Iddardatos;
		//Envio la cabecera
		if(send(emisor,cabecera,sizeof(Cabecera),0) == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		printf("Envie la cabecera a fd2.\n");
		//Ahora debo enviar los datos
		if(send(emisor,datos,sizeof(Datos),0) == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		printf("Envie datos a fd2.\n");
	}
	if(cabecera->identificador == IdCerrar)
	{
		printf("Tengo que cerrar.\n");
		//Tengo que avisar que cierro todo.
		//Envio la cabecera
		if(send(emisor,cabecera,sizeof(Cabecera),0) == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		return -1;
	}
	return 0;
}


void recibirMensajes(int fd1, int fd2)
{
	while(1)
	{
		Cabecera* cabecera = malloc(sizeof(Cabecera)); //Creo la cabecera que voy a recibir
		cabecera->identificador = -1;
		if(funcionAPedir == -1) //Si no habia ningun pedido encolado
		{
			/*Primero miro pedido en fd1*/
			if(recv(fd1,cabecera,sizeof(Cabecera),MSG_DONTWAIT)==-1) //Miro pero no me quedo bloqueado
			{
			}
			if(cabecera->identificador != -1)
			{
				printf("Recibi un mensaje de fd1.\n");
				if(cabecera->identificador == IdpedirFuncion)
				{
					Funcion* funcion = malloc(sizeof(Funcion));
					if(recv(fd1,funcion,sizeof(Funcion),0)==-1) //Espero a recibir la funcion
					{
					}
					printf("Recibi funcion de fd1.\n");
					//Fd1 le pide a fd2 una funcion
					int i = 0;
					i = comunicar(fd1,fd2,funcion->Idfuncion);
					if(i==-1)
					{
						break;
					}
				}
				if(cabecera->identificador == IdCerrar)
				{
					printf("Tengo que cerrar.\n");
					//Tengo que avisar que cierro todo.
					//Envio la cabecera
					if(send(fd2,cabecera,sizeof(Cabecera),0) == -1){
						perror("sendto");
						exit(EXIT_FAILURE);
					}
					break;
				}
			}
			else
			{
				/*Miro pedido en fd2*/
				if(recv(fd2,cabecera,sizeof(Cabecera),MSG_DONTWAIT)==-1) //Miro pero no me quedo bloqueado
				{
				}
				if(cabecera->identificador != -1)
				{
					printf("Recibi un mensaje de fd2.\n");
					if(cabecera->identificador == IdpedirFuncion)
					{
						Funcion* funcion = malloc(sizeof(Funcion));
						if(recv(fd2,funcion,sizeof(Funcion),0)==-1) //Espero a recibir la funcion
						{
						}
						//Fd2 le pide a fd1 una funcion
						printf("Recibi funcion de fd2.\n");
						int i = 0;
						i = comunicar(fd2,fd1,funcion->Idfuncion);
						if(i==-1)
						{
							break;
						}
					}
					if(cabecera->identificador == IdCerrar)
					{
						printf("Tengo que cerrar.\n");
						//Tengo que avisar que cierro todo.
						//Envio la cabecera
						if(send(fd1,cabecera,sizeof(Cabecera),0) == -1){
							perror("sendto");
							exit(EXIT_FAILURE);
						}
						break;
					}
						
				}	
			}
		}
		else
		{
			if(quienPide == fd1)
				comunicar(fd1,fd2,funcionAPedir);
			if(quienPide == fd2)
				comunicar(fd2,fd1,funcionAPedir);
		}
		
	}
	
	
}


void procesar(int socketEmisor, int socketReceptor)
{
	while(1){ //Ejecuto siempre
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
				printf("Cierro.\n");
				//Debo enviar al receptor que voy a cerrar el canal.
				cabecera->identificador = IdCerrar;
				//Envio la cabecera, no es necesario enviar mas nada
				if(send(socketReceptor,cabecera,sizeof(Cabecera),0) == -1){
					perror("sendto");
					exit(EXIT_FAILURE);
				}
				break;
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
	//Este fd seria para la primera agencia que se conecto
	int fd1 = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size);
	printf("server:conexion desde:%s\n",inet_ntoa(their_addr.sin_addr));
	printf("Desde puerto:%d\n",ntohs(their_addr.sin_port));
	//Este fd seria para la segunda agencia que se conecto
	int fd2 = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size);
	printf("server:conexion desde:%s\n",inet_ntoa(their_addr.sin_addr));
	printf("Desde puerto:%d\n",ntohs(their_addr.sin_port));

	recibirMensajes(fd1,fd2);
	//Si entro aca es porque recibio IdCerrar
	close(fd1);
	close(fd2);	
	return 0;
}

