#include <stdio.h>
#include <stdlib.h>
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

	clnt = clnt_create(srv, PROY1EJ3, PROY1EJ3VERS,"tcp");

	if(clnt == (CLIENT*)NULL)
	{
		clnt_pcreateerror(srv);
		exit(2);
	}

    ByteArray *input_array = malloc(sizeof(ByteArray));
	ByteArray *output_array = malloc(sizeof(ByteArray));
	
	/*
	 * carga e impresion del arreglo solo para test
	for(int i = 0; i < 100; i++)
	{
		input_array->bytes[i*sizeof(int)] = rand() % 10;
		input_array->bytes[(i+100)*sizeof(int)] = rand() % 10;
		printf("%d + %d =\n",input_array->bytes[i*sizeof(int)],input_array->bytes[(i+100)*sizeof(int)]);
	}
	*/

	struct timeval begin, end;
	gettimeofday(&begin,NULL);

	output_array = addbytearray_1(input_array,clnt);

	gettimeofday(&end,NULL);

	unsigned int t = end.tv_usec - begin.tv_usec;
    printf("tiempo en microsegundos: %u\n",t);

	clnt_destroy(clnt);
	return 0;
}
