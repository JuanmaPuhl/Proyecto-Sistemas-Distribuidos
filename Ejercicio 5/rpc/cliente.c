#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "protocolo.h"

void texto()
{
	printf("Seleccione una opcion:\n");
	printf("1) Conversion de numero decimal a binario.\n");
	printf("2) Conversion de numero binario a hexadecimal.\n");
	printf("3) Suma de hasta cuatro operandos.\n");
	printf("4) Resta de hasta cuatro operandos.\n");
	printf("5) Multiplicacion de dos operandos.\n");
	printf("6) Division de hasta dos operandos.\n");
}

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

	int opcion;
	printf("Mini Operaciones\n");
	printf("-----------------\n");
	texto();
	fflush(stdin);
	scanf("%d",&opcion);
	
	while(!((opcion>=1)&&(opcion<=6)))
	{
		printf("Elija una opcion valida.\n");
		printf("%d",opcion);
		texto();
		fflush(stdin);
		scanf("%d",&opcion);
	}
	
	clnt = clnt_create(srv, PROY1EJ5, PROY1EJ5VERS,"tcp");

	if(clnt == (CLIENT*)NULL)
	{
		clnt_pcreateerror(srv);
		exit(2);
	}
	
	struct timeval begin, end;
    Mensaje *msg_to_send = malloc(sizeof(Mensaje));
	Mensaje *msg_to_rec = malloc(sizeof(Mensaje));
	
	switch(opcion)
	{
		case 1:
			printf("Ingrese un numero decimal para convertir a binario.\n");			
			fflush(stdin);
			scanf("%d",&(msg_to_send->datos[0]));	
			gettimeofday(&begin,NULL);
			msg_to_rec = dectobin_1(msg_to_send,clnt);
			gettimeofday(&end,NULL);
			printf("Resultado: %d\n",msg_to_rec->datos[0]);
			break;
		case 2:
			printf("Ingrese un numero binario para convertir a hexadecimal.\n");			
			fflush(stdin);
			scanf("%d",&(msg_to_send->datos[0]));	
			gettimeofday(&begin,NULL);
			char **resp = bintohex_1(msg_to_send,clnt);
			gettimeofday(&end,NULL);
			printf("Resultado: %s\n",*resp);
			break;
		case 3:
			printf("Ingrese hasta 4 numeros para sumarlos.\n");
			for(int i = 0; i < 4; i++)
			{
				printf("Ingrese el operando %d: \n",(i+1));
				fflush(stdin);
				scanf("%d",&(msg_to_send->datos[i]));
			}
			gettimeofday(&begin,NULL);
			msg_to_rec = suma_1(msg_to_send,clnt);
			gettimeofday(&end,NULL);
			printf("Resultado: %d\n",msg_to_rec->datos[0]);
			break;
		case 4:
			printf("Ingrese hasta 4 numeros para restarlos.\n");
			for(int i = 0; i < 4; i++)
			{
				printf("Ingrese el operando %d: \n",(i+1));
				fflush(stdin);
				scanf("%d",&(msg_to_send->datos[i]));
			}
			gettimeofday(&begin,NULL);
			msg_to_rec = resta_1(msg_to_send,clnt);
			gettimeofday(&end,NULL);
			printf("Resultado: %d\n",msg_to_rec->datos[0]);
			break;
		case 5:
			printf("Ingrese hasta 2 numeros para multiplicarlos.\n");
			for(int i = 0; i < 2; i++)
			{
				printf("Ingrese el operando %d: \n",(i+1));
				fflush(stdin);
				scanf("%d",&(msg_to_send->datos[i]));
			}
			gettimeofday(&begin,NULL);
			msg_to_rec = multiplicacion_1(msg_to_send,clnt);
			gettimeofday(&end,NULL);
			printf("Resultado: %d\n",msg_to_rec->datos[0]);
			break;
		case 6:
			printf("Ingrese hasta 2 numeros para dividirlos.\n");
			for(int i = 0; i < 2; i++)
			{
				printf("Ingrese el operando %d: \n",(i+1));
				fflush(stdin);
				scanf("%d",&(msg_to_send->datos[i]));
			}
			gettimeofday(&begin,NULL);
			msg_to_rec = division_1(msg_to_send,clnt);
			gettimeofday(&end,NULL);
			printf("Resultado: %d, resto: %d\n",msg_to_rec->datos[0],msg_to_rec->datos[1]);
			break;
		default:
			exit(0);		
	}
	
	unsigned int t = end.tv_usec - begin.tv_usec;
    printf("tiempo en microsegundos: %u\n",t);

	clnt_destroy(clnt);
	return 0;
}
