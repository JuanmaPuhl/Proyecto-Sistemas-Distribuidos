#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
		printf("Debe ingresar el hostname\n");
		exit(0);
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
		printf("Elija una opcion valido.\n");
		printf("%d",opcion);
		texto();
		fflush(stdin);
		scanf("%d",&opcion);
	}

	clnt = clnt_create(srv, PROY1EJ5, PROY1EJ5VERS, "tcp");
	
	int *resentera;
	Double *resreal = malloc(sizeof(Double));
	unsigned long int *resbin;
	String *reshex = malloc(sizeof(String));
	Cuadrupla *cuadr = malloc(sizeof(Cuadrupla));
	Dupla *dupla = malloc(sizeof(Dupla));
	struct timeval begin, end;

	switch (opcion)
	{
	case 1:
		printf("Ingrese un numero decimal para convertirlo a binario.\n");
		int numdecimal;
		fflush(stdin);
		scanf("%d",&numdecimal);
		gettimeofday(&begin,NULL);
		resbin = decabin_1(&numdecimal, clnt);
		gettimeofday(&end,NULL);
		printf("Resultado = %lu\n",*resbin);
		break;
	case 2:
		printf("Ingrese un numero binario para convertirlo a hexadecimal.\n");
		unsigned long int numbinario;
		fflush(stdin);
		scanf("%lu",&numbinario);
		gettimeofday(&begin,NULL);
		reshex = binahex_1(&numbinario, clnt);
		gettimeofday(&end,NULL);
		printf("Resultado = %s\n",reshex->val);
		break;
	case 3:
		printf("Ingrese hasta 4 operandos para sumarlos.\n");
		for(int i = 0; i < 4; i++)
		{
			printf("Ingrese el operando %d: \n",i);
			fflush(stdin);
			scanf("%d",&(cuadr->operandos[i]));
		}
		gettimeofday(&begin,NULL);
		resentera = suma_1(cuadr, clnt);
		gettimeofday(&end,NULL);
		printf("Resultado = %d\n",*resentera);
		break;
	case 4:
		printf("Ingrese hasta 4 operandos para restarlos.\n");
		for(int i = 0; i < 4; i++)
		{
			printf("Ingrese el operando %d: \n",i);
			fflush(stdin);
			scanf("%d",&(cuadr->operandos[i]));
		}
		gettimeofday(&begin,NULL);
		resentera = resta_1(cuadr, clnt);
		gettimeofday(&end,NULL);
		printf("Resultado = %d\n",*resentera);
		break;
	case 5:
		printf("Ingrese dos numeros para multiplicarlos.\n");
		printf("Ingrese el primer operando");
		fflush(stdin);
		scanf("%d",&(dupla->a));
		printf("Ingrese el segundo operando");
		fflush(stdin);
		scanf("%d",&(dupla->b));
		gettimeofday(&begin,NULL);
		resentera = multiplicacion_1(dupla, clnt);
		gettimeofday(&end,NULL);
		printf("Resultado = %d\n",*resentera);
		break;
	case 6:
		printf("Ingrese dos numeros para dividirlos.\n");
		printf("Ingrese el dividendo");
		fflush(stdin);
		scanf("%d",&(dupla->a));
		printf("Ingrese el divisor");
		fflush(stdin);
		scanf("%d",&(dupla->b));
		if(&(dupla->b) == 0)
		{
			printf("No se puede dividir por cero.");
			exit(0);
		}
		gettimeofday(&begin,NULL);
		resreal = division_1(dupla, clnt);
		gettimeofday(&end,NULL);
		printf("Resultado = %lf\n",resreal->val);
		break;
	default:
		exit(0);
	}

	unsigned int t = end.tv_usec - begin.tv_usec;
    printf("tiempo en microsegundos: %u\n",t);

	clnt_destroy(clnt);
	return 0;
}
