#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv, char *envp[])
{
	int N = 5000; //Este es un valor por defecto
	if(argc > 1) //Si el usuario ingreso algun argumento...
		N = atoi(argv[0]);//Obtengo la cantidad de iteraciones
	int i;
	for(i=0; i<N; i++)//Itero hasta N
	{
		/**Realizo dos operaciones aritmeticas arbitrarias*/
		int j = 5+3;
		double h = (double)35/(double)4;
	}
	/**Escribo en un archivo*/
	FILE* fp;
	fp = fopen("child.txt","w");
	fprintf(fp, "Escribi desde la imagen %d.",N); 
	fclose(fp);
	/**Termino la ejecucion de la imagen*/
	return 0;
}

