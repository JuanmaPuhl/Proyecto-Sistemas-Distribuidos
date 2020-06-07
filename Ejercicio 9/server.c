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
	result = 0;
	return &result;
} 

