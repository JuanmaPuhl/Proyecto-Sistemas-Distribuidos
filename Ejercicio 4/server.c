#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#define MYPORT 14550
#define BACKLOG 10


int main(int argc, char **argv)
{
	int sockfd;
	int newfd;
	
	struct sockaddr_in
	{
		unsigned short sin_family; //Familia de la direccion
		unsigned short int sin_port; //Puerto
		struct in_addr sin_addr; //Direccion de internet
		unsigned char sin_zero[8]; //Del mismo tiempo que struct sockaddr
	};
	
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	
	socklen_t sin_size;
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
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		//Acepto la conexion del cliente
		if((newfd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size))==-1)
		{
		}
		struct timeval begin, end;
		gettimeofday(&begin,NULL); //Obtengo la hora actual
		printf("server:conexion desde:%s\n",inet_ntoa(their_addr.sin_addr));
		printf("Desde puerto:%d\n",ntohs(their_addr.sin_port));
		char buf[1]; //Mensaje recibido desde el cliente
		int numbytes = 0; //Cantidad de bytes recibidos 
		//Recibo los datos del cliente
		if((numbytes=recv(newfd,buf,1,0))==-1){
			//Si no recibo datos me quedo bloqueado hasta que llegan...
		}
		buf[numbytes]='\0';
		/*Ahora debo revisar la funcion que ingreso el usuario*/
		if(strcmp(buf,"a")==0){
			//Debo enviar 4 bytes al usuario
			char arrtosend[4];
			int i;
			for(i = 0; i < 4; i++){
				arrtosend[i] = 'a';
			}
			//Envio los datos solicitados al sv.
			if(send(newfd,arrtosend,4,0) == -1){
				perror("sendto");
				exit(EXIT_FAILURE);
			}
			gettimeofday(&end,NULL);
			printf("Enviar 4 bytes ha tomado: %ld microsegundos\n",
				((long)1.0e6*end.tv_sec + end.tv_usec) - 
				((long)1.0e6*begin.tv_sec + begin.tv_usec));
		}
		else
		if(strcmp(buf,"b")==0){
			//Debo enviar 2048 bytes al usuario.
			char arrtosend[2048];
			int i;
			for(i = 0; i < 2048; i++){
				arrtosend[i] = 'a';
			}
			//Envio el dato solicitado al usuario.
			if(send(newfd,arrtosend,2048,0) == -1){
				perror("sendto");
				exit(EXIT_FAILURE);
			}
			gettimeofday(&end,NULL);
			printf("Enviar 2048 bytes ha tomado: %ld microsegundos\n",
				((long)1.0e6*end.tv_sec + end.tv_usec) - 
				((long)1.0e6*begin.tv_sec + begin.tv_usec));
		}
		close(newfd);
	}
	return 0;
}

