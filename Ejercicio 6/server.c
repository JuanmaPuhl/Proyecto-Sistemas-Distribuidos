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
//Estas dos variables sirven para guardar hasta un mensaje de pedido de funcion.
int quienPide = -1; 
int funcionAPedir = -1;

/*Metodo auxiliar para cerrar el server y otra agencia*/
void cerrarComunicacion(int emisor, int receptor)
{
	//Preparo el mensaje
	Cabecera* cabecera = malloc(sizeof(Cabecera));
	cabecera->identificador = IdCerrar;
	printf("Cerrando sv.\n");
	//Tengo que avisar que cierro todo.
	//Envio la cabecera
	if(send(receptor,cabecera,sizeof(Cabecera),0) == -1){
		perror("sendto");
		exit(EXIT_FAILURE);
	}
}

/*Metodo auxiliar para recibir y enviar mensajes entre el sv y las agencias*/
int comunicar(int emisor, int receptor, int fn)
{
	quienPide = 0; //Atiendo los mensajes
	funcionAPedir = -1; //Atiendo los mensajes
	/*Preparo el paquete a enviar*/
	Cabecera* cabecera = malloc(sizeof(Cabecera));
	Funcion* funcion = malloc(sizeof(Funcion));
	cabecera->identificador = Idpedirdato;
	funcion->Idfuncion = fn;
	//Envio la cabecera
	if(send(receptor,cabecera,sizeof(Cabecera),0) == -1){
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	//Ahora debo enviar la funcion que necesito
	if(send(receptor,funcion,sizeof(Funcion),0) == -1){
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	//Tengo que esperar la respuesta de la agencia
	if(recv(receptor,cabecera,sizeof(Cabecera),0)==-1)
	{
		perror("recv");
	}
	//Recibi un mensaje, ahora tengo que ver de que tipo es.
	if(cabecera->identificador == IdpedirFuncion) //Recibi un mensaje de nuevo pedido
	{
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
	if(cabecera->identificador == Iddardatos) //El receptor me manda los datos que le pedi
	{
		Datos* datos = malloc(sizeof(Datos));
		//Recibo los datos
		if(recv(receptor,datos,sizeof(Datos),0)==-1)
		{
			perror("recv");
		}
		//Ahora envio todo al emisor
		cabecera->identificador = Iddardatos;
		//Envio la cabecera
		if(send(emisor,cabecera,sizeof(Cabecera),0) == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		//Ahora debo enviar los datos
		if(send(emisor,datos,sizeof(Datos),0) == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
	}
	if(cabecera->identificador == IdCerrar) //El receptor me envio un mensaje de cierre
	{
		cerrarComunicacion(emisor,receptor);
		return -1;
	}
	return 0;
}

/*Metodo auxiliar para revisar si que tipo de mensaje llego e iniciar la comunicacion
 * en el caso correspondiente*/
int iniciarPedido(int emisor, int receptor, Cabecera* cabecera)
{
	if(cabecera->identificador == IdpedirFuncion)
	{
		Funcion* funcion = malloc(sizeof(Funcion));
		if(recv(emisor,funcion,sizeof(Funcion),0)==-1) //Espero a recibir la funcion
		{
		}
		//Fd1 le pide a fd2 una funcion
		int i = 0;
		i = comunicar(emisor,receptor,funcion->Idfuncion);
		if(i==-1)
		{
			return -1;
		}
	}
	if(cabecera->identificador == IdCerrar)
	{
		cerrarComunicacion(emisor,receptor);
		return -1;
	}
	return 0;
}


/*Metodo para revisar si alguien pidio algo*/
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
				int r = iniciarPedido(fd1,fd2,cabecera);
				if(r == -1) //Tengo que cerrar el sv, entonces corto el while
					break;
			}
			else
			{
				/*Miro pedido en fd2*/
				if(recv(fd2,cabecera,sizeof(Cabecera),MSG_DONTWAIT)==-1) //Miro pero no me quedo bloqueado
				{
				}
				if(cabecera->identificador != -1)
				{
					int r = iniciarPedido(fd2,fd1,cabecera);
					if(r == -1) //Tengo que cerrar el sv, entonces corto el while
						break;
				}	
			}
		}
		else //Tengo que procesar los pedidos guardados
		{
			if(quienPide == fd1)
				comunicar(fd1,fd2,funcionAPedir);
			if(quienPide == fd2)
				comunicar(fd2,fd1,funcionAPedir);
		}	
	}
}


int main(int argc, char **argv)
{
	int sockfd;
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
	/*Acepto las dos conexiones de las agencias, no importa el orden*/
	//Este fd seria para la primera agencia que se conecto
	int fd1 = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size);
	printf("Se conecto la primer agencia.\n");
	//Este fd seria para la segunda agencia que se conecto
	int fd2 = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size);
	printf("Se conecto la segunda agencia.\n");
	recibirMensajes(fd1,fd2);
	//Si entro aca es porque recibio IdCerrar
	close(fd1); //Cierro los descriptores de archivo.
	close(fd2);	
	return 0;
}

