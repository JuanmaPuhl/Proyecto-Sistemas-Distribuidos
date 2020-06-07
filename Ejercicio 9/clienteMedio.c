#include <stdio.h>
#include "rpcprog.h"
#include <rpc/rpc.h>


int main(int argc, char **argv)
{
	CLIENT *clnt;
	char* server;
	server = argv[0];
	clnt = clnt_create(server,IMPRIMIRPANTALLA,PRIMERA,"tcp");
	if(clnt == (CLIENT*)NULL)
	{
			clnt_pcreateerror(server);
			exit(1);
	}
	int* resultado;
	char* argumentos[1] = {argv[2]};
	printf("ENTRO");
	resultado = imprimir_1(NULL,clnt);
	if(resultado==(int*)NULL)
	{
		clnt_perror(clnt,server);
		exit(1);
	}

	if(*resultado!=0){
		printf("Ocurrio un error al ejecutar.\n");
		return -1;
	}

	return 0;
}

