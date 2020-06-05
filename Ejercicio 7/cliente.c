#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "protocolo.h"

int main(int argc, char *argv[])
{
	char *srv;
	CLIENT *clnt;

	if(argc != 2)
	{
		printf("El argumento deben ser <ip>\n");
		exit(1);
	}

	srv = argv[1];

	clnt = clnt_create(srv, PROY1EJ7, PROY1EJ7VERS,"tcp");

	if(clnt == (CLIENT*)NULL)
	{
		clnt_pcreateerror(srv);
		exit(2);
	}
	
	//calculamos una latencia promedio al servidor
	//esta latencia es ida+vuelta
	struct timeval begin, end;
	long int latencia = 0;
	for(int i = 0; i < 5; i++)
	{
		gettimeofday(&begin,NULL);
		ping_1(NULL,clnt);
		gettimeofday(&end,NULL);
		latencia += end.tv_usec - begin.tv_usec;
	}
	latencia = latencia/5;
	printf("latencia en microsegundos: %u\n",latencia);
	//probemos el desfasaje de relojes
	//pedimos el tiempo del servidor y le restamos la latencia/2
	struct timeval localclock;
	MiTimeval *serverclock;
	gettimeofday(&localclock,NULL);
	serverclock = localtime_1(NULL,clnt);
	long int timedrift = (serverclock->tv_usec - localclock.tv_usec) - latencia/2;
	printf("deriva de reloj en microsegundos: %ld\n",timedrift);
	clnt_destroy(clnt);
	return 0;
}
