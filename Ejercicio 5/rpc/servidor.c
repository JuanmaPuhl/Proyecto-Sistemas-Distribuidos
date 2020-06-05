#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "protocolo.h"

unsigned long int *decabin_1_svc(int *nro, struct svc_req *req)
{
	static unsigned long int result;
	result = 0;
	int rem, temp = 1;
	while (*nro > 0) { 
		rem = *nro%2;
		*nro = *nro / 2;
		result = result + rem*temp;
		temp = temp * 10; 
	} 
	return(&result);
}

String *binahex_1_svc(unsigned long int *bin, struct svc_req *req)
{
	static String result;
	long int binaryval, hexadecimalval = 0, i = 1, remainder;
	while ((*bin) != 0)
	{
		remainder = (*bin) % 10;
		hexadecimalval = hexadecimalval + remainder * i;
		i = i * 2;
		(*bin) = (*bin) / 10;
	}
	sprintf(result.val,"%lX",hexadecimalval);
	return(&result);
}

int *suma_1_svc(Cuadrupla *op, struct svc_req *req)
{
	static int result;
	result = op->operandos[0] + op->operandos[1] + op->operandos[2] + op->operandos[3];
	return(&result);
}

int *resta_1_svc(Cuadrupla *op, struct svc_req *req)
{	
	static int result;
	
	result = op->operandos[0] - op->operandos[1] - op->operandos[2] - op->operandos[3];
	
	return(&result);
}

int *multiplicacion_1_svc(Dupla *op, struct svc_req *req)
{
	static int result;
	result = op->a * op->b;
	return(&result);
}

Double *division_1_svc(Dupla *op, struct svc_req *req)
{
	static Double result;
	result.val = (double)op->a / (double)op->b;
	return(&result);
}
