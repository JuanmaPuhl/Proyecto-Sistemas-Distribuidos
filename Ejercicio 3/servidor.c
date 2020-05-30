#include <stdio.h>
#include "protocolo.h"

ByteArray *addbytearray_1_svc(ByteArray *msg, struct svc_req *req){
	static ByteArray to_return;
	for(int i = 0; i < 1; i++)
	{
		to_return.bytes[i] = msg->bytes[i] + msg->bytes[i+100];
	}
	return (&to_return);
}
