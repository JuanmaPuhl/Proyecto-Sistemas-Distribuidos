#include <stdio.h>
#include <sys/time.h>
#include "protocolo.h"

MiTimeval *localtime_1_svc(void *argp, struct svc_req *rqstp)
{	
	struct timeval t;
	gettimeofday(&t,NULL);
	static MiTimeval result;
	result.tv_sec = t.tv_sec;
	result.tv_usec = t.tv_usec;
	return &result;
}

void *ping_1_svc(void *argp, struct svc_req *rqstp)
{
}
