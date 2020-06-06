#include <stdio.h>
#include "protocolo.h"

Mensaje *dectobin_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	Mensaje *binario = malloc(sizeof(Mensaje));
	int rem, temp = 1;
	int resultado = 0;
	while (msg->datos[0] > 0) { 
		rem = msg->datos[0]%2;
		msg->datos[0] = msg->datos[0] / 2;
		binario->datos[0] = binario->datos[0] + rem*temp;
		temp = temp * 10; 
	} 
	to_return = *binario;
	return (&to_return);
	
}

char **bintohex_1_svc(Mensaje *msg, struct svc_req *req)
{
	static char *to_return;
	to_return = malloc(8*sizeof(char));
	
	int binaryval = msg->datos[0];
	int hexadecimalval = 0;
	int i = 1;
	int remainder;
	while(binaryval != 0)
	{
		remainder = binaryval % 10;
		hexadecimalval = hexadecimalval + remainder * i;
		i = i * 2;
		binaryval = binaryval / 10;
	}
	sprintf(to_return,"%X",hexadecimalval);
	return (&to_return);
}

Mensaje *suma_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	Mensaje *suma = malloc(sizeof(Mensaje));
	for(int i = 0; i < 4; i++)
	{
		suma->datos[0] += msg->datos[i];
	}
	to_return = *suma;
	return (&to_return);
}

Mensaje *resta_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	Mensaje *resta = malloc(sizeof(Mensaje));
	resta->datos[0] = msg->datos[0];
	for(int i = 1; i < 4; i++)
	{
		resta->datos[0] -= msg->datos[i];
	}
	to_return = *resta;
	return (&to_return);
}

Mensaje *multiplicacion_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;	
	Mensaje *mult = malloc(sizeof(Mensaje));
	mult->datos[0] = msg->datos[0] * msg->datos[1];
	to_return = *mult;
	return (&to_return);
}

Mensaje *division_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	Mensaje *div = malloc(sizeof(Mensaje));
	div->datos[0] = msg->datos[0] / msg->datos[1];
	div->datos[1] = msg->datos[0] % msg->datos[1];
	to_return = *div;
	return (&to_return);
}
