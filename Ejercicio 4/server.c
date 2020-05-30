#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

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
	
	int sin_size;
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
	if(listen(sockfd,BACKLOG) == -1)
	{
	}
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if((newfd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size))==-1)
		{
		}
		printf("server:conexion desde:%s\n",inet_ntoa(their_addr.sin_addr));
		printf("Desde puerto:%d\n",ntohs(their_addr.sin_port));
		if(!fork())
		{
			if(send(newfd,"Hello, world!\n",14,0)==-1)
				perror("send");
			close(newfd);
			exit(0);
		}
		close(newfd);
	}
	return 0;
}

