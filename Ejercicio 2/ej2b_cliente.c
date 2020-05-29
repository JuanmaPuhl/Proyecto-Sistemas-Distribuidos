#include <stdio.h>
#include "rpcprog.h"
#include <rpc/rpc.h>


int main(int argc, char **argv)
{
	CLIENT *clnt;
	char* server;
	server = argv[1];
	clnt = clnt_create(server,EJECUTARIMAGEN,PRIMERA,"tcp");
	if(clnt == (CLIENT*)NULL)
	{
			clnt_pcreateerror(server);
			exit(1);
	}
	int* resultado;
	char* argumentos[1] = {argv[2]};
	/**Variables para guardar el tiempo*/
	struct timeval begin, end;
	/**Aqui comienzo el timer*/
    gettimeofday(&begin,NULL);
	resultado = ejecutar_1(argumentos,clnt);
	if(resultado==(int*)NULL)
	{
		clnt_perror(clnt,server);
		exit(1);
	}

	if(*resultado!=0){
		printf("Ocurrio un error al ejecutar.\n");
		return -1;
	}
	gettimeofday(&end,NULL);
	printf("Crear el proceso hijo en otra pc ha tomado: %ld microsegundos\n",
		((long)1.0e6*end.tv_sec + end.tv_usec) - 
		((long)1.0e6*begin.tv_sec + begin.tv_usec));
	return 0;
}

