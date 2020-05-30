#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define PORT 14550
#define MAXDATASIZE 100

int main(int argc, char **argv)
{
	int sockfd,numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr;
	if(argc != 2){
	}
	if((he=gethostbyname(argv[1]))==NULL){
	}
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
	}
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);
	bzero(&(their_addr.sin_zero),8);
	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr))==-1){
	}
	if((numbytes=recv(sockfd,buf,MAXDATASIZE,0))==-1){
	}
	buf[numbytes]='\0';
	printf("Recibido:%s\n",buf);
	close(sockfd);
	return 0;
}

