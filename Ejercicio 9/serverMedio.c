#include <stdio.h>
#include "rpcprog.h"
#include <rpc/rpc.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

extern int errno;

int *imprimir_1_svc(void* arg,struct svc_req *req)
{
	static int  result;
	printf("HOLA.\n");
	char* arr[1] = {"192.168.1.109"};
	char* arr2[1] = {"192.168.1.103"};
	int i;
	if(!fork())
	{
		
	execve("clienteMedio",arr,NULL);}
	else{
		
	execve("clienteMedio",arr2,NULL);
	}
	
	printf("Ya me comunique con ese sv.\n");
	result = 0;
	return &result;
} 

