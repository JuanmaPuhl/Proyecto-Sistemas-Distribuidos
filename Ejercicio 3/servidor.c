#include <stdio.h>
#include "protocolo.h"

ByteArray *addbytearray_1_svc(ByteArray *msg, struct svc_req *req){
	static ByteArray to_return;
	for(int i = 0; i < 100; i++)
	{
		to_return.bytes[i*sizeof(int)] = msg->bytes[i*sizeof(int)] + msg->bytes[(i+100)*sizeof(int)];
		//printf("%d = %d\n",i*sizeof(int),to_return.bytes[i*sizeof(int)]);
	}
	return (&to_return);
}
